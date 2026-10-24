#!/bin/sh
# Fetch and lay out the MinGW SDL2 + SDL2_ttf development files used by
# Makefile.PatternPlayer_windows_mingw_SDL2_{32,64}bit.
#
# The tree ships SDL 1.2 (SDL_windows/) but no SDL2, and vendoring another set
# of DLLs/import libraries is what improvement_plan.md step 7 wants to remove.
# So this script downloads the official MinGW packages instead, verifies them
# and unpacks only the headers/libraries/DLLs into a gitignored directory:
#
#     SDL2_windows/
#         include/SDL2/...        (shared by both arches)
#         lib32/libSDL2*.a        (i686)
#         lib64/libSDL2*.a        (x86_64)
#         bin32/SDL2*.dll
#         bin64/SDL2*.dll
#
# Usage (from the source directory):
#     platform/windows/fetch_sdl2.sh            # into ./SDL2_windows
#     SDL2_DIR=/somewhere platform/windows/fetch_sdl2.sh
#
# Versions and checksums are pinned; bump them together with the two makefiles.
set -eu

SDL2_VERSION=2.32.10
SDLTTF_VERSION=2.24.0

SDL2_SHA256=83a5d74012311edc3c0d40ea6faecbe57ad692aa033fa5dc273cc937e3938ff2
SDLTTF_SHA256=3a09e0a967ad53eca3ff2701de4d0df3369c0368739a1088a78d9d74908ef2c3

SDL2_URL=https://github.com/libsdl-org/SDL/releases/download/release-${SDL2_VERSION}/SDL2-devel-${SDL2_VERSION}-mingw.tar.gz
SDLTTF_URL=https://github.com/libsdl-org/SDL_ttf/releases/download/release-${SDLTTF_VERSION}/SDL2_ttf-devel-${SDLTTF_VERSION}-mingw.tar.gz

DEST=${SDL2_DIR:-SDL2_windows}
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT INT TERM

fetch() {
    url=$1; out=$2; sum=$3
    if [ -f "$out" ]; then
        echo "cached   $out"
    else
        echo "fetching $url"
        curl -fsSL --retry 3 -o "$out" "$url"
    fi
    echo "$sum  $out" | sha256sum -c - >/dev/null
    echo "verified $out"
}

fetch "$SDL2_URL"   "$TMP/sdl2.tar.gz"    "$SDL2_SHA256"
fetch "$SDLTTF_URL" "$TMP/sdl2ttf.tar.gz" "$SDLTTF_SHA256"

rm -rf "$DEST"
mkdir -p "$DEST/include" "$DEST/lib32" "$DEST/lib64" "$DEST/bin32" "$DEST/bin64"

tar xzf "$TMP/sdl2.tar.gz"    -C "$TMP"
tar xzf "$TMP/sdl2ttf.tar.gz" -C "$TMP"

for arch in i686 x86_64; do
    case $arch in
        i686)   suffix=32 ;;
        x86_64) suffix=64 ;;
    esac

    # Headers are identical between arches; keep one copy.
    if [ ! -d "$DEST/include/SDL2" ]; then
        cp -R "$TMP/SDL2-${SDL2_VERSION}/$arch-w64-mingw32/include/SDL2" "$DEST/include/"
        cp -R "$TMP/SDL2_ttf-${SDLTTF_VERSION}/$arch-w64-mingw32/include/SDL2/"*.h "$DEST/include/SDL2/"
    fi

    cp "$TMP/SDL2-${SDL2_VERSION}/$arch-w64-mingw32/lib/"*.a          "$DEST/lib$suffix/"
    cp "$TMP/SDL2_ttf-${SDLTTF_VERSION}/$arch-w64-mingw32/lib/"*.a    "$DEST/lib$suffix/"
    cp "$TMP/SDL2-${SDL2_VERSION}/$arch-w64-mingw32/bin/"*.dll        "$DEST/bin$suffix/"
    cp "$TMP/SDL2_ttf-${SDLTTF_VERSION}/$arch-w64-mingw32/bin/"*.dll  "$DEST/bin$suffix/"
done

echo
echo "$DEST ready:"
echo "  SDL2 $SDL2_VERSION, SDL2_ttf $SDLTTF_VERSION"
echo "  lib32: $(ls "$DEST/lib32" | wc -l) files, lib64: $(ls "$DEST/lib64" | wc -l) files"
echo "  SDK_ttf's own freetype deps are inside libSDL2_ttf.a (static)."
