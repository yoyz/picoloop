# Building picoloop for the GCW Zero / OpenDingux

The GCW Zero runs OpenDingux: a 32 MB MIPS (Ingenic JZ4770) handheld that loads
`.dge` executables from gmenu2x. The only usable compiler is the **2014 GCW
Zero toolchain** (Buildroot 2014.05, GCC 4.9.1, `mipsel-gcw0-linux-uclibc`),
because the firmware's libc is uClibc 0.9.33.2 and its libraries are built with
that toolchain.

That toolchain's original download URL is dead, so this tree builds its own
container image around an archived, checksum-pinned copy - see section 1.1.

Reference layout:

    picoloop/picoloop/                          source (this repo)
    .../Makefile.PatternPlayer_opendingux_SDL   build file (SDL 1.2, SDL audio)
    .../doc/opendingux/Dockerfile               toolchain container
    .../opendingux_sdl/                         objects (gitignored)
    .../opendingux/PatternPlayer.dge            built artifact (gitignored)
    .../font.ttf                                font loaded at runtime
    .../doc/opendingux/build.md                 this file

## 1. Build the toolchain container

    podman build -t picoloop-od -f doc/opendingux/Dockerfile doc/opendingux

With docker, the same command with `docker build`. There is no published image:
the historical one (`podulator/OpenDinguxBuilder`) is broken today, see 1.1.

What the image is:

| | value |
|---|---|
| base | `docker.io/library/ubuntu:24.04` |
| extra arch | i386 runtime (`libc6:i386`, `libstdc++6:i386`, `libgcc-s1:i386`) - the 2014 toolchain binaries are **32-bit i386** |
| toolchain | Buildroot 2014.05, `gcc 4.9.1`, `mipsel-gcw0-linux-uclibc`, uClibc 0.9.33.2 |
| target libs in the sysroot | SDL 1.2 (`libSDL-1.2.so.0.11.4`), SDL_ttf 2.0.10.1, SDL_gfx 13.9.1, freetype, zlib |
| installed to | `/opt/opendingux-toolchain` (+ `/opt/gcw0-toolchain` symlink, which `Makefile.PatternPlayer_gcwzero_RtAudio` expects) |
| `PATH` / `CC` | toolchain `bin` dirs, `CC=mipsel-gcw0-linux-uclibc-gcc` |
| workdir | `/src` |

The image build performs the download and verifies it, so an image that builds
is an image with the right toolchain:

```
sha256  3632c85f48879108d4349570ded60d87d7a324850b81d683d031e4ee112baaa0
size    144 431 062 bytes
source  https://web.archive.org/web/20160310002829id_/http://www.gcw-zero.com/
        files/opendingux-gcw0-toolchain.2014-08-20.tar.bz2
```

`id_` asks the Wayback Machine for the original bytes; a capture is immutable,
so the pinned hash either matches or the build stops.

### 1.1 Why not the old `podulator/OpenDinguxBuilder` image?

That Dockerfile (`github.com/podulator/OpenDinguxBuilder`) is the historical
way this target was built, and it no longer works, for three independent
reasons:

- `FROM debian:stretch`: stretch is EOL and its apt repositories moved to
  `archive.debian.org`, so the first `apt-get update` returns 404.
- The toolchain URL `http://www.gcw-zero.com/files/opendingux-gcw0-toolchain.
  2014-08-20.tar.bz2` now 301-redirects to the site homepage and serves HTML;
  `wget` saves that as a `.tar.bz2` and `tar` fails.
- Nothing was ever published to a registry (`podulator` has no Docker Hub
  repositories), so there is no prebuilt image to fall back on.

What this tree does instead: modern base (`ubuntu:24.04`), the same i386
runtime the old image got from `gcc-multilib`, and the archived tarball with a
SHA-256 check. Same toolchain, reproducible build.

## 2. Build

    podman run --rm --userns=keep-id -v "$PWD":/src:z -w /src picoloop-od \
        make -f Makefile.PatternPlayer_opendingux_SDL -j"$(nproc)"

    cp PatternPlayer.dge opendingux/       # optional: keep the artifact

With docker, use `-u "$(id -u):$(id -g)"` instead of `--userns=keep-id`
(rootless podman also needs the `:z` SELinux label on the bind mount).

Build time is about 7 seconds on 8 cores (59 translation units - four engines).

### 2.1 Output

| file | size (verified build) | purpose |
|---|---:|---|
| `PatternPlayer.dge` | 3.1 MB (3 219 443 B) | the executable gmenu2x launches |

Verified properties:

```
ELF 32-bit LSB executable, MIPS, MIPS32 rel2, dynamically linked
interpreter: /lib/ld-uClibc.so.0
NEEDED: libSDL-1.2.so.0  libSDL_ttf-2.0.so.0  libSDL_gfx.so.13
        libpthread.so.0  libstdc++.so.6  libm.so.0  libgcc_s.so.1  libc.so.0
```

All of those are part of the OpenDingux firmware image on a GCW Zero, so the
`.dge` can be copied on its own. If a unit is missing, copy the matching `.so`
into the same directory as the `.dge`.

`opendingux/` and `opendingux_sdl/` are in `.gitignore`.

## 3. Deploy

On the GCW Zero (device storage mounted over USB, or an SD card):

    /media/data/apps/PatternPlayer.dge
    /media/data/apps/PatternPlayer.gcw0.desktop
    /media/data/apps/font.ttf

