# Building picoloop for the PSP

The PSP (PlayStation Portable) build cross-compiles on a normal Linux PC with
the **pspdev** toolchain, which is published as a container image. Nothing has
to be installed on the host except `podman` or `docker`, and no PSP hardware is
needed to produce `EBOOT.PBP`.

pspdev documentation and the image/installation instructions:

- <https://pspdev.github.io/installation/docker.html> (container installation)
- <https://pspdev.github.io/> (project start page)

Reference layout (what this tree uses):

    picoloop/picoloop/                       source (this repo)
    .../Makefile.PatternPlayer_psp_SDL       build file
    .../psp_callback.cpp                     PSP exit-callback thread
    .../font.ttf                             font loaded at runtime (not embedded on PSP)
    .../psp/EBOOT.PBP                        built artifact (gitignored)
    .../doc/psp/build.md                     this file

## 1. Get the container image

    podman pull docker://pspdev/pspdev:latest

or with docker:

    docker pull pspdev/pspdev:latest

Check what you have (the build in this document was verified against this
image; pin it by digest if you need a reproducible toolchain):

    podman images pspdev/pspdev

| | value |
|---|---|
| image | `docker.io/pspdev/pspdev:latest` |
| digest used here | `sha256:54895e6f5afb71b8f4f6915ee6d5023e1e087ca7afdf2dcb1d7a694a5233e45f` |
| compressed size | 549 MB (`podman images`), 0.65 GB uncompressed |
| toolchain | `psp-gcc (GCC) 15.2.0`, `PSPDEV=/usr/local/pspdev` |
| SDK | libSDL (SDL 1.2), libSDL_ttf, freetype, libpng, zlib, bzip2, `psp-config`, `sdl-config` |

Everything is inside the image: cross compiler, PSPSDK, SDL 1.2, SDL_ttf and
the freetype/png/z/bz2 dependencies. There is no host-side setup step.

## 2. Build

