#ifdef __ANDROID__

#include "AndroidJNIUtils.h"

static JavaVM *g_javaVM = 0;
static jclass g_midiBridgeClass = 0;

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    g_javaVM = vm;

    JNIEnv *env;
    if (vm->GetEnv((void **)&env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    // Resolved once here (guaranteed to be a JVM-attached thread with the
    // app's classloader) so background native threads never need to call
    // FindClass(), which fails on threads not created by the JVM.
    jclass localClass = env->FindClass("org/picoloop/android/MidiBridge");
    if (localClass) {
        g_midiBridgeClass = (jclass)env->NewGlobalRef(localClass);
        env->DeleteLocalRef(localClass);
    } else {
        env->ExceptionClear();
    }

    return JNI_VERSION_1_6;
}

JNIEnv *AcquireJNIEnv(bool *didAttach) {
    *didAttach = false;
    if (!g_javaVM) {
        return 0;
    }

    JNIEnv *env = 0;
    jint status = g_javaVM->GetEnv((void **)&env, JNI_VERSION_1_6);
    if (status == JNI_EDETACHED) {
        if (g_javaVM->AttachCurrentThread(&env, 0) != JNI_OK) {
            return 0;
        }
        *didAttach = true;
    } else if (status != JNI_OK) {
        return 0;
    }
    return env;
}

void ReleaseJNIEnv(bool didAttach) {
    if (didAttach && g_javaVM) {
        g_javaVM->DetachCurrentThread();
    }
}

jclass GetMidiBridgeClass() { return g_midiBridgeClass; }

#endif // __ANDROID__
