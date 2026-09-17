#ifndef _ANDROID_MIDI_BRIDGE_H_
#define _ANDROID_MIDI_BRIDGE_H_

#include <jni.h>
#include <string>
#include <vector>

struct AndroidMidiPortInfo {
    int deviceId;
    int portIndex;
    bool isInput; // true: device's IN port (we send to it) - usable as a
                  // MIDI output. false: device's OUT port (we receive from
                  // it) - usable as a MIDI input.
    std::string name;
};

// Lists all MIDI ports currently visible to android.media.midi.MidiManager
// (USB and virtual devices alike). Safe to call from the main/SDL thread.
std::vector<AndroidMidiPortInfo> AndroidMidi_ListPorts();

// Opens (synchronously blocks on the async Android callback) the device
// owning a port and returns a global JNI reference to the resulting
// android.media.midi.MidiDevice, or NULL on failure/timeout.
jobject AndroidMidi_OpenDevice(int deviceId);

// Closes a device previously returned by AndroidMidi_OpenDevice() and
// releases the global reference.
void AndroidMidi_CloseDevice(jobject deviceGlobalRef);

#endif
