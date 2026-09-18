// JNI entry points called from Kotlin (NativeBridge.kt) for the Android
// autosave/quit lifecycle. See PatternPlayer.cpp's autosaveCurrentState()
// for what actually gets saved.
//
// These call straight into native code synchronously on whatever thread
// invokes them (the Java UI thread), rather than setting a flag for the
// native main loop to notice later: SDLActivity suspends the native thread
// on onPause() (pauseNativeThread() in SDLActivity.java), so a flag set
// around that point could race and not be picked up until the app is
// resumed again - too late for a background-kill. A direct call sidesteps
// that. This does touch P[]/PatternReader from a non-native thread with no
// locking, which is fine in practice: by the time onPause() fires (or Quit
// is pressed) nothing else is concurrently mutating that state.
//
// Neither of these calls exit()/killProcess() - that's deliberate. Killing
// the process while its task still exists in Recents looks like a crash to
// Android, which then auto-relaunches the last activity to "recover" it
// (confirmed on-device: Quit would immediately reopen picoloop). Closing is
// done the normal Android way instead - SettingsActivity.kt calls
// finishAndRemoveTask() right after these return, which removes the task
// cleanly, and no relaunch happens.

#include <jni.h>
#include "UserInterface.h"
#include "SYSTEM.h"

extern "C" JNIEXPORT void JNICALL
Java_org_picoloop_android_NativeBridge_triggerAutosave(JNIEnv *, jobject)
{
  autosaveCurrentState();
}

extern "C" JNIEXPORT jstring JNICALL
Java_org_picoloop_android_NativeBridge_getStorageFolder(JNIEnv * env, jobject)
{
  return env->NewStringUTF(GETPICOLOOPUSERSTORAGE());
}

extern "C" JNIEXPORT void JNICALL
Java_org_picoloop_android_NativeBridge_triggerQuit(JNIEnv *, jobject)
{
  autosaveCurrentState();
}
