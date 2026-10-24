#!/bin/sh
#
# build_sdk.sh - build the aarch64 cross-compilation SDK used by
#                Makefile.PatternPlayer_rg35xxsp (Anbernic RG35XX SP, KNULLI).
#
#   platform/rg35xxsp/build_sdk.sh            build (no-op if already built)
#   platform/rg35xxsp/build_sdk.sh --force    wipe and rebuild from scratch
#
# Everything is generated into  <repo>/rg35xxsp-sdk/  (gitignored):
#
#   rg35xxsp-sdk/src/        pinned source tarballs (kept for offline rebuilds)
#   rg35xxsp-sdk/build/      out-of-tree build dirs (removed after success)
#   rg35xxsp-sdk/usr/        install prefix the makefile links against
#                            usr/include/{SDL2,alsa} + SDL_ttf.h
#                            usr/lib/libSDL2*.so*, libasound.so*
#   rg35xxsp-sdk/device-lib/ the device's own glibc, imported once over adb
#   rg35xxsp-sdk/.stamp      versions built, used to skip redundant rebuilds
#
# Versions are pinned to the libraries the device image actually ships, so the
# headers the port compiles against match the runtime it links into:
#
#   SDL2       2.32.8    device: /usr/lib/libSDL2-2.0.so.0.3200.8
#   SDL2_ttf   2.24.0    device: /usr/lib/libSDL2_ttf-2.0.so.0.2400.0
#   alsa-lib   1.2.13    RtAudio ALSA backend (libasound.so.2)
#
# The picoloop binary itself is linked against the .so files in
# rg35xxsp-sdk/usr/lib (so no rpath is recorded) and at runtime the device's
# own /usr/lib copies are used - same sonames, same versions.
#
# The device runs glibc 2.40 (Buildroot) while the host cross toolchain ships
# glibc 2.43. Everything built here is therefore linked against the device's
# own libc/libm, imported once over adb into rg35xxsp-sdk/device-lib/; without
# that the binary would demand e.g. sqrtf@GLIBC_2.43 and fail to start.
#
# Requires: aarch64-linux-gnu-gcc-14/g++-14, make, curl, tar, a POSIX shell,
#           and - once - adb with the RG35XX SP attached (device glibc import).

set -eu

SDL2_VER=2.32.8
SDL2_TTF_VER=2.24.0
ALSA_VER=1.2.13

SDL2_SHA=0ca83e9c9b31e18288c7ec811108e58bac1f1bb5ec6577ad386830eac51c787e
SDL2_TTF_SHA=0b2bf1e7b6568adbdbc9bb924643f79d9dedafe061fa1ed687d1d9ac4e453bfd
ALSA_SHA=8c4ff37553cbe89618e187e4c779f71a9bb2a8b27b91f87ed40987cc9233d8f6

SDL2_URL=https://github.com/libsdl-org/SDL/releases/download/release-$SDL2_VER/SDL2-$SDL2_VER.tar.gz
SDL2_TTF_URL=https://github.com/libsdl-org/SDL_ttf/releases/download/release-$SDL2_TTF_VER/SDL2_ttf-$SDL2_TTF_VER.tar.gz
ALSA_URL=https://www.alsa-project.org/files/pub/lib/alsa-lib-$ALSA_VER.tar.bz2

ROOT=$(cd "$(dirname "$0")/../.." && pwd -P)
SDK="$ROOT/rg35xxsp-sdk"
SRC="$SDK/src"
BLD="$SDK/build"
PREFIX="$SDK/usr"
DEVLIB="$SDK/device-lib"
STAMP="$SDK/.stamp"

CROSS=aarch64-linux-gnu
CC=$CROSS-gcc-14
CXX=$CROSS-g++-14
JOBS=${JOBS:-$(nproc 2>/dev/null || echo 4)}
# Build triplet must be the *host* machine, otherwise configure sees
# --build == --host and refuses to cross compile.
BUILD_TRIPLET=$(gcc -dumpmachine 2>/dev/null || echo "$(uname -m)-linux-gnu")