The PSP target builds *in tree* (pspdev's `build.mak` has no object directory),
so by default `EBOOT.PBP` and the `.o` files land next to the sources. The
recommended variant builds in a scratch copy and keeps the source tree clean.
Run these from the source directory (`picoloop/picoloop`):

    rm -rf /tmp/picoloop-psp && mkdir -p /tmp/picoloop-psp
    rsync -a --exclude 'debian_*' --exclude 'windows_*' --exclude 'rg35xxsp*' \
          --exclude '*.o' --exclude '*.d' --exclude 'picoloop_*' \
          --exclude 'PatternPlayer_*' ./ /tmp/picoloop-psp/

    podman run --rm --userns=keep-id -v /tmp/picoloop-psp:/src:z -w /src \
        pspdev/pspdev:latest \
        make -f Makefile.PatternPlayer_psp_SDL -j"$(nproc)"

    mkdir -p psp && cp /tmp/picoloop-psp/EBOOT.PBP psp/EBOOT.PBP

With docker, use `-u "$(id -u):$(id -g)"` instead of `--userns=keep-id` (rootless
podman also needs the `:z` SELinux label on the bind mount, as shown).

To build directly in the source tree instead (faster for repeated builds, but
leaves `.o`/`.elf`/`.prx` behind):

    podman run --rm --userns=keep-id -v "$PWD":/src:z -w /src \
        pspdev/pspdev:latest \
        make -f Makefile.PatternPlayer_psp_SDL -j"$(nproc)"

Build time is about 10 seconds on a 24-core host (60 translation units - the PSP
target only compiles the four enabled engines).

### 2.1 Outputs

| file | size (verified build) | purpose |
|---|---:|---|
| `EBOOT.PBP` | 6.0 MB | what you copy to the PSP (PBP wrapping `picoloop.prx`) |
| `picoloop.prx` | 6.25 MB | PRX packaged inside the PBP |
| `picoloop.elf` | 8.7 MB | unstripped link output, only for `psp-addr2line`/crash analysis |
| `PARAM.SFO` | small | PBP metadata, title `picoloop`, `MEMSIZE`, FW 5.00 |

`psp/EBOOT.PBP` in the source tree is gitignored, like the other build outputs.

## 3. Deploy to the PSP

Copy the PBP to a game folder on the memory stick (or the internal storage of
a PSP Go, `ef0:`), together with `font.ttf`:

    ms0:/PSP/GAME/PICOLOOP/EBOOT.PBP
    ms0:/PSP/GAME/PICOLOOP/font.ttf

`font.ttf` is **required**: unlike the desktop SDL2 build, the PSP does not use
the embedded assets (`EmbeddedAssets.cpp` is PC_DESKTOP-only), so `openTTFFont()`
falls back to `TTF_OpenFont("font.ttf", ...)` relative to the game directory.
Without it picoloop exits with a "ttf font error" right after video init.

Storage is relative to the current directory: `GETPICOLOOPUSERSTORAGE()` returns
`"./"` (`SYSTEMLINUX.cpp`), so banks, patterns and `picoloop.log` are written
under `ms0:/PSP/GAME/PICOLOOP/bank/...` next to the EBOOT. The memory stick must
not be write-protected.

Running under an emulator is the usual smoke test without hardware:

    ppsspp psp/EBOOT.PBP

That is not covered by the build verification in section 7 (no emulator was
available on the machine where this was written, so the binary was verified as a
valid PBP/PRX but not executed).

## 4. What the makefile does

`Makefile.PatternPlayer_psp_SDL` follows the same shape as the other
`Makefile.PatternPlayer_*` files and then hands over to pspdev for the rules:

    TARGET          = picoloop
    EXTRA_TARGETS   = EBOOT.PBP
    PSP_EBOOT_TITLE = picoloop
    BUILD_PRX       = 1
    PSP_FW_VERSION  = 500
    ...
    PSPSDK = $(shell psp-config --pspsdk-path)
    include $(PSPSDK)/lib/build.mak

Source groups come from `Makefile_sources` (`$(SOURCES_PICOLOOP)`,
`$(SOURCES_PICOSYNTH)`, ...), plus `psp_callback.cpp`, `AudioDriverSDL.cpp`
and `SYSTEMLINUX.cpp`; `OBJS` is derived from `SOURCES`.

Notable choices, all of them forced by the toolchain:

- **`-std=c++11`**: psp-gcc 15.2 defaults to `-std=gnu++17`, in which
  `Machine/Dbopl/setup.h`'s dynamic exception specifications are a hard error.
- **`-c` is not in `CFLAGS`**: `build.mak` links with `$(LINK.c)`, which reuses
  `CFLAGS`; a `-c` there makes the link a no-op.
- **`libSDLmain` is not linked** (`sdl-config --libs` minus `-lSDLmain`, and no
  `-Dmain=SDL_main`): `PatternPlayer.cpp` defines its own `main()` and
  `PSP_MODULE_INFO`/`PSP_MAIN_THREAD_ATTR`, so linking SDLmain adds a second
  `module_info` and renames `main` to `SDL_main`.
- **`-lstdc++` is explicit**: the driver is `psp-gcc`, not `psp-g++`.
- **`-lpng -lz -lbz2` after `-lfreetype`**: pspdev's `libfreetype.a` is built
  with PNG, gzip and bzip2 support, so those symbols must resolve.

Only four engines are enabled - `PICOSYNTH`, `PICODRUM`, `OPL2` (Dbopl) and
`PBSYNTH` - because every `MonoMixer` instantiates one copy of every enabled
engine and there are four mixers. On a 32 MB PSP the heavier engines
(Cursynth, Open303, Twytch, MDADrum, SID, LGPT sampler, MIDI out, ER-1) are
4× each; they are deliberately left out. Enabling one is a two-line change:
add its `-DENABLE_SYNTH_*` to `SYNTH` and its `$(SOURCES_*)`/
`$(SOURCES_*UI)` pair to `SOURCES`.

## 5. Rebuilding and cleaning

    # rebuild in place (objects are up to date, only changed files recompile)
    podman run --rm --userns=keep-id -v "$PWD":/src:z -w /src \
        pspdev/pspdev:latest \
        make -f Makefile.PatternPlayer_psp_SDL -j"$(nproc)"

    # clean: removes OBJS, EBOOT.PBP, PARAM.SFO, .elf and .prx (build.mak rule)
    podman run --rm --userns=keep-id -v "$PWD":/src:z -w /src \
        pspdev/pspdev:latest \
        make -f Makefile.PatternPlayer_psp_SDL clean

`make rebuild` also works, it is `clean` followed by `all` from `build.mak`.

## 6. Troubleshooting

Every item below was hit while making this target build again on the current
pspdev image.

| symptom | cause | fix |
|---|---|---|
| `'PSUI' was not declared in this scope` at `PatternPlayer.cpp` | no `-DENABLE_SYNTH_*` at all: the machine UIs are `#ifdef`-gated but `main()` uses `PSUI` unconditionally | keep the `SYNTH` list in the makefile (added) |
| `ISO C++17 does not allow dynamic exception specifications` in `Machine/Dbopl/setup.h` | psp-gcc 15.2 defaults to gnu++17 | `-std=c++11` |
| link reports `linker input file unused because linking not done`, then `Error, could not find file picoloop.elf` | `-c` in `CFLAGS`, reused by `$(LINK.c)` | do not put `-c` in `CFLAGS` |
| `multiple definition of 'module_info'` + `undefined reference to SDL_main` | `libSDLmain` linked while `PatternPlayer.cpp` has its own module info/main | drop `-lSDLmain`, no `-Dmain=SDL_main` |
| `undefined reference to png_*`, `inflate*`, `BZ2_*` | pspdev `libfreetype.a` is built with PNG/gzip/bzip2 | `-lpng -lz -lbz2` |
| `undefined reference to _Znwj`, `_ZdlPv`, `__throw_system_error` | C++ runtime missing (`LINK.c` = `psp-gcc`) | `-lstdc++` |
| `psp-config: command not found` | running make outside the container, or a shell without `$PSPDEV/bin` in `PATH` | run through the container as shown above |
| `ttf font error` at startup on the device | `font.ttf` not copied next to `EBOOT.PBP` | see section 3 |

## 7. Notes and limits

- The build is deterministic for a fixed image digest and source state: same
  inputs, same `EBOOT.PBP`.
- `picoloop/psp/`, `EBOOT.PBP`, `PARAM.SFO`, `picoloop.elf`, `picoloop.prx` and
  `picoloop_strip.elf` are in `.gitignore`. In-tree builds also leave `.o` files
  next to the sources, which are **not** ignored - prefer the scratch-copy
  recipe in section 2.
- `PSP_FW_VERSION = 500` controls the `MEMSIZE`/SFO metadata and the module
  flags; raise it if you need a newer-firmware-only feature.
- No runtime verification was done for this build (hardware or PPSSPP). What is
  verified is only that the toolchain produces a complete, valid PBP with the
  expected title and module.
