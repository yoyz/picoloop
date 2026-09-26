#ifndef _ANDROID_JNI_UTILS_H_
#define _ANDROID_JNI_UTILS_H_

#ifdef __ANDROID__

#include <jni.h>

// Attaches the current native thread to the JVM if it isn't already.
// *didAttach is set to true if this call attached the thread, in which
// case the caller must pass it back to ReleaseJNIEnv() when done.
JNIEnv *AcquireJNIEnv(bool *didAttach);
void ReleaseJNIEnv(bool didAttach);

// Global ref to org/picoloop/android/MidiBridge, resolved once in
// JNI_OnLoad (safe from any thread afterwards).
jclass GetMidiBridgeClass();

#endif // __ANDROID__
#endif
