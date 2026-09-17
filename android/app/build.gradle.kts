plugins {
    id("com.android.application")
    id("org.jetbrains.kotlin.android")
}

// ---------------------------------------------------------------------------
// SDL2 / SDL2_ttf sources: downloaded once, built with ndk-build into
// src/main/jniLibs/<abi>/{libSDL2.so,libSDL2_ttf.so}. picoloop's own native
// code is then cross-compiled separately with picoloop/Makefile.PatternPlayer_android_SDL2
// (the project's usual Makefile-based build, not CMake) into libmain.so,
// linking against those two.
// ---------------------------------------------------------------------------

val sdlVersion = "2.30.9"
val sdlTtfVersion = "2.22.0"
val sdlUrl = "https://github.com/libsdl-org/SDL/releases/download/release-$sdlVersion/SDL2-$sdlVersion.tar.gz"
val sdlTtfUrl = "https://github.com/libsdl-org/SDL_ttf/releases/download/release-$sdlTtfVersion/SDL2_ttf-$sdlTtfVersion.tar.gz"

val sdlDownloadDir = layout.buildDirectory.dir("sdl-download").get().asFile
val sdlExtractDir = layout.buildDirectory.dir("sdl-extracted").get().asFile
val sdlDir = file("$sdlExtractDir/SDL2-$sdlVersion")
val sdlTtfDir = file("$sdlExtractDir/SDL2_ttf-$sdlTtfVersion")
// Single include root: SDL2/SDL.h and SDL2/SDL_ttf.h both resolve from here,
// matching this codebase's `#include <SDL2/...>` style.
val sdlCombinedIncludeDir = file("$sdlDir/SDL2")

val abiList = listOf("armeabi-v7a", "arm64-v8a")

android {
    namespace = "org.picoloop.android"
    compileSdk = 36

    defaultConfig {
        applicationId = "org.picoloop.android"
        // 29: required by the native MIDI API (<amidi/AMidi.h>) used for
        // MIDI support - see MidiInSystem.cpp / MidiOutSystem.cpp.
        minSdk = 29
        targetSdk = 36
        versionCode = 1
        versionName = "0.77d"

        ndk {
            abiFilters += abiList
        }
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_11
        targetCompatibility = JavaVersion.VERSION_11
    }

    kotlinOptions {
        jvmTarget = "11"
    }

    sourceSets {
        getByName("main") {
            jniLibs.srcDirs("src/main/jniLibs")
        }
    }
}

val downloadSDL2 by tasks.registering {
    group = "SDL2"
    val outputFile = file("$sdlDownloadDir/SDL2-$sdlVersion.tar.gz")
    outputs.file(outputFile)
    doLast {
        sdlDownloadDir.mkdirs()
        if (!outputFile.exists()) {
            ant.invokeMethod("get", mapOf("src" to sdlUrl, "dest" to outputFile, "verbose" to true))
        }
    }
}

val downloadSDL2Ttf by tasks.registering {
    group = "SDL2"
    val outputFile = file("$sdlDownloadDir/SDL2_ttf-$sdlTtfVersion.tar.gz")
    outputs.file(outputFile)
    doLast {
        sdlDownloadDir.mkdirs()
        if (!outputFile.exists()) {
            ant.invokeMethod("get", mapOf("src" to sdlTtfUrl, "dest" to outputFile, "verbose" to true))
        }
    }
}

val extractSDL2 by tasks.registering(Copy::class) {
    group = "SDL2"
    dependsOn(downloadSDL2)
    from(tarTree(resources.gzip("$sdlDownloadDir/SDL2-$sdlVersion.tar.gz")))
    into(sdlExtractDir)
    onlyIf { !sdlDir.exists() }
}

val extractSDL2Ttf by tasks.registering(Copy::class) {
    group = "SDL2"
    dependsOn(downloadSDL2Ttf)
    from(tarTree(resources.gzip("$sdlDownloadDir/SDL2_ttf-$sdlTtfVersion.tar.gz")))
    into(sdlExtractDir)
    onlyIf { !sdlTtfDir.exists() }
}

