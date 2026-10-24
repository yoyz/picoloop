# Building picoloop for Android

The Android build produces an APK for `org.picoloop.android` (API 29+, `armv7`
and `arm64-v8a`). The Gradle build does three things:

1. downloads SDL2 **2.30.9** and SDL2_ttf **2.22.0** and ndk-builds them into
   `android/app/src/main/jniLibs/<abi>/{libSDL2.so,libSDL2_ttf.so}`,
2. cross-compiles picoloop itself with `picoloop/Makefile.PatternPlayer_android_SDL2`
   (NDK clang, `ANDROID_API ?= 29`),
3. packages both into the APK.

There are two container environments:

| | image | status |
|---|---|---|
| **A** | `mingc/android-build-box:latest`, pinned by digest | **used today**, APK built with it (section 2) |
| **B** | `doc/android/Dockerfile` (this repo) | our own equivalent, for when A is not available or has to be pinned differently (section 3) |

Reference layout:

    picoloop/                       repo root
    android/                        Gradle project (Kotlin DSL, wrapper 8.13)
    picoloop/Makefile.PatternPlayer_android_SDL2   native build (NDK clang)
    doc/android/Dockerfile          our build image
    doc/android/build.md            this file

## 1. What the project requires

| item | value | where |
|---|---|---|
| Gradle | 8.13 (wrapper is committed) | `android/gradle/wrapper/gradle-wrapper.properties` |
| JDK | 17+ (Gradle 8.13 + AGP for SDK 36) | - |
| SDK | `compileSdk = 36`, `targetSdk = 36`, `minSdk = 29` | `android/app/build.gradle.kts` |
| build-tools | 36.0.0 (any recent 36.x) | - |
| ABIs | `armeabi-v7a`, `arm64-v8a` | `android/app/build.gradle.kts` |
| NDK | any recent; read from `ANDROID_NDK_HOME` | `picoloop/Makefile.PatternPlayer_android_SDL2` |
| SDL2 / SDL2_ttf | 2.30.9 / 2.22.0, downloaded by Gradle | `android/app/build.gradle.kts` |

`minSdk = 29` is deliberate: the native MIDI code uses `<amidi/AMidi.h>`
(`MidiInSystem.cpp` / `MidiOutSystem.cpp`).

## 2. Build with the ready-made image (what we use now)

Pin by digest, not by tag - `latest` is rebuilt weekly and moves:

    digest   sha256:47a26138302605eb8a37b024e8a263af0812c14800813458bc674df47cc26331
    size     5.9 GB compressed / 14.2 GB on disk
    contents Ubuntu 22.04, JDK 8/11/17/21 (jenv), SDK platforms 29..36 with
             build-tools, NDK (r28b), cmdline-tools/sdkmanager, Flutter, Node

    mkdir -p "$HOME/.cache/picoloop-android-gradle"

    podman run --rm --userns=keep-id \
        -v "$PWD":/src:z \
        -v "$HOME/.cache/picoloop-android-gradle":/tmp/gradle:z \
        -w /src/android -e HOME=/tmp -e GRADLE_USER_HOME=/tmp/gradle \
        mingc/android-build-box@sha256:47a26138302605eb8a37b024e8a263af0812c14800813458bc674df47cc26331 \
        bash -lc 'unset ANDROID_SDK_HOME ANDROID_PREFS_ROOT; \
                  export ANDROID_USER_HOME=/tmp/android-home; \
                  ./gradlew assembleDebug'

Notes on that command (each one is a failure that was hit and fixed):

- `--userns=keep-id` + `HOME=/tmp` keeps the Gradle caches, the debug keystore
  and the output files owned by you; the image itself expects root.
- `unset ANDROID_SDK_HOME ANDROID_PREFS_ROOT` + `ANDROID_USER_HOME` in a
  writable directory: the image sets `ANDROID_SDK_HOME=/opt/android-sdk`, and
  AGP then tries to create its preferences (and the debug keystore) there.
  Leaving it set gives either
  `Unable to create debug keystore in /opt/android-sdk/.android because it is
  not writable` or `Several environment variables ... contain different paths
  to the Android Preferences folder`.
