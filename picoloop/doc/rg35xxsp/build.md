# Building, pushing and debugging picoloop on the RG35XX SP

The RG35XX SP (Anbernic, Allwinner H700) runs KNULLI, a Linux distribution.
There is no Windows/desktop toolchain on the device: you cross-compile on a
Linux PC and deploy the binary over **ADB**. This document covers the build
paths, the deploy loop and debugging.

Reference layout (what this tree uses):

    picoloop/picoloop/                       source (this repo)
    .../Makefile.PatternPlayer_rg35xxsp      cross build file
    .../platform/rg35xxsp/build_sdk.sh       generates the cross SDK (path A)
    .../doc/rg35xxsp/Dockerfile              container build env (path B)
    .../platform/rg35xxsp/picoloop.sh        deployable launcher
    .../rg35xxsp-sdk/                        generated SDK (gitignored)
    .../rg35xxsp/                            generated object tree (gitignored)

## Build paths at a glance

| | path | needs device | needs docker | disk | first build | SDL2 / ttf / alsa |
|---|---|---|---|---|---|---|
| **A** | generated SDK (`build_sdk.sh`), default | yes, once (glibc) | no | 43 MB | ~3 min | 2.32.8 / 2.24.0 / 1.2.13, built from pinned tarballs |
| **B** | **Docker container** (`doc/rg35xxsp/Dockerfile`) | **no** | yes | ~0.9 GB image | ~2 min | 2.30.0 / 2.22.0 / 1.2.11 (ubuntu 24.04) |
| **C** | KNULLI toolchain + staging sysroot | no | yes (KNULLI build env) | 20–35 GB | 45–90 min | exactly what the image ships, including KNULLI patches |
| **D** | pull headers/libs from the device | yes | no | ~50 MB | seconds | exactly the device's |

**The pragmatic way is B, the Docker container.** It is the only path that
needs neither the handheld nor a distro-specific toolchain, it pins the build
environment (pin the base image by digest), and it is cheap: one image, no
generated SDK, no device glibc. It is validated at build/link level (see
section 8.1) but is **not wired into the makefile yet** - path A stays the
default until it is.