msg()  { printf '==> %s\n' "$*"; }
warn() { printf '*** %s\n' "$*" >&2; }
die()  { printf '*** %s\n' "$*" >&2; exit 1; }

# ---------------------------------------------------------------- arguments
FORCE=0
for arg in "$@"; do
    case "$arg" in
        --force|-f) FORCE=1 ;;
        -h|--help)  sed -n '3,30p' "$0" | sed 's/^# \{0,1\}//'; exit 0 ;;
        *) die "unknown option: $arg (try --help)" ;;
    esac
done

# ---------------------------------------------------------------- preflight
command -v "$CC"  >/dev/null 2>&1 || die "missing $CC (install gcc-14-aarch64-linux-gnu)"
command -v "$CXX" >/dev/null 2>&1 || die "missing $CXX (install g++-14-aarch64-linux-gnu)"
command -v make  >/dev/null 2>&1 || die "missing make"
command -v curl  >/dev/null 2>&1 || die "missing curl"

if [ "$FORCE" -eq 0 ] && [ -f "$STAMP" ] && [ -f "$PREFIX/lib/libSDL2.so" ]; then
    msg "SDK already built ($SDK); use --force to rebuild"
    cat "$STAMP"
    exit 0
fi

if [ "$FORCE" -eq 1 ]; then
    msg "removing build tree (keeping src/ tarballs)"
    rm -rf "$BLD" "$PREFIX" "$DEVLIB" "$STAMP"
fi

mkdir -p "$SRC" "$BLD" "$PREFIX"

# ---------------------------------------------------------------- tarballs
fetch() { # $1 = url, $2 = expected sha256, $3 = target file
    url=$1; sha=$2; file=$3
    if [ -f "$SRC/$file" ]; then
        if echo "$sha  $SRC/$file" | sha256sum -c - >/dev/null 2>&1; then
            msg "using cached $file"
            return 0
        fi
        warn "checksum mismatch on cached $file, re-downloading"
        rm -f "$SRC/$file"
    fi
    msg "downloading $file"
    curl -fsSL --retry 3 --max-time 600 -o "$SRC/$file.part" "$url" \
        || die "download failed: $url"
    echo "$sha  $SRC/$file.part" | sha256sum -c - >/dev/null 2>&1 \
        || { rm -f "$SRC/$file.part"; die "checksum mismatch: $file"; }
    mv "$SRC/$file.part" "$SRC/$file"
}

unpack() { # $1 = archive name, $2 = top-level dir name
    [ -d "$SRC/$2" ] && return 0
    msg "unpacking $1"
    case "$1" in
        *.tar.gz)  tar xzf "$SRC/$1" -C "$SRC" ;;
        *.tar.bz2) tar xjf "$SRC/$1" -C "$SRC" ;;
        *) die "unknown archive type: $1" ;;
    esac
}

fetch "$SDL2_URL"     "$SDL2_SHA"     "SDL2-$SDL2_VER.tar.gz"
fetch "$SDL2_TTF_URL" "$SDL2_TTF_SHA" "SDL2_ttf-$SDL2_TTF_VER.tar.gz"
fetch "$ALSA_URL"     "$ALSA_SHA"     "alsa-lib-$ALSA_VER.tar.bz2"

unpack "SDL2-$SDL2_VER.tar.gz"       "SDL2-$SDL2_VER"
unpack "SDL2_ttf-$SDL2_TTF_VER.tar.gz" "SDL2_ttf-$SDL2_TTF_VER"
unpack "alsa-lib-$ALSA_VER.tar.bz2"  "alsa-lib-$ALSA_VER"