A minimal `/media/data/apps/PatternPlayer.gcw0.desktop`:

    [Desktop Entry]
    Name=Picoloop
    Exec=PatternPlayer.dge
    Icon=PatternPlayer.png
    Terminal=false
    Type=Application
    Categories=AudioVideo;
    Comment=Pattern-based groovebox

`font.ttf` is **required**: OpenDingux builds do not use the embedded assets
(those are `PC_DESKTOP`-only), so `openTTFFont()` opens `font.ttf` relative to
the working directory. Storage is relative too (`GETPICOLOOPUSERSTORAGE()` is
`"./"`), so `bank/` and `picoloop.log` are created next to the binary.

Logs: the build does not define `DEBUGPRINTF`, so `DPRINTF()` is compiled out
and no `picoloop.log` is written. Add `-DDEBUGPRINTF` to `CFLAGS` for a
diagnostic build.

The binary was **not executed** - there is no GCW Zero here and no usable
OpenDingux emulator. What is verified is that it compiles, links, and is a
well-formed MIPS/uClibc executable with the firmware's libraries.

## 4. What the makefile does

`Makefile.PatternPlayer_opendingux_SDL` follows the same shape as the other
`Makefile.PatternPlayer_*` files:

    SYNTH            four engines: PICOSYNTH, PICODRUM, OPL2, PBSYNTH
    CFLAGS           -std=c++11 -O3 -c, -D__SDL12__ -DOPENDINGUX -D__SDL_AUDIO__
    LDFLAGS          -L$(SYSROOT)/usr/lib -lSDL -lSDL_ttf -lSDL_gfx -lpthread
    SOURCES          AudioDriverSDL.cpp, SYSTEMLINUX.cpp + Makefile_sources groups
    EXECUTABLE       PatternPlayer.dge

Notes:

- **Four engines only.** 32 MB of RAM, a 600 MHz MIPS core, and each
  `MonoMixer` instantiates every enabled engine (four mixers). Cursynth,
  Open303, Twytch, MDADrum, SID, the LGPT sampler, MIDI out and ER-1 are not
  built here; the heavier ones also rely on newer C++ than GCC 4.9.1 handles
  comfortably.
- **`-std=c++11` works** on this toolchain: `std::mutex`, `std::atomic`,
  `std::thread`, `unique_ptr` and lambdas all compile and link (verified with a
  probe program before porting the makefile).
- **`std::to_string` / `std::stoi` do not exist** in this uClibc libstdc++
  (no `_GLIBCXX_USE_C99`). The current codebase uses none of them - keep it
  that way for this target.
- `-lSDL_gfx` is kept for parity with the historical target; the sources in this
  target do not call SDL_gfx functions, so it could be dropped.

## 5. Rebuilding and cleaning

    # incremental
    podman run --rm --userns=keep-id -v "$PWD":/src:z -w /src picoloop-od \
        make -f Makefile.PatternPlayer_opendingux_SDL -j"$(nproc)"

    # remove objects, dependency files and the .dge
    podman run --rm --userns=keep-id -v "$PWD":/src:z -w /src picoloop-od \
        make -f Makefile.PatternPlayer_opendingux_SDL clean

## 6. Troubleshooting

| symptom | cause | fix |
|---|---|---|
| `E: Failed to fetch .../stretch/... 404` | building the old `podulator` Dockerfile: `debian:stretch` is EOL | use `doc/opendingux/Dockerfile` (ubuntu 24.04) |
| `tar: This does not look like a tar archive` during image build | the gcw-zero.com URL now returns the site homepage | download the archived tarball with the pinned hash (already in the Dockerfile) |
| `exec format error` running `mipsel-gcw0-linux-uclibc-g++` | its host binaries are 32-bit i386 | i386 runtime in the image (`libc6:i386`, `libstdc++6:i386`, `libgcc-s1:i386`) |
| `make: not found` inside the container | the container ships the cross toolchain only | the Dockerfile installs `make` |
| `undefined reference to 'main'`, `SDL_OpenAudio`, `~PicosynthUserInterface()` while "compiling" a `.cpp` | `-c` missing from `CFLAGS`: the compile rule then tries to link the object | keep `-c` in `CFLAGS` (the link is a separate rule) |
| `'PSUI' was not declared in this scope` | old makefiles had no `-DENABLE_SYNTH_*` | keep the `SYNTH` list |
| `'to_string' is not a member of 'std'` | uClibc libstdc++ has no C99 `std::string` conversions | do not use `std::to_string`/`stoi`/`stod` here |

## 7. Notes and limits

- Reproducible for a fixed image and source: the toolchain bytes are pinned by
  SHA-256 and the base is a dated `ubuntu:24.04` tag (pin it by digest if you
  need more).
- The image build needs network access to `web.archive.org` (~144 MB). The
  compiled output does not need the network.
- GCC 4.9.1 means no C++14/17: keep new code C++11-clean if it should build for
  this target.
- Nothing verified at runtime; no device and no emulator.
- The `.dge` is not stripped (890 exported text symbols), but this makefile
  compiles without `-g`, so there is no line information. Add `-ggdb` to
  `CFLAGS` for a binary where
  `mipsel-gcw0-linux-uclibc-addr2line -f -C -e PatternPlayer.dge <offset>`
  resolves file and line; without it, only symbol names resolve.