Paths C and D are documented in section 8 for the cases where exactness
matters (a crash inside KNULLI's own SDL build) or where Docker is unwanted.

## 1. Host prerequisites

### Cross compiler
Ubuntu packages: `gcc-14-aarch64-linux-gnu` (provides
`aarch64-linux-gnu-gcc-14` / `aarch64-linux-gnu-g++-14`). Any aarch64
g++/gcc works; this project builds with gcc-14.

### Everything else is generated (path A)
SDL2, SDL2_ttf and alsa-lib are **not** taken from the host and not pulled from
the device: `platform/rg35xxsp/build_sdk.sh` downloads pinned upstream
tarballs, cross-compiles them and installs everything into `rg35xxsp-sdk/`
(gitignored). Required host tools: `make`, `curl`, `tar`, `sha256sum`.

The versions are pinned to the libraries the device image ships, so the headers
the port compiles against match the runtime it links into:

| library | version | device file |
|---|---|---|
| SDL2 | 2.32.8 | `/usr/lib/libSDL2-2.0.so.0.3200.8` |
| SDL2_ttf | 2.24.0 | `/usr/lib/libSDL2_ttf-2.0.so.0.2400.0` |
| alsa-lib | 1.2.13 | `/usr/lib/libasound.so.2` (RtAudio ALSA backend) |

Check the device with:

    adb shell "ls -l /usr/lib/libSDL2*.so.0 /usr/lib/libasound.so.2"

To move to a newer device image, bump `SDL2_VER` / `SDL2_TTF_VER` (and the
`*_SHA` checksums) at the top of `build_sdk.sh`, then rebuild with `--force`.

### Device glibc (path A, needed once)
The device runs glibc **2.40** (Buildroot) while the host cross toolchain ships
its own, newer glibc (2.43 on the machine used here). Linking the port against
the host glibc produces a binary that dies at startup with:

    ./picoloop: /lib64/libm.so.6: version `GLIBC_2.43' not found

`build_sdk.sh` therefore imports the device's own `libc.so.6`, `libm.so.6`,
`libpthread.so.0`, `libdl.so.2`, `librt.so.1`, `libgcc_s.so.1` and
`ld-linux-aarch64.so.1` into `rg35xxsp-sdk/device-lib/` (over adb, cached) and
links **everything** - SDL2, SDL2_ttf, alsa-lib and picoloop - against them.
The device only has to be attached for that first import (or after `--force`).

Path B avoids this entirely by using a container whose glibc is older than the
device's (section 8.1).

Whatever the path, the final binary must not require a glibc symbol newer than
the device's:

    aarch64-linux-gnu-objdump -T picoloop_rg35xxsp \
        | grep -o 'GLIBC_2\.[0-9]*' | sort -V -u | tail -1

### ADB
`adb` from `android-tools-adb`. This is the primary way to talk to the
device (see section 4).

## 2. Build

    cd picoloop/picoloop
    make -j12 -f Makefile.PatternPlayer_rg35xxsp all     # -> picoloop_rg35xxsp

The first run builds the SDK automatically (`rg35xxsp-sdk/`), which takes a few
minutes; later builds only rebuild picoloop. Useful targets:

    make -f Makefile.PatternPlayer_rg35xxsp sdk           # SDK only
    platform/rg35xxsp/build_sdk.sh --force                # rebuild the SDK
    make -f Makefile.PatternPlayer_rg35xxsp clean         # objects + binary only

`rm -rf rg35xxsp` (or `make ... clean`) forces a full rebuild. Always do a
clean rebuild after changing CFLAGS (make does not track flag changes).

The Makefile defines: `__SDL20__ PC_DESKTOP RG35XXSP __RTAUDIO__
__RTMIDI__ __LINUX_ALSA__` plus `-include cstdio -include cstdlib -include
cstring -include string -include cmath` (missing includes on newer GCC) and
`-static-libstdc++ -static-libgcc` (the device's libstdc++ only exports
CXXABI_1.3.14; gcc-14 emits CXXABI_1.3.15).

Nothing from the SDK is copied to the device: the binary records the normal
sonames (`libSDL2-2.0.so.0`, `libSDL2_ttf-2.0.so.0`, `libasound.so.2`) and no
rpath, so the device's own `/usr/lib` copies are used at runtime.

## 3. Deploy (push)

The port lives at `/userdata/roms/ports/picoloop/` on the device
(EmulationStation runs `picoloop.sh` from there). Push the binary and the
launcher:

    adb push picoloop_rg35xxsp /userdata/roms/ports/picoloop/picoloop
    adb shell chmod +x /userdata/roms/ports/picoloop/picoloop

    adb push platform/rg35xxsp/picoloop.sh /userdata/roms/ports/picoloop/picoloop.sh
    adb shell chmod +x /userdata/roms/ports/picoloop/picoloop.sh

or `make -f Makefile.PatternPlayer_rg35xxsp push` for the binary only. The copy
already on the device may carry local edits (e.g. `DEBUG_GDB=1`), so pushing
the launcher overwrites those.

Launch it from EmulationStation: Ports -> picoloop. Starting it from an `adb
shell` while EmulationStation is running does not work - ES keeps the display,
and with `SDL_VIDEODRIVER=dummy` the window surface is NULL so the app crashes
in `SDL_GUI::clearScreen()`.

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

    cd picoloop/picoloop
    make -j12 -f Makefile.PatternPlayer_rg35xxsp all
    adb push picoloop_rg35xxsp /userdata/roms/ports/picoloop/picoloop

If a source file was deleted/renamed or flags changed: `rm -rf rg35xxsp`
before rebuilding to avoid stale objects.

## 7. Shipping and reproducibility

The binary is shippable as-is: it only requires `libSDL2-2.0.so.0`,
`libSDL2_ttf-2.0.so.0`, `libasound.so.2` and a libc at least as new as the one
it was linked against - all provided by any KNULLI image. What is *not*
available is a self-contained binary (static glibc/SDL is not attempted).

Consequences:

- To distribute to other people's images, build against the **oldest** glibc
  you intend to support, not the newest that happens to work. `ubuntu:24.04`
  (2.39) is fine for current KNULLI images; `debian:12` (2.36) is the safe
  choice if older images are in scope.
- "Reproducible" here means pinned inputs -> same behaviour, not bit-identical
  output (that would additionally need `SOURCE_DATE_EPOCH`,
  `-ffile-prefix-map`, no `__DATE__`, ...).
- Pinning options, cheapest first: a digest-pinned Docker image (path B), a
  Nix flake (`pkgsCross.aarch64-multiplatform`), a chroot from
  snapshot.debian.org, or the KNULLI sysroot (path C, exact by git revision).
- The canonical answer for distribution is none of these: a Buildroot package
  (`package/picoloop/Config.in` + `picoloop.mk`) or a PortMaster port, so the
  image's own toolchain builds it and shipping/reproducibility stop being two
  problems.

## 8. Alternative build paths

### 8.1 Path B - Docker container (pragmatic)

`doc/rg35xxsp/Dockerfile` builds a small cross environment:

| | version | appears in device |
|---|---|---|
| base | `ubuntu:24.04` | - |
| glibc | 2.39 | 2.40 |
| SDL2 | 2.30.0 | 2.32.8 |
| SDL2_ttf | 2.22.0 | 2.24.0 |
| alsa-lib | 1.2.11 | device's own |

Compiling against *older* headers/libraries is the safe direction: every
symbol the binary references exists on the device (the reverse is what fails
with `GLIBC_2.4x not found`). Nothing is pulled from the handheld: no headers,
no `.so`, no glibc.

    docker build -t picoloop-sp -f doc/rg35xxsp/Dockerfile doc/rg35xxsp

    docker run --rm -u "$(id -u):$(id -g)" -v "$PWD":/src -w /src picoloop-sp \
        make -f Makefile.PatternPlayer_rg35xxsp \
             CC=aarch64-linux-gnu-g++ \
             SDK=/nonexistent SDK_STAMP=/bin/sh \
             OBJDIR=rg35xxsp_docker EXECUTABLE=picoloop_sp_docker \
             -j"$(nproc)"

Notes:
- `SDK=/nonexistent SDK_STAMP=/bin/sh` neutralises the SDK variables of the
  makefile (missing `-I`/`-L` directories are ignored by compiler/linker, and
  `/bin/sh` exists so the SDK build rule is not triggered). A separate
  `OBJDIR` keeps objects built with different headers apart.
- With rootless **podman** use `--userns=keep-id` instead of `-u`, and note
  that the base image is fully qualified (`docker.io/library/ubuntu:24.04`)
  because podman does not expand short names.
- The Dockerfile installs the arm64 runtime libraries normally but extracts
  the `-dev` `.deb`s with `dpkg-deb -x` instead of installing them: Ubuntu's
  `libsdl2-dev:arm64` hard-depends on `libibus-1.0-dev` -> `libglib2.0-dev`
  -> `python3`, and configuring `python3.12-minimal:arm64` fails with
  `/usr/bin/python3.12: Exec format error` unless qemu/binfmt is registered.
  Only headers and the `.so` symlinks are wanted, and unpacking runs no
  postinst.
- Validated end-to-end on 2026-10-24: image builds (podman 5.7.0), picoloop
  compiles and links, result is `ELF 64-bit ARM aarch64`, no rpath, `NEEDED` =
  `libSDL2_ttf-2.0.so.0`, `libSDL2-2.0.so.0`, `libasound.so.2`, `libm.so.6`,
  `libc.so.6`, `ld-linux-aarch64.so.1`, max symbol `GLIBC_2.38`, and the binary
  starts and runs when launched from the Ports menu on the device.
- Caveat: this cannot reproduce a bug inside KNULLI's **patched** SDL 2.32.8;
  for that use path C.

### 8.2 Path C - KNULLI toolchain and staging sysroot (exact)

Build only the parts of KNULLI you need, from `knulli-cfw/knulli-linux`
checked out at the revision matching the installed image:

    git clone --recurse-submodules https://github.com/knulli-cfw/knulli-linux
    make build-docker-image              # or let it pull knulli/knulli-build
    make h700-build CMD=toolchain        # toolchain + glibc 2.40 staging
    make h700-pkg PKG=sdl2
    make h700-pkg PKG=sdl2_ttf
    make h700-pkg PKG=libasound2

Then compile with
`output/h700/host/bin/aarch64-buildroot-linux-gnu-g++ --sysroot=output/h700/staging`.
This gives the image's own headers, libraries, libc, `libc.so` ld script and
`libc_nonshared.a`, so none of the hacks of path A are needed.

Costs and traps:
- ~20-35 GB and 45-90 min for toolchain + those packages; a **full** image is
  120-180 GB and 4-10 h (`make h700-build`), which is not needed.
- `make h700-toolchain` also builds LLVM (adds ~30-60 min); `CMD=toolchain`
  avoids that.
- `%-toolchain` snapshots with btrfs and needs `sudo`; on a non-btrfs
  filesystem use `CMD=toolchain` or `DIRECT_BUILD=1`.
- The revision must match the installed image, otherwise the sysroot no
  longer corresponds to what the device runs.

### 8.3 Path D - pull headers and libraries from the device

The quickest possible setup: pull the runtime `.so` files (following `NEEDED`
recursively: SDL2, SDL2_ttf, freetype, harfbuzz, glib, pcre2, png, bz2, z,
asound), create the unversioned symlinks, and import the device glibc exactly
as path A does. It links fine and needs no compilation of dependencies.

It was the first approach used for this port and was dropped because nothing
is pinned: "whatever this unit has today" is not a version, and a stale
snapshot was what made the previous build rot (headers from a wiped
`/tmp/opencode/SDL-release-2.0.22`, sysroot from a path that no longer
existed, while the device actually ran 2.32.8). If this path is revived, keep
a `MANIFEST` (md5 of every file, `uname -a`/image id, resolved soname
versions) so a build can be traced back to an image.
