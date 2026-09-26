package org.picoloop.android

/**
 * JNI entry points into the native autosave/quit lifecycle (see
 * picoloop/AndroidLifecycle.cpp). No System.loadLibrary here - the "main"
 * native library is already loaded by PicoloopActivity/SDLActivity by the
 * time any of these can be called.
 */
object NativeBridge {
    @JvmStatic
    external fun triggerAutosave()

    /** Saves state only - does NOT exit the process. Follow up with
     *  Activity.finishAndRemoveTask() on the Kotlin side to actually close
     *  (see SettingsActivity.kt's Quit button); native exit()/killProcess()
     *  looks like a crash to Android and triggers an unwanted auto-relaunch. */
    @JvmStatic
    external fun triggerQuit()

    /** The picoloop storage folder actually in use (may differ from
     *  MainActivity.PUBLIC_FOLDER_PATH if picoloop.ini's folder= redirects it). */
    @JvmStatic
    external fun getStorageFolder(): String
}