- `GRADLE_USER_HOME` on a host directory survives `--rm`, so only the first
  build downloads Gradle 8.13 and the AGP dependencies (1 minute instead of 12).
- `ANDROID_NDK_HOME` is used by `android/app/build.gradle.kts` to set
  `ndkVersion`: AGP resolves the NDK as `<sdk>/ndk/<version>`, and the version
  differs per image (`mingc` currently ships `30.0.15729638`, our Dockerfile
  pins `27.2.12479018`). Without that, the build stops with
  `NDK is not installed`.

Docker instead of podman: replace `--userns=keep-id` with
`-u "$(id -u):$(id -g)"` (the `:z` SELinux label is podman-only).

### 2.1 Result (verified)

With the image above, on b550:

| step | time |
|---|---|
| first build (Gradle 8.13 + AGP deps + SDL2/SDL2_ttf ndk-build + picoloop native x2 ABIs) | 11 min 50 s |
| rebuild with the Gradle cache mounted (`assembleDebug`, nothing changed) | 24 s |

    android/app/build/outputs/apk/debug/app-debug.apk        29 033 402 bytes

    package: name='org.picoloop.android' versionCode='2' versionName='0.80a'
             compileSdkVersion='36' minSdkVersion='29' targetSdkVersion='36'
    lib/armeabi-v7a/{libmain.so,libSDL2.so,libSDL2_ttf.so,libc++_shared.so}
    lib/arm64-v8a/  {libmain.so,libSDL2.so,libSDL2_ttf.so,libc++_shared.so}
    classes{,2,3,4}.dex

The APK is **not installed or run** on a device as part of this procedure.

## 3. Build with our own image (section B)

    podman build -t picoloop-android -f doc/android/Dockerfile doc/android

    podman run --rm --userns=keep-id \
        -v "$PWD":/src:z -w /src/android \
        -e HOME=/tmp -e GRADLE_USER_HOME=/tmp/gradle \
        picoloop-android ./gradlew assembleDebug

What is pinned in `doc/android/Dockerfile`:

| component | value |
|---|---|
| base | `docker.io/library/ubuntu:24.04` |
| JDK | `openjdk-17-jdk-headless` |
| cmdline-tools | `15859902`, SHA-256 `4e4c464f145a7512b57d088ac6c278c03c9eea610886b35a5e0804e74eedf583` (verified before unpacking) |
| SDK | `platform-tools`, `platforms;android-36`, `build-tools;36.0.0` |
| NDK | `ndk;27.2.12479018` (r27c), exported as `ANDROID_NDK_HOME` |
| extras | `git make unzip zip curl file xz-utils` |

All of those are `ARG`s, so bumping the toolchain is a one-line change. The
image is ~3 GB and contains only what the build needs (no Flutter, no Node, no
emulator).

**Status:** this Dockerfile has not been built or used for an APK yet - the
0.80a APK was produced with the image from section 2. Rebuild it and run the
command above before trusting it; the only expected difference is the NDK
version (r27c here vs r28b in `mingc`).

## 4. Outputs

After `assembleDebug`:

    android/app/build/outputs/apk/debug/app-debug.apk

The APK carries `lib/armeabi-v7a/libSDL2.so`, `libSDL2_ttf.so`,
`libmain.so` (picoloop) and the same set under `lib/arm64-v8a/`.

Install on a device (USB debugging enabled):

    adb install -r android/app/build/outputs/apk/debug/app-debug.apk

The package is `org.picoloop.android`; storage is the app's private directory
(`SYSTEMANDROID.cpp`), and the sample bank/patterns are **not** bundled.

## 5. Rebuilding and cleaning

    # inside the container, as in section 2 / 3
    ./gradlew clean

    # on the host: remove the generated native libs and Gradle state
    rm -rf android/app/src/main/jniLibs android/build android/app/build android/.gradle

    # remove the image
    podman rmi mingc/android-build-box        # or: picoloop-android

Run `./gradlew clean` before switching images, so the native `.so` files from
one NDK are not reused with the other.