# ---------------------------------------------------------------- device glibc
# Import the device's libc/libm/ld.so so every library built below is linked
# against glibc 2.40 instead of the host toolchain's 2.43. Cached, so the
# device only has to be attached the first time (or after --force).
import_device_libs() {
    if [ -s "$DEVLIB/libc.so" ] && [ -f "$DEVLIB/libm.so.6" ]; then
        msg "using cached device glibc ($DEVLIB)"
        return 0
    fi
    command -v adb >/dev/null 2>&1 \
        || die "adb not found: it is needed once to import the device glibc into $DEVLIB"
    adb get-state >/dev/null 2>&1 \
        || die "no adb device: attach the RG35XX SP (needed once to import its glibc)"

    mkdir -p "$DEVLIB"
    for lib in libc.so.6 libm.so.6 libpthread.so.0 libdl.so.2 librt.so.1 \
               libgcc_s.so.1 ld-linux-aarch64.so.1; do
        dev_path=$(adb shell "readlink -f /lib/$lib 2>/dev/null || readlink -f /lib64/$lib 2>/dev/null" | tr -d '\r')
        [ -n "$dev_path" ] || die "device is missing /lib/$lib"
        msg "importing $lib"
        adb pull "$dev_path" "$DEVLIB/$lib" >/dev/null 2>&1 \
            || die "adb pull failed: $dev_path"
    done

    # ld resolves -lm / -lpthread / ... through these names.
    for f in "$DEVLIB"/*.so.*; do
        base=$(basename "$f")
        case "$base" in ld-linux*) continue ;; esac
        ln -sf "$base" "$DEVLIB/${base%%.so.*}.so"
    done

    # glibc's `atexit`/`at_quick_exit`/`pthread_atfork` live in
    # libc_nonshared.a, which Buildroot does not ship on the device. The host
    # toolchain's copy only wraps __cxa_atexit, so it links fine against the
    # device's 2.40 runtime.
    nonshared=$($CC -print-file-name=libc_nonshared.a)
    [ -f "$nonshared" ] || die "libc_nonshared.a not found in the cross toolchain"
    cp "$nonshared" "$DEVLIB/libc_nonshared.a"

    # libc.so is a linker script (not a symlink!): aarch64 defines
    # __stack_chk_guard in ld.so, which AS_NEEDED pulls in like the stock one.
    rm -f "$DEVLIB/libc.so"
    cat > "$DEVLIB/libc.so" <<EOF
/* GNU ld script: link against the RG35XX SP system C library (glibc 2.40,
   Buildroot) plus the libc_nonshared wrappers from the cross toolchain.
   Generated by platform/rg35xxsp/build_sdk.sh. */
OUTPUT_FORMAT(elf64-littleaarch64)
GROUP ( $DEVLIB/libc.so.6 $DEVLIB/libc_nonshared.a AS_NEEDED ( $DEVLIB/ld-linux-aarch64.so.1 ) )
EOF
}

import_device_libs

# pkg-config must not pick up host (x86_64) .pc files while cross configuring,
# and every SDK library must link against the device glibc, not the host one.
export PKG_CONFIG=false
export CC CXX
export LDFLAGS="-L$DEVLIB -Wl,-rpath-link,$DEVLIB"

# ---------------------------------------------------------------- SDL2
# Only the ABI matters: picoloop links against rg35xxsp-sdk/usr/lib/libSDL2.so
# but runs against the device's own libSDL2-2.0.so.0, so optional backends are
# disabled to keep the build self-contained and dependency free.
if [ ! -f "$BLD/SDL2/Makefile" ]; then
    msg "configuring SDL2 $SDL2_VER"
    mkdir -p "$BLD/SDL2"
    cd "$BLD/SDL2"
    "$SRC/SDL2-$SDL2_VER/configure" \
        --host="$CROSS" --build="$BUILD_TRIPLET" --prefix="$PREFIX" \
        --enable-shared --disable-static \
        --disable-video-x11 --disable-video-wayland --disable-video-kmsdrm \
        --disable-video-vivante --disable-video-vulkan --disable-video-metal \
        --disable-video-rpi --disable-video-directfb \
        --disable-libudev --disable-dbus --disable-ime --disable-ibus \
        --disable-fcitx --disable-hidapi \
        --disable-alsa --disable-pulseaudio --disable-jack --disable-pipewire \
        --disable-sndio --disable-nas --disable-esd --disable-libsamplerate \
        >configure.log 2>&1 \
        || { tail -30 configure.log >&2; die "SDL2 configure failed"; }
else
    cd "$BLD/SDL2"
