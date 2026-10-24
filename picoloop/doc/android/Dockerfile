# Build environment for the Android target (picoloop/android).
#
# This is our own image. It is what the Android build should use once it has
# been exercised; until then doc/android/build.md documents the ready-made
# mingc/android-build-box image that is used today.
#
# What the build needs (see doc/android/build.md):
#   - JDK 17+            (the committed Gradle wrapper is 8.13)
#   - Android platform 36, build-tools 36  (compileSdk/targetSdk = 36)
#   - an NDK             (picoloop/Makefile.PatternPlayer_android_SDL2 reads
#                         ANDROID_NDK_HOME and calls the NDK's clang directly)
#   - unzip/zip/make,    (SDL2 + SDL2_ttf are ndk-built by the Gradle build)
#     git, curl
#
# Everything downloaded here is pinned: the cmdline-tools archive by version +
# SHA-256, the SDK components and the NDK by exact version.
#
# Build:  podman build -t picoloop-android -f doc/android/Dockerfile doc/android
# Use:    podman run --rm -v "$PWD":/src:z -w /src/android picoloop-android \
#             ./gradlew assembleDebug
FROM docker.io/library/ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

ARG CMDLINE_TOOLS_VERSION=15859902
ARG CMDLINE_TOOLS_SHA256=4e4c464f145a7512b57d088ac6c278c03c9eea610886b35a5e0804e74eedf583
ARG ANDROID_PLATFORM=36
ARG BUILD_TOOLS_VERSION=36.0.0
ARG NDK_VERSION=27.2.12479018

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        ca-certificates \
        curl \
        file \
        git \
        make \
        openjdk-17-jdk-headless \
        unzip \
        xz-utils \
        zip \
    && rm -rf /var/lib/apt/lists/*

ENV JAVA_HOME=/usr/lib/jvm/java-17-openjdk-amd64
ENV ANDROID_SDK_ROOT=/opt/android-sdk
ENV ANDROID_HOME=${ANDROID_SDK_ROOT}
ENV PATH=${PATH}:${ANDROID_SDK_ROOT}/cmdline-tools/latest/bin:${ANDROID_SDK_ROOT}/platform-tools

# cmdline-tools: pinned archive, checksum verified before use.
RUN set -eu; \
    mkdir -p /tmp/clt; \
    cd /tmp/clt; \
    curl -fsSL -o clt.zip \
        "https://dl.google.com/android/repository/commandlinetools-linux-${CMDLINE_TOOLS_VERSION}_latest.zip"; \
    echo "${CMDLINE_TOOLS_SHA256}  clt.zip" | sha256sum -c -; \
    mkdir -p "${ANDROID_SDK_ROOT}/cmdline-tools"; \
    unzip -q clt.zip -d "${ANDROID_SDK_ROOT}/cmdline-tools"; \
    mv "${ANDROID_SDK_ROOT}/cmdline-tools/cmdline-tools" "${ANDROID_SDK_ROOT}/cmdline-tools/latest"; \
    rm -rf /tmp/clt

# SDK components. The licences have to be accepted non-interactively; `yes`
# gets SIGPIPE once sdkmanager stops reading, hence the `|| true`.
RUN yes | sdkmanager --licenses > /dev/null 2>&1 || true; \
    sdkmanager --install \
        "platform-tools" \
        "platforms;android-${ANDROID_PLATFORM}" \
        "build-tools;${BUILD_TOOLS_VERSION}" \
        "ndk;${NDK_VERSION}" \
    && sdkmanager --list_installed

ENV ANDROID_NDK_HOME=${ANDROID_SDK_ROOT}/ndk/${NDK_VERSION}
ENV ANDROID_NDK_ROOT=${ANDROID_NDK_HOME}

WORKDIR /src

CMD ["/bin/bash"]