// Populate SDL2-<ver>/SDL2/ with every header both libraries expose, so a
// single -I(sdlDir) satisfies '#include <SDL2/SDL.h>' and '#include <SDL2/SDL_ttf.h>'.
val prepareCombinedHeaders by tasks.registering {
    group = "SDL2"
    dependsOn(extractSDL2, extractSDL2Ttf)
    outputs.dir(sdlCombinedIncludeDir)
    doLast {
        copy {
            from("$sdlDir/include")
            into(sdlCombinedIncludeDir)
        }
        copy {
            from(sdlTtfDir) { include("SDL_ttf.h") }
            into(sdlCombinedIncludeDir)
        }
    }
}

// Single ndk-build pass building SDL2 core + SDL2_ttf (which itself pulls in
// the freetype/harfbuzz sources vendored under SDL2_ttf's external/) straight
// from their upstream Android.mk files - same approach as SDL2-only apps,
// just with a second `include` line.
val buildSDLNativeLibs by tasks.registering {
    group = "SDL2"
    dependsOn(prepareCombinedHeaders)

    val jniDir = file("src/main/jni")
    val outputs_ = abiList.flatMap { abi ->
        listOf(
            file("src/main/jniLibs/$abi/libSDL2.so"),
            file("src/main/jniLibs/$abi/libSDL2_ttf.so")
        )
    }
    outputs.files(outputs_)
    onlyIf { outputs_.any { !it.exists() } }

    doLast {
        jniDir.mkdirs()
        file("$jniDir/Application.mk").writeText(
            """
            APP_PLATFORM := android-29
            APP_ABI := ${abiList.joinToString(" ")}
            APP_STL := c++_shared
            APP_LDFLAGS := -Wl,-z,max-page-size=16384
            """.trimIndent()
        )
        file("$jniDir/Android.mk").writeText(
            """
            LOCAL_PATH := ${'$'}(call my-dir)
            include ${'$'}(CLEAR_VARS)
            include $sdlDir/Android.mk
            include $sdlTtfDir/Android.mk
            """.trimIndent()
        )
        exec {
            workingDir = file("src/main")
            commandLine(
                "${android.ndkDirectory}/ndk-build",
                "NDK_PROJECT_PATH=.",
                "NDK_OUT=$jniDir/obj",
                "NDK_LIBS_OUT=jniLibs"
            )
        }
        delete(jniDir)
    }
}

// One Exec task per ABI, invoking picoloop's own Makefile-based Android build
// (picoloop/Makefile.PatternPlayer_android_SDL2) rather than duplicating the
// source list here - keeps a single source of truth in Makefile_sources.
val picoloopNativeDir = file("$projectDir/../../picoloop")

fun registerPicoloopNativeTask(abi: String) =
    tasks.register<Exec>("buildPicoloopNative-$abi") {
        group = "Native Build"
        dependsOn(buildSDLNativeLibs)

        val libDir = file("src/main/jniLibs/$abi")
        val outputLib = file("$libDir/libmain.so")

        inputs.dir(picoloopNativeDir).withPropertyName("picoloopSources")
        outputs.file(outputLib)

        workingDir = picoloopNativeDir
        commandLine(
            "make", "-f", "Makefile.PatternPlayer_android_SDL2",
            "ABI=$abi",
            "ANDROID_NDK_HOME=${android.ndkDirectory}",
            "SDL_INCLUDE=$sdlDir",
            "SDL_LIB=$libDir"
        )

        doLast {
            copy {
                from("$picoloopNativeDir/libmain_$abi.so")
                into(libDir)
                rename { "libmain.so" }
            }
        }
    }

val buildPicoloopNativeTasks = abiList.map { registerPicoloopNativeTask(it) }

tasks.named("preBuild") {
    dependsOn(buildPicoloopNativeTasks)
}

dependencies {
    // ActivityCompat/ContextCompat for the storage-permission flow in MainActivity.
    implementation("androidx.core:core-ktx:1.13.1")
}
