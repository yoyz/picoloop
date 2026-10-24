# Building, pushing and debugging picoloop on the RG35XX SP

The RG35XX SP (Anbernic, Allwinner H700) runs KNULLI, a Linux distribution.
There is no Windows/desktop toolchain on the device: you cross-compile on a
Linux PC and deploy the binary over **ADB**. This document covers the build
paths, the deploy loop and debugging.

Reference layout (what this tree uses):

    picoloop/picoloop/                       source (this repo)
    .../Makefile.PatternPlayer_rg35xxsp      build file (both modes)
    .../doc/rg35xxsp/Dockerfile              container build env (default)
    .../platform/rg35xxsp/build_sdk.sh       device-matched SDK (opt-in)
    .../platform/rg35xxsp/picoloop.sh        deployable launcher
    .../rg35xxsp-sdk/                        generated SDK (gitignored)
    .../rg35xxsp/                            objects, container mode (gitignored)
    .../rg35xxsp-device/                     objects, device-libs mode (gitignored)

## Build paths at a glance

| | path | needs device | needs docker | disk | first build | SDL2 / ttf / alsa |
|---|---|---|---|---|---|---|
| **B** | **Docker container - DEFAULT** (`doc/rg35xxsp/Dockerfile`) | **no** | yes | ~0.9 GB image | ~2 min | 2.30.0 / 2.22.0 / 1.2.11 (ubuntu 24.04) |
| **A** | generated SDK (`build_sdk.sh`) - **opt-in**, device-matched | yes, once (glibc) | no | 43 MB | ~3 min | 2.32.8 / 2.24.0 / 1.2.13, built from pinned tarballs, + the device's own glibc |
| C | KNULLI toolchain + staging sysroot | no | yes (KNULLI build env) | 20-35 GB | 45-90 min | exactly what the image ships, including KNULLI patches |
| D | pull headers/libs from the device | yes | no | ~50 MB | seconds | exactly the device's |

**Path B is the normal way to build.** It needs neither the handheld nor a
generated SDK, and it pins the build environment (pin the base image by digest).

**Path A is not used by default.** Keep it for the case where a build has to
be linked against the same libraries the device actually runs - typically when
a crash may come from the device's SDL or from a glibc difference, and you want
to remove that variable. It builds SDL2/SDL2_ttf/alsa-lib at the device's
versions from pinned upstream tarballs and links everything against the
device's own glibc (imported over adb). Same versions as the device plus the
device glibc - not a byte-identical copy of KNULLI's patched builds.

Paths C and D are documented in section 8.

## 1. Default build: Docker container (path B)

Prerequisite: `docker` or `podman`. No cross compiler, no SDL dev packages and
no device are needed on the host.

    docker build -t picoloop-sp -f doc/rg35xxsp/Dockerfile doc/rg35xxsp

    docker run --rm -u "$(id -u):$(id -g)" -v "$PWD":/src -w /src picoloop-sp \
        make -f Makefile.PatternPlayer_rg35xxsp -j"$(nproc)"

With rootless podman, replace `-u "$(id -u):$(id -g)"` with `--userns=keep-id`
(and note the base image is fully qualified, `docker.io/library/ubuntu:24.04`,
because podman does not expand short names):

    podman build -t picoloop-sp -f doc/rg35xxsp/Dockerfile doc/rg35xxsp
    podman run --rm --userns=keep-id -v "$PWD":/src:z -w /src picoloop-sp \
        make -f Makefile.PatternPlayer_rg35xxsp -j"$(nproc)"

Result: `picoloop_rg35xxsp` in the source tree, built by the container's
`aarch64-linux-gnu-g++` against its arm64 multiarch dev packages. The build is
deterministic: the same image digest and source produce the same binary.

What the image provides, and why that is safe:

| | image (ubuntu 24.04) | device |
|---|---|---|
| glibc | 2.39 | 2.40 |
| SDL2 | 2.30.0 | 2.32.8 |
| SDL2_ttf | 2.22.0 | 2.24.0 |
| alsa-lib | 1.2.11 | device's own |

Compiling against *older* headers/libraries is the safe direction: every symbol
the binary references exists on the device. The reverse fails at startup with
`GLIBC_2.4x not found`, which is what happens when the host's own cross
toolchain (glibc 2.43 on the development machine) is used directly.

