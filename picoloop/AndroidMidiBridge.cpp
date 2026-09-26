#include "AndroidMidiBridge.h"
#include "AndroidJNIUtils.h"
#include <cstdlib>

// Kotlin side lives in android/app/src/main/java/org/picoloop/android/MidiBridge.kt
// listPorts() encodes each port as "deviceId|portIndex|DIR|name" to keep the
// JNI surface to a single String[] round trip instead of custom object
// marshalling.

std::vector<AndroidMidiPortInfo> AndroidMidi_ListPorts() {
    std::vector<AndroidMidiPortInfo> ports;

    jclass bridgeClass = GetMidiBridgeClass();
    if (!bridgeClass) {
        return ports;
    }

    bool didAttach = false;
    JNIEnv *env = AcquireJNIEnv(&didAttach);
    if (!env) {
        return ports;
    }

    jmethodID listPortsMethod = env->GetStaticMethodID(
        bridgeClass, "listPorts", "()[Ljava/lang/String;");
    if (!listPortsMethod) {
        env->ExceptionClear();
        ReleaseJNIEnv(didAttach);
        return ports;
    }

    jobjectArray array =
        (jobjectArray)env->CallStaticObjectMethod(bridgeClass, listPortsMethod);

    if (array) {
        jsize count = env->GetArrayLength(array);
        for (jsize i = 0; i < count; i++) {
            jstring jline = (jstring)env->GetObjectArrayElement(array, i);
            const char *cline = env->GetStringUTFChars(jline, 0);
            std::string line(cline);
            env->ReleaseStringUTFChars(jline, cline);
            env->DeleteLocalRef(jline);

            size_t p1 = line.find('|');
            size_t p2 = (p1 == std::string::npos) ? std::string::npos
                                                   : line.find('|', p1 + 1);
            size_t p3 = (p2 == std::string::npos) ? std::string::npos
                                                   : line.find('|', p2 + 1);
            if (p1 == std::string::npos || p2 == std::string::npos ||
                p3 == std::string::npos) {
                continue;
            }

            AndroidMidiPortInfo info;
            info.deviceId = atoi(line.substr(0, p1).c_str());
            info.portIndex = atoi(line.substr(p1 + 1, p2 - p1 - 1).c_str());
            info.isInput = (line.substr(p2 + 1, p3 - p2 - 1) == "IN");
            info.name = line.substr(p3 + 1);
            ports.push_back(info);
        }
        env->DeleteLocalRef(array);
    }

    ReleaseJNIEnv(didAttach);
    return ports;
}

jobject AndroidMidi_OpenDevice(int deviceId) {
    jclass bridgeClass = GetMidiBridgeClass();
    if (!bridgeClass) {
        return 0;
    }

    bool didAttach = false;
    JNIEnv *env = AcquireJNIEnv(&didAttach);
    if (!env) {
        return 0;
    }

    jobject result = 0;
    jmethodID openMethod =
        env->GetStaticMethodID(bridgeClass, "openDeviceBlocking",
                                "(I)Landroid/media/midi/MidiDevice;");
    if (openMethod) {
        jobject device =
            env->CallStaticObjectMethod(bridgeClass, openMethod, deviceId);
        if (device) {
            result = env->NewGlobalRef(device);
            env->DeleteLocalRef(device);
        }
    } else {
        env->ExceptionClear();
    }

    ReleaseJNIEnv(didAttach);
    return result;
}

void AndroidMidi_CloseDevice(jobject deviceGlobalRef) {
    if (!deviceGlobalRef) {
        return;
    }

    jclass bridgeClass = GetMidiBridgeClass();
    bool didAttach = false;
    JNIEnv *env = AcquireJNIEnv(&didAttach);
    if (env) {
        if (bridgeClass) {
            jmethodID closeMethod = env->GetStaticMethodID(
                bridgeClass, "closeDevice",
                "(Landroid/media/midi/MidiDevice;)V");
            if (closeMethod) {
                env->CallStaticVoidMethod(bridgeClass, closeMethod,
                                           deviceGlobalRef);
            } else {
                env->ExceptionClear();
            }
        }
        env->DeleteGlobalRef(deviceGlobalRef);
    }
    ReleaseJNIEnv(didAttach);
}