fi
msg "building SDL2"
make -j"$JOBS" >build.log 2>&1 || { tail -30 build.log >&2; die "SDL2 build failed"; }
make install >>build.log 2>&1 || { tail -30 build.log >&2; die "SDL2 install failed"; }

# ---------------------------------------------------------------- SDL2_ttf
# Bundled FreeType (external/freetype in the release tarball) keeps the SDK
# independent of the device's freetype/harfbuzz/png headers; HarfBuzz is off.
if [ ! -f "$BLD/SDL2_ttf/Makefile" ]; then
    msg "configuring SDL2_ttf $SDL2_TTF_VER"
    mkdir -p "$BLD/SDL2_ttf"
    cd "$BLD/SDL2_ttf"
    SDL2_CONFIG="$PREFIX/bin/sdl2-config" \
    "$SRC/SDL2_ttf-$SDL2_TTF_VER/configure" \
        --host="$CROSS" --build="$BUILD_TRIPLET" --prefix="$PREFIX" \
        --enable-shared --disable-static \
        --enable-freetype-builtin --disable-harfbuzz --disable-freetypetest \
        --with-sdl-prefix="$PREFIX" \
        >configure.log 2>&1 \
        || { tail -30 configure.log >&2; die "SDL2_ttf configure failed"; }
else
    cd "$BLD/SDL2_ttf"
fi
msg "building SDL2_ttf"
make -j"$JOBS" >build.log 2>&1 || { tail -30 build.log >&2; die "SDL2_ttf build failed"; }
make install >>build.log 2>&1 || { tail -30 build.log >&2; die "SDL2_ttf install failed"; }

# ---------------------------------------------------------------- alsa-lib
# RtAudio's ALSA backend calls snd_pcm_* directly, so -lasound is needed at
# link time. The device provides libasound.so.2 at runtime.
if [ ! -f "$BLD/alsa-lib/Makefile" ]; then
    msg "configuring alsa-lib $ALSA_VER"
    mkdir -p "$BLD/alsa-lib"
    cd "$BLD/alsa-lib"
    "$SRC/alsa-lib-$ALSA_VER/configure" \
        --host="$CROSS" --build="$BUILD_TRIPLET" --prefix="$PREFIX" \
        --enable-shared --disable-static \
        >configure.log 2>&1 \
        || { tail -30 configure.log >&2; die "alsa-lib configure failed"; }
else
    cd "$BLD/alsa-lib"
fi
msg "building alsa-lib"
make -j"$JOBS" >build.log 2>&1 || { tail -30 build.log >&2; die "alsa-lib build failed"; }
make install >>build.log 2>&1 || { tail -30 build.log >&2; die "alsa-lib install failed"; }

# ---------------------------------------------------------------- verify
for probe in "$PREFIX/include/SDL2/SDL.h" \
             "$PREFIX/include/SDL2/SDL_ttf.h" \
             "$PREFIX/include/alsa/asoundlib.h" \
             "$PREFIX/lib/libSDL2.so" \
             "$PREFIX/lib/libSDL2_ttf.so" \
             "$PREFIX/lib/libasound.so" \
             "$DEVLIB/libc.so" \
             "$DEVLIB/libm.so.6"; do
    [ -e "$probe" ] || die "SDK incomplete, missing: $probe"
done

# ---------------------------------------------------------------- cleanup
# Keep the SDK small: the object trees (and the unpacked sources) are ~200 MB
# and are recreated from the cached tarballs by the next --force rebuild.
rm -rf "$BLD" "$SRC/SDL2-$SDL2_VER" "$SRC/SDL2_ttf-$SDL2_TTF_VER" \
       "$SRC/alsa-lib-$ALSA_VER"

cat > "$STAMP" <<EOF
SDL2=$SDL2_VER
SDL2_ttf=$SDL2_TTF_VER
alsa-lib=$ALSA_VER
target=$($CC -dumpmachine)
built=$(date -u +%Y-%m-%dT%H:%M:%SZ)
EOF

msg "SDK ready in $SDK"
cat "$STAMP"
echo "--- libraries ---"
ls -1 "$PREFIX"/lib/*.so* 2>/dev/null | sed "s|$SDK/||"
