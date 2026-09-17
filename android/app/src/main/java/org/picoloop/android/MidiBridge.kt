package org.picoloop.android

import android.content.Context
import android.media.midi.MidiDevice
import android.media.midi.MidiDeviceInfo
import android.media.midi.MidiManager
import android.os.Handler
import android.os.Looper
import android.util.Log
import java.util.concurrent.CountDownLatch
import java.util.concurrent.TimeUnit

/**
 * Bridge to android.media.midi, called from native code
 * (picoloop/AndroidMidiBridge.cpp). Device/port discovery is synchronous on
 * this API so it can be called directly from native; opening a device is
 * callback-based on the Android side, so [openDeviceBlocking] blocks the
 * calling (native) thread on a latch until the callback fires or a timeout
 * is hit.
 */
object MidiBridge {
    private const val TAG = "MidiBridge"
    private var midiManager: MidiManager? = null
    private val handler = Handler(Looper.getMainLooper())

    @JvmStatic
    fun init(context: Context) {
        if (midiManager == null) {
            midiManager = context.getSystemService(Context.MIDI_SERVICE) as? MidiManager
            if (midiManager == null) {
                Log.w(TAG, "android.media.midi not available on this device")
            }
        }
    }

    // One entry per port, encoded as "deviceId|portIndex|DIR|name" so the
    // native side only needs a single String[] JNI round trip.
    @JvmStatic
    fun listPorts(): Array<String> {
        val mm = midiManager ?: return emptyArray()
        val result = mutableListOf<String>()
        for (info in mm.devices) {
            val deviceName = info.properties.getString(MidiDeviceInfo.PROPERTY_NAME)
                ?: info.properties.getString(MidiDeviceInfo.PROPERTY_PRODUCT)
                ?: "MIDI device ${info.id}"
            for (port in info.ports) {
                val dir = when (port.type) {
                    MidiDeviceInfo.PortInfo.TYPE_INPUT -> "IN"
                    MidiDeviceInfo.PortInfo.TYPE_OUTPUT -> "OUT"
                    else -> continue
                }
                val portName = if (port.name.isNullOrEmpty()) deviceName else "$deviceName ${port.name}"
                result.add("${info.id}|${port.portNumber}|$dir|${portName.replace('|', '/')}")
            }
        }
        Log.i(TAG, "Enumerated ${result.size} MIDI port(s)")
        return result.toTypedArray()
    }

    @JvmStatic
    fun openDeviceBlocking(deviceId: Int): MidiDevice? {
        val mm = midiManager ?: return null
        val info = mm.devices.firstOrNull { it.id == deviceId } ?: run {
            Log.w(TAG, "No MIDI device with id $deviceId")
            return null
        }

        val latch = CountDownLatch(1)
        var opened: MidiDevice? = null
        mm.openDevice(info, { device ->
            opened = device
            latch.countDown()
        }, handler)

        if (!latch.await(3, TimeUnit.SECONDS)) {
            Log.w(TAG, "Timed out opening MIDI device $deviceId")
        }
        return opened
    }

    @JvmStatic
    fun closeDevice(device: MidiDevice) {
        try {
            device.close()
        } catch (e: Exception) {
            Log.e(TAG, "Error closing MIDI device", e)
        }
    }
}
