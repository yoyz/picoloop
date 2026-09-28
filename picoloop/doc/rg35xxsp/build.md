# Building, pushing and debugging picoloop on the RG35XX SP

The RG35XX SP (Anbernic, Allwinner H700) runs KNULLI, a Linux distribution.
There is no Windows/desktop toolchain on the device: you cross-compile on a
Linux PC and deploy the binary over **ADB**. This document covers the whole
loop (prerequisites -> build -> push -> debug) using ADB only.

Reference layout (what this tree uses):

    /home/ollama/doc/rg35xxsp/picoloop/picoloop/   source (this repo)
    .../Makefile.PatternPlayer_rg35xxsp            cross build file
    .../platform/rg35xxsp/picoloop.sh              deployable launcher
    /home/ollama/doc/rg35xxsp/hello/               SDL_ttf header + device sysroot
    /home/ollama/doc/rg35xxsp/hello/sysroot/usr/lib  device .so files
    /tmp/opencode/SDL-release-2.0.22/include       upstream SDL2 headers

## 1. Host prerequisites

### Cross compiler
Ubuntu packages: `gcc-14-aarch64-linux-gnu` (provides
`aarch64-linux-gnu-gcc-14` / `aarch64-linux-gnu-g++-14`). Any aarch64
g++/gcc works; this project builds with gcc-14.

### SDL2 headers (upstream, do NOT use the Debian-split ones)
The Ubuntu `/usr/include/SDL2` (SDL_config.h -> SDL2/_real_SDL_config.h)
does **not** cross-compile. Use upstream `SDL-release-2.0.22`:

    curl -L https://github.com/libsdl-org/SDL/archive/refs/tags/release-2.0.22.tar.gz
    # headers land flat in include/ ; create include/SDL2 symlink -> . so
    # #include <SDL2/SDL.h> resolves.

### SDL_ttf header
Upstream `SDL_ttf-release-2.0.18/SDL_ttf.h` (matches the device's
`libSDL2_ttf-2.0.so.0.2400.0`). The Makefile points `TTF_INC` at a folder
containing `SDL_ttf.h`.

### Device shared libraries (the sysroot)
Pull the runtime libs from the device into a sysroot and create unversioned
symlinks for the linker:

    adb shell "ls /usr/lib/libSDL2* /usr/lib/libSDL2_ttf* /usr/lib/libfreetype* \
               /usr/lib/libharfbuzz* /usr/lib/libglib-2.0* /usr/lib/libpcre2-8* \
               /usr/lib/libpng16* /usr/lib/libbz2* /usr/lib/libz* /usr/lib/libasound*"
    # copy them to sysroot/usr/lib and symlink e.g. libSDL2.so -> libSDL2-2.0.so.0

The Makefile links with `-Wl,-rpath-link,<sysroot>/usr/lib` so transitive
deps resolve, then `-lSDL2_ttf -lSDL2 -lfreetype -lharfbuzz -lpng16 -lbz2
-lz -lasound -lpthread -lm`. Only `libSDL2_ttf`, `libSDL2`, `libc` and the
loader end up NEEDED by the final binary.

### ALSA headers
Host `/usr/include` provides `alsa/asoundlib.h` (RtAudio/RtMidi ALSA
backend); link against the device `libasound.so.2` from the sysroot.

### ADB
`adb` from `android-tools-adb`. This is the primary way to talk to the
device (see section 4).

### make
Any GNU make (>=3.81); build in parallel with `-j` (this host has 24 cores).

## 2. Build

    cd /home/ollama/doc/rg35xxsp/picoloop/picoloop
    make -j12 -f Makefile.PatternPlayer_rg35xxsp all     # -> picoloop_rg35xxsp

`make ... clean` (or `rm -rf rg35xxsp`) forces a full rebuild. Always do a
clean rebuild after changing CFLAGS (make does not track flag changes).

The Makefile defines: `__SDL20__ PC_DESKTOP RG35XXSP __RTAUDIO__
__RTMIDI__ __LINUX_ALSA__` plus `-include cstdio -include cstdlib -include
cstring -include string -include cmath` (missing includes on newer GCC) and
`-static-libstdc++ -static-libgcc` (the device's libstdc++ only exports
CXXABI_1.3.14; gcc-14 emits CXXABI_1.3.15).

## 3. Deploy (push)

The port lives at `/userdata/roms/ports/picoloop/` on the device
(EmulationStation runs `picoloop.sh` from there). Push the binary and the
launcher:

    adb push picoloop_rg35xxsp /userdata/roms/ports/picoloop/picoloop
    adb shell chmod +x /userdata/roms/ports/picoloop/picoloop

    adb push platform/rg35xxsp/picoloop.sh /userdata/roms/ports/picoloop/picoloop.sh
    adb shell chmod +x /userdata/roms/ports/picoloop/picoloop.sh

Launch it from EmulationStation: Ports -> picoloop.

## 4. ADB usage

The RG35XX SP enumerates as a fake Android device over USB:

    adb devices          # reports e.g. 0123456789 (Nexus_4/mako identity)
    adb shell            # root shell (bash-5.2#)

If you get "no permissions", fix the udev rule for the device's gadget
(`1d6b:0104`) so adb can open it, then `adb kill-server && adb start-server`.

ADB gives you a root shell, `adb push`/`adb pull` for files, and lets you
forward ports (e.g. `adb forward tcp:22222 tcp:22` for ssh), but for this
project plain `adb shell` + `adb pull` is all you need.

## 5. Debug with gdb

The launcher has a toggle. Edit `picoloop.sh` (the reference copy in
`platform/rg35xxsp/picoloop.sh`) and set:

    DEBUG_GDB=1

Then launch picoloop from EmulationStation. The script runs the binary under
`gdb -batch` and, on a crash, dumps the backtrace to
`/userdata/roms/ports/picoloop/gdb.log`. Grab it over adb:

    adb pull /userdata/roms/ports/picoloop/gdb.log .

The log contains: signal + crashing PC, backtrace of the current thread,
all-thread backtraces, and registers. `DEBUG_GDB=0` runs the binary normally.

Tips:
- Build with `-g` (add it to CFLAGS, `rm -rf rg35xxsp`, rebuild) for symbol
  names in the backtrace. Keep `-O2` so the crash reproduces identically.
- gdb on the device is `gdb`; `strace` is also available if needed.
- `picoloop.log` (written by the app via DPRINTF, always enabled in this
  build) is often the fastest way to see what happened before an abort
  (e.g. an `exit(1)` from a PatternElement default case). Pull it with
  `adb pull /userdata/roms/ports/picoloop/picoloop.log .`.

## 6. Rebuilding after a source change

    cd /home/ollama/doc/rg35xxsp/picoloop/picoloop
    make -j12 -f Makefile.PatternPlayer_rg35xxsp all
    adb push picoloop_rg35xxsp /userdata/roms/ports/picoloop/picoloop

If a source file was deleted/renamed or flags changed: `rm -rf rg35xxsp`
before rebuilding to avoid stale objects.