## 6. The ready-made image: provenance and caveats

Why it is acceptable for now, and what to keep in mind:

| check | result |
|---|---|
| source | `github.com/mingchen/docker-android-build-box`, MIT, 624 stars, created 2016, active (last push 2026-10) |
| registry | `mingc/android-build-box`, 1.67M pulls, `latest` pushed 2026-08 |
| provenance | OCI labels point at that repo and at commit `3d9db22…`; built by GitHub Actions in the repo, actions pinned by commit SHA |
| signature | **none** (no cosign/attestation), so the labels are a claim, not a proof |

Caveats:

- one maintainer (bus factor 1);
- `latest` is rebuilt weekly: SDK/build-tools/NDK/JDK contents move;
- the Dockerfile pipes a `curl -k` (TLS verification off) NodeSource script
  into bash, and the Android cmdline-tools download is not checksum-verified;
- runs as root by default; 5.9 GB compressed / 14.2 GB on disk, amd64 only.

This is why section 3 exists: when the APK has to be reproducible, build
`doc/android/Dockerfile` instead and pin *it* by digest.

## 7. Troubleshooting

| symptom | cause | fix |
|---|---|---|
| `Failed to install the following SDK components ... platform 36` | SDK platform missing in the image | `sdkmanager --install "platforms;android-36" "build-tools;36.0.0"` (accept licences once with `yes \| sdkmanager --licenses`) |
| `NDK not configured` / `ANDROID_NDK_HOME is not set` | image without an NDK, or a different env name | set `ANDROID_NDK_HOME=/opt/android-sdk/ndk/<ver>`; `picoloop/Makefile.PatternPlayer_android_SDL2` reads it via `NDK_PATH ?= $(ANDROID_NDK_HOME)` |
| `Unsupported class file major version` / Gradle refuses to start | JDK 8/11 is the default (jenv in `mingc`) | `jenv global 17` (source `~/.bash_profile` first), or use our Dockerfile (JDK 17 only) |
| `Could not resolve ...` for AGP/Kotlin | no network, or a proxy | the first build needs to download Gradle 8.13 + dependencies |
| Gradle dies with `OutOfMemoryError` | default JVM heap in a container | `-e GRADLE_OPTS="-Xmx4g"` or adjust `org.gradle.jvmargs` |
| `Unable to create debug keystore in /opt/android-sdk/.android` | image sets `ANDROID_SDK_HOME=/opt/android-sdk`, not writable by your uid | `unset ANDROID_SDK_HOME ANDROID_PREFS_ROOT` and set `ANDROID_USER_HOME` to a writable dir (see section 2) |
| `Several environment variables ... contain different paths to the Android Preferences folder` | `ANDROID_USER_HOME` and `ANDROID_SDK_HOME` disagree | unset the latter (same command) |
| `NDK is not installed` when configuring the native tasks | `ndkVersion` not set, AGP default not present in the image | `android/app/build.gradle.kts` now derives `ndkVersion` from `ANDROID_NDK_HOME` |
| output files owned by root | image runs as root and `--userns=keep-id` was omitted | `chown -R $(id -u):$(id -g) android/` or run with `--userns=keep-id` and `HOME=/tmp` |
| `libSDL2.so` missing in the APK | `jniLibs` was cleaned and only the Java part rebuilt | run a full `./gradlew assembleDebug` (the SDL tasks are wired into the build) |

## 8. Notes and limits

- `android/app/build.gradle.kts` was bumped from `versionName = "0.77d"` /
  `versionCode = 1` to **`"0.80a"` / `2`** for this build. `PICOLOOP_VERSION`
  in `Master.h` stays `"0.80"` (that is the in-app splash string), so the
  splash and the APK label differ by the trailing `a`.
- The Android makefile enables 11 engines and **no ER-1**; adding it is the
  usual two lines (`-DENABLE_SYNTH_ER1` + `$(SOURCES_ER1)`/`$(SOURCES_ER1UI)`).
- The APK is built here; it is **not** run on a device or emulator as part of
  this procedure. `adb install` and a manual smoke test are the next step.