Caveat: this cannot reproduce a bug that lives inside KNULLI's **patched**
SDL 2.32.8 or in the device glibc. For that, use path A (device-matched
library versions + device glibc) or path C (the image's exact sysroot).

Implementation notes on the Dockerfile:

- Ubuntu's arm64 `libsdl2-dev` hard-depends on `libibus-1.0-dev` ->
  `libglib2.0-dev` -> `python3`, so a plain multiarch install tries to
  configure `python3.12-minimal:arm64` and fails with
  `/usr/bin/python3.12: Exec format error` unless qemu/binfmt is registered.
  The image therefore installs the arm64 *runtime* libraries normally and
  unpacks the `-dev` `.debs` with `dpkg-deb -x` (headers + `.so` symlinks, no
  postinst).
- Do not move the base to a newer release without checking its glibc: Debian 13
  and Ubuntu 25.10 already ship glibc >= 2.41 and reintroduce the problem.
  Debian 12 (glibc 2.36) is the safest base if binaries are distributed to
  older KNULLI images.

## 2. Device-matched build: generated SDK (path A, opt-in)

Use this when the binary must link against the same library versions the
handheld runs, e.g. to rule out "the device's SDL 2.32.8 behaves differently
from the container's 2.30.0" while chasing a crash.

`platform/rg35xxsp/build_sdk.sh` downloads pinned upstream tarballs, builds
**SDL2 2.32.8**, **SDL2_ttf 2.24.0** (bundled FreeType, HarfBuzz off) and
**alsa-lib 1.2.13**, and imports the **device's own glibc** into
`rg35xxsp-sdk/` (gitignored). Versions and checksums are pinned at the top of
the script and must match the device image:

| library | version | device file |
|---|---|---|
| SDL2 | 2.32.8 | `/usr/lib/libSDL2-2.0.so.0.3200.8` |
| SDL2_ttf | 2.24.0 | `/usr/lib/libSDL2_ttf-2.0.so.0.2400.0` |
| alsa-lib | 1.2.13 | `/usr/lib/libasound.so.2` (RtAudio ALSA backend) |

Check the device with:

    adb shell "ls -l /usr/lib/libSDL2*.so.0 /usr/lib/libasound.so.2"

Build the SDK (needs `aarch64-linux-gnu-gcc-14`/`g++-14`, `curl`, `make`, and
the device attached once for the glibc import; tarballs are cached so later
rebuilds work offline):

    platform/rg35xxsp/build_sdk.sh            # ~3 min, cached
    platform/rg35xxsp/build_sdk.sh --force    # rebuild (keeps the tarballs)

Then build with the device libraries. This is a host build, not a container
build, and uses its own object tree (`rg35xxsp-device/`) so the two modes never
share objects:

    make -f Makefile.PatternPlayer_rg35xxsp \
         USE_DEVICE_LIBS=1 CC=aarch64-linux-gnu-g++-14 -j"$(nproc)"

Why the glibc import exists: the device runs glibc **2.40** (Buildroot) while
the host cross toolchain ships its own, newer glibc (2.43 here). Linking
against the host glibc produces a binary that dies at startup with

    ./picoloop: /lib64/libm.so.6: version `GLIBC_2.43' not found

so the script pulls the device's `libc.so.6`, `libm.so.6`, `libpthread.so.0`,
`libdl.so.2`, `librt.so.1`, `libgcc_s.so.1` and `ld-linux-aarch64.so.1` into
`rg35xxsp-sdk/device-lib/` and links everything (SDL2, SDL2_ttf, alsa-lib and
picoloop) against them. `device-lib/libc.so` is a hand-written ld script that
groups the device `libc.so.6` with the toolchain's `libc_nonshared.a` (needed
for `atexit`, which Buildroot does not ship) and `AS_NEEDED(ld-linux-aarch64.so.1)`
(aarch64 keeps `__stack_chk_guard` in ld.so).

Whatever the path, check that the binary does not require a glibc symbol newer
than the device's:

    aarch64-linux-gnu-objdump -T picoloop_rg35xxsp \
        | grep -o 'GLIBC_2\.[0-9]*' | sort -V -u | tail -1

To move to a newer device image, bump `SDL2_VER` / `SDL2_TTF_VER` (and the
`*_SHA` checksums) at the top of `build_sdk.sh`, then rebuild with `--force`.

## 3. Deploy (push)

The port lives at `/userdata/roms/ports/picoloop/` on the device
(EmulationStation runs `picoloop.sh` from there). Push the binary, and the
launcher when it changed:

    adb push picoloop_rg35xxsp /userdata/roms/ports/picoloop/picoloop
    adb shell chmod +x /userdata/roms/ports/picoloop/picoloop

    adb push platform/rg35xxsp/picoloop.sh /userdata/roms/ports/picoloop/picoloop.sh
    adb shell chmod +x /userdata/roms/ports/picoloop/picoloop.sh

or `make -f Makefile.PatternPlayer_rg35xxsp push` (binary only; must run on the
host, adb is not in the container image). The launcher copy already on the
device may carry local edits, so pushing it overwrites those.

Launch from EmulationStation: Ports -> picoloop. Starting it from an `adb shell`
while EmulationStation is running does not work - ES keeps the display, and with
`SDL_VIDEODRIVER=dummy` the window surface is NULL so the app crashes in
`SDL_GUI::clearScreen()`.

## 4. ADB usage

The RG35XX SP enumerates as a fake Android device over USB:

    adb devices          # reports e.g. 0123456789 (Nexus_4/mako identity)
    adb shell            # root shell (bash-5.2#)

If you get "no permissions", fix the udev rule for the device's gadget
(`1d6b:0104`) so adb can open it, then `adb kill-server && adb start-server`.

ADB gives you a root shell, `adb push`/`adb pull` for files, and lets you
forward ports (e.g. `adb forward tcp:22222 tcp:22` for ssh).

## 5. Debugging

### In-app crash report (default)

The launcher runs the binary natively with stderr redirected to
`picoloop.err`, and the app installs a crash handler for SIGSEGV, SIGBUS,
SIGILL, SIGFPE and SIGABRT (see `PatternPlayer.cpp`, `install_crash_handlers`).
A crash leaves:

    *** picoloop crash: SIGSEGV (11), code 1 (SEGV_MAPERR), fault address 0x8, pc 0x...
    ./picoloop(+0x6f324) ...

Pull it with

    adb pull /userdata/roms/ports/picoloop/picoloop.err .

and resolve frames directly (the offsets are relative to the executable):

    aarch64-linux-gnu-addr2line -f -C -e picoloop_rg35xxsp 0x6f324

`picoloop.log` (DPRINTF, always enabled in this build) shows what happened
before the crash; it is truncated per run:

    adb pull /userdata/roms/ports/picoloop/picoloop.log .

### gdb (all threads, registers)

Set `DEBUG_GDB=1` in `picoloop.sh` and relaunch. The script runs the binary
under `gdb -batch` and dumps signal + PC, backtrace, all-thread backtraces and
registers to `gdb.log`:

    adb pull /userdata/roms/ports/picoloop/gdb.log .

gdb costs startup time, ~50-150 MB of memory and disables ASLR; it does not
slow the run itself (no single-stepping). Use it when the in-app report is not
enough, e.g. to see other threads.

Tips:
- Build with `-g` (add it to CFLAGS, rebuild) for file:line in gdb backtraces.
- `strace` and `gdb` are available on the device.

## 6. Rebuilding after a source change

Default (container):

    podman run --rm --userns=keep-id -v "$PWD":/src:z -w /src picoloop-sp \
        make -f Makefile.PatternPlayer_rg35xxsp -j"$(nproc)"
    adb push picoloop_rg35xxsp /userdata/roms/ports/picoloop/picoloop

Device-matched (path A): same with `USE_DEVICE_LIBS=1 CC=aarch64-linux-gnu-g++-14`.

If a source file was deleted/renamed or the flags changed, remove the object
tree first (`rm -rf rg35xxsp` or `rm -rf rg35xxsp-device`) - make does not track
flag changes.

## 7. Shipping and reproducibility

The binary is shippable as-is: it only requires `libSDL2-2.0.so.0`,
`libSDL2_ttf-2.0.so.0`, `libasound.so.2` and a libc at least as new as the one
it was linked against - all provided by any KNULLI image. A self-contained
binary (static glibc/SDL) is not attempted.

Consequences:

- To distribute to other people's images, build against the **oldest** glibc
  you intend to support, not the newest that happens to work. `ubuntu:24.04`
  (2.39) is fine for current KNULLI images; `debian:12` (2.36) is safer for
  older ones.
- "Reproducible" here means pinned inputs -> same behaviour (and, with path B,
  the same binary: the container build is deterministic). Bit-identical output
  does not need `SOURCE_DATE_EPOCH`/`-ffile-prefix-map` tricks at the moment.
- Pinning options: a digest-pinned container image (path B), a Nix flake
  (`pkgsCross.aarch64-multiplatform`), a chroot from snapshot.debian.org, or
  the KNULLI sysroot (path C, exact by git revision).
- The canonical answer for distribution is none of these: a Buildroot package
  (`package/picoloop/Config.in` + `picoloop.mk`) or a PortMaster port, so the
  image's own toolchain builds it.

## 8. Other paths

### 8.1 Path C - KNULLI toolchain and staging sysroot (exact)

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
`libc_nonshared.a`, so none of path A's glibc workarounds are needed.

Costs and traps:
- ~20-35 GB and 45-90 min for toolchain + those packages; a **full** image is
  120-180 GB and 4-10 h (`make h700-build`), which is not needed.
- `make h700-toolchain` also builds LLVM (adds ~30-60 min); `CMD=toolchain`
  avoids that.
- `%-toolchain` snapshots with btrfs and needs `sudo`; on a non-btrfs filesystem
  use `CMD=toolchain` or `DIRECT_BUILD=1`.
- The revision must match the installed image, otherwise the sysroot no longer
  corresponds to what the device runs.

### 8.2 Path D - pull headers and libraries from the device

The quickest possible setup: pull the runtime `.so` files (following `NEEDED`
recursively: SDL2, SDL2_ttf, freetype, harfbuzz, glib, pcre2, png, bz2, z,
asound), create the unversioned symlinks, and import the device glibc exactly
as path A does. It links fine and needs no compilation of dependencies.

It was the first approach used for this port and was dropped because nothing is
pinned: "whatever this unit has today" is not a version, and a stale snapshot
was what made an earlier build rot (headers from a wiped
`/tmp/opencode/SDL-release-2.0.22`, sysroot from a path that no longer existed,
while the device actually ran 2.32.8). If this path is revived, keep a
`MANIFEST` (md5 of every file, `uname -a`/image id, resolved soname versions)
so a build can be traced back to an image.
