# Building picoloop for the PS Vita

The PS Vita build cross-compiles on a normal Linux PC with the **VitaSDK**
toolchain, which is published as a container image. Nothing has to be installed
on the host except `podman` or `docker`, and no Vita is needed to produce a
`.vpk`.

VitaSDK documentation and the official container images:

- <https://github.com/vitasdk/docker> (this is the image's source; the README
  documents tags and variants)
- <https://vitasdk.org/> (project start page / installation)
- <https://docs.vitasdk.org/> (API documentation)

Reference layout (what this tree uses):

    picoloop/picoloop/                       source (this repo)
    .../Makefile.PatternPlayer_psvita_SDL2   build file
    .../SYSTEMPSVITA.cpp                     platform glue
    .../AudioDriverPSVita.cpp                audio driver (SDL2 + SceAudio)
    .../psp2shell.h, p2s_cmd.h, ...          in-tree debug shell headers
    .../libpsp2shell.a                      in-tree static library (linked -L.)
    .../sce_sys/                            VPK metadata (icon0, livearea)
    .../font.ttf                            font packaged into the VPK
    .../samples/                            optional wavs for the LGPT sampler
    .../psvita/picoloop_vita.vpk            built artifact (gitignored)

## 1. Get the container image

    podman pull docker://vitasdk/vitasdk:2026.08

or with docker:

    docker pull vitasdk/vitasdk:2026.08

Pin the series (or a dated tag) rather than `latest`: since the 2026.08
pipeline, `latest` means "newest supported series", not "nightly" - it used to
be a nightly, and it will move again when a new series ships. `nightly` and
`nightly-<date>` still exist for the old behaviour.

    podman images vitasdk/vitasdk

| | value |
|---|---|
| image | `docker.io/vitasdk/vitasdk:2026.08` (full variant) |
| multi-arch index digest | `sha256:8227da1b596d981f4513cd9b36bda1e43857f6dae5bcf4312c1cd40167f7d9bb` |
| amd64 manifest digest | `sha256:4f726038a59f57bee7341d2b66862c72fe24f060044165c2f331da3f181f3138` |
| size | 1.57 GB on disk |
| base | Ubuntu 24.04 (glibc), pinned by digest in their CI |
| toolchain | `arm-vita-eabi-g++ (GCC) 15.2.0`, `VITASDK=/usr/local/vitasdk` |
| runs as | `root` by default (`-non-root` tag or `--user 1000:0` also work) |
| packaging tools | `vita-elf-create`, `vita-make-fself`, `vita-mksfoex`, `vita-pack-vpk` |
| libraries | SDL2, SDL2_ttf, freetype, png, z, bz2, vita2d, vitaGL, vitashark, mathneon, `Sce*_stub` set |

The full variant matters: `minimal` carries only the core toolchain and would
not have SDL2/SDL2_ttf.

### 1.1 Is this the official image?

It is the community-official one, and it is worth knowing on what basis:

- The `vitasdk/docker` repository is listed in the `vitasdk` GitHub org profile
  under *Project Infrastructure* ("container and build environments"),
  alongside `vdpm`, `vita-toolchain`, `packages` and `buildscripts`.
- Its Dockerfile installs nothing exotic: Ubuntu packages, then
  `bootstrap-vitasdk.sh` + `SHA256SUMS` from
  `github.com/vitasdk/vdpm/releases/latest/download`, checksum-verified before
  running, then deleted. The SDK is installed as an unprivileged user and made
  group-writable so any UID can use it.
- Published images carry OCI labels describing exactly what is inside. Check
  them on a pulled image:

      podman inspect vitasdk/vitasdk:2026.08 --format '{{json .Config.Labels}}'

  Expected (2026.08): `org.vitasdk.channel=2026.08`,
  `org.vitasdk.channel.sequence=2026101567`,
  `org.vitasdk.core.release=sdk-snapshot-20260825.611.1`,
  `org.vitasdk.packages.release=packages-2026.08-snapshot-20261015.56.1`.
- Tags are multi-arch (`linux/amd64` + `linux/arm64`), pushed from CI after it
  builds and smoke-links a real `.vpk` as root and as an arbitrary UID.

Caveats, so this is not read as more than it is:

- **Trust root is GitHub.** The checksum file comes from the same release as
  the script it verifies, so it protects against a corrupted download, not a
  compromised upstream release. This is the same trust as running
  `bootstrap-vitasdk.sh` by hand, which is all the image does.
- **It is not Sony's SDK.** Headers and `Sce*` stubs are the community
  reverse-engineered set; VitaSDK states it is not affiliated with or endorsed
  by Sony. That is normal for Vita homebrew.
- The plain tag runs as root so derived Dockerfiles and CI keep working; use
  `-non-root` or `--user` when that matters.

## 2. Build

The Vita target already writes its objects into an `out/` tree, so building in
the source tree is fine and leaves only `out/`, the `*.elf`/`*.velf`,
`eboot.bin`, `param.sfo` and the `.vpk` behind - all gitignored. Run from the
source directory (`picoloop/picoloop`):

    podman run --rm --userns=keep-id -v "$PWD":/src:z -w /src \
        vitasdk/vitasdk:2026.08 \
        make -f Makefile.PatternPlayer_psvita_SDL2 -j"$(nproc)"

    cp picoloop_vita.vpk psvita/          # optional: keep the artifact

With docker, use `-u "$(id -u):$(id -g)"` instead of `--userns=keep-id`
(rootless podman also needs the `:z` SELinux label on the bind mount).

Build time is about 25 seconds on a 24-core host (86 translation units - the
Vita target compiles five engines).

### 2.1 Outputs

| file | size (verified build) | purpose |
|---|---:|---|
| `picoloop_vita.vpk` | 4.6 MB | what you install on the Vita |
| `eboot.bin` | 8.7 MB | SELF-wrapped executable inside the VPK |
| `picoloop_vita.velf` | 8.7 MB | vita-elf-create output, signed-packed into eboot.bin |
| `picoloop_vita.elf` | 7.7 MB | stripped link output, for `arm-vita-eabi-addr2line` |
| `param.sfo` | 912 B | title/TITLE_ID metadata |

`make` (default target) produces the VPK; `make package` is the same thing.
`picoloop/psvita/` and all of the above are in `.gitignore`.

## 3. Deploy

Copy `picoloop_vita.vpk` to the Vita (USB/FTP through VitaShell, or
`curl -T picoloop_vita.vpk ftp://<vita>:1337/ux0:/Vpk/`), then install it from
VitaShell. It installs as:

    ux0:/app/PICOLOOP1/

`TITLE_ID` **must** stay `PICOLOOP1`: the code hardcodes it in two places,
`ux0:/app/PICOLOOP1/font.ttf` (font) and `ux0:/app/PICOLOOP1/samples/%d.wav`
(LGPT sample pool). Both paths are inside the installed app directory.

Inside the VPK: `eboot.bin`, `sce_sys/param.sfo`, `sce_sys/icon0.png`,
`sce_sys/livearea/contents/{bg.png,startup.png,template.xml}` and `font.ttf`.
Samples are packaged only if `samples/*.wav` exists when you build; without
them the LGPT sampler starts empty (`SamplePool::loadSample()` logs and skips
missing files, nothing else fails).

There is no emulator verification in this document: the VPK was built and
inspected (valid `SCE\0` self, correct title/TITLE_ID, expected files inside)
but never executed.

## 4. What the makefile does

`Makefile.PatternPlayer_psvita_SDL2` follows the same shape as the other
`Makefile.PatternPlayer_*` files, then adds the Vita packaging chain:

    $(PROJECT).elf  -> strip -g -> vita-elf-create -> $(PROJECT).velf
    $(PROJECT).velf -> vita-make-fself       -> eboot.bin
    param.sfo       <- vita-mksfoex -s TITLE_ID=PICOLOOP1
    .vpk            <- vita-pack-vpk -s param.sfo -b eboot.bin ... $(EXECUTABLE)

Source groups come from `Makefile_sources`; the platform files are
`SYSTEMPSVITA.cpp` and `AudioDriverPSVita.cpp`. Objects go to `out/`, created
on demand by the `out/%.o` rule.

Flags that are not obvious:

- **`-D__SDL20__`** - the Vita uses SDL2 (the PSP target uses SDL 1.2).
- **`-DPSVITA -D__PSVITA_AUDIO__`** - selects the Vita platform block in
  `Master.h` (960x544, `SCREEN_MULT=2`, `__RAM512MIB__`) and the SceAudio driver.
- **`-DDEBUGPRINTF=1`** - on PSVITA `DPRINTF` is only defined when
  `DEBUGPRINTF==1` (it routes to `sceClibPrintf`). Without it every `DPRINTF()`
  is an undeclared identifier.
- **`-fpermissive`** - legacy code, same as the other targets.

Engines: `PICOSYNTH`, `PICODRUM`, `OPL2` (Dbopl), `PBSYNTH` and the LGPT
sampler. LGPT is kept because PSVITA sets `__RAM512MIB__`, which makes `main()`
call `init_lgpt_samplepool()`; dropping the engine would mean guarding that
call too. No ER-1, Cursynth, Open303, Twytch, MDA, SID or MIDI out.

### 4.1 Why the link line is long

SDL2 on the Vita is built on **vitaGL**, and each of these is a hard
dependency of the one before it:

| library | needed for |
|---|---|
| `-lSDL2_ttf -lfreetype -lpng -lz -lbz2` | text rendering; vitasdk's freetype is built with png/gzip/bzip2 |
| `-lSDL2` | the app itself (SDL2 video/audio/joystick) |
| `-lvitaGL -lvitashark -lmathneon` | SDL2's Vita video backend, its Shark shader compiler and NEON math |
| `-lSceShaccCgExt -lSceShaccCg_stub -ltaihen_stub` | shader compiler extensions (via taiHEN) |
| `-lSceKernelDmacMgr_stub` | `sceDmacMemcpy`, used by vitaGL buffers |
| `-lSceAudio_stub -lSceAudioIn_stub` | audio out, audio capture |
| `-lSceMotion_stub -lSceIme_stub` | SDL2 sensors and IME |
| `-lvita2d -lvita2d_vgl -lSceGxm_stub -lSceDisplay_stub` | drawing / display |
| `-lSceCtrl_stub -lSceTouch_stub -lSceHid_stub` | input |
| `-lSceSysmodule_stub -lSceCommonDialog_stub -lSceAppMgr_stub -lScePower_stub -lSceNet_stub -lSceNetCtl_stub -lSceLibKernel_stub` | modules, dialogs, power, net |
| `-L. -lpsp2shell` | the in-tree `libpsp2shell.a` (debug shell) |

## 5. Rebuilding and cleaning

    # incremental
    podman run --rm --userns=keep-id -v "$PWD":/src:z -w /src \
        vitasdk/vitasdk:2026.08 \
        make -f Makefile.PatternPlayer_psvita_SDL2 -j"$(nproc)"

    # remove objects, elf/velf, eboot.bin, param.sfo and the vpk
    podman run --rm --userns=keep-id -v "$PWD":/src:z -w /src \
        vitasdk/vitasdk:2026.08 \
        make -f Makefile.PatternPlayer_psvita_SDL2 clean

`clean` also removes the whole `out/` tree, so the next build is a full rebuild
(~25 s).

## 6. Troubleshooting

All of these were hit while making this target build again on the current
VitaSDK image.

| symptom | cause | fix |
|---|---|---|
| `'PSUI' was not declared in this scope` | no `-DENABLE_SYNTH_*`: the machine UIs are `#ifdef`-gated but `main()` uses `PSUI` unconditionally | keep the `SYNTH` list in the makefile |
| `'BUFFER_FRAME' was not declared` | `AudioDriverPSVita.cpp` used a macro that only exists commented-out in `Master.h` | use `DEFAULTSAMPLES` (as `AudioDriverSDL.cpp` already does) |
| `'DPRINTF' was not declared` | PSVITA only defines `DPRINTF` when `DEBUGPRINTF==1` | `-DDEBUGPRINTF=1` |
| `vita-pack-vpk: ... samples/0.wav: No such file` | the recipe hardcoded `samples/0..49.wav`, which are not in the repo | package `$(wildcard samples/*.wav)` instead (done) |
| `undefined reference to png_*`, `BZ2_*` | vitasdk freetype is built with png/gzip/bzip2 | `-lpng -lz -lbz2` |
| `undefined reference to vglInitExtended`, `glFinish`, `eglSwapInterval` | SDL2's Vita backend is vitaGL | `-lvitaGL` |
| `undefined reference to shark_init`, `matmul4_neon`, `sincosf_c` | vitaGL needs the Shark compiler + NEON math | `-lvitashark -lmathneon` |
| `undefined reference to sceShaccCg*`, `sceShaccCgExt*` | shader compiler stubs and their extension layer | `-lSceShaccCg_stub -lSceShaccCgExt` |
| `undefined reference to taiInjectRelease` | `libSceShaccCgExt` needs taiHEN | `-ltaihen_stub` |
| `undefined reference to sceDmacMemcpy` | vitaGL buffers | `-lSceKernelDmacMgr_stub` |
| `undefined reference to sceAudioIn*`, `sceMotion*`, `sceImeUpdate` | SDL2 capture/sensors/IME | `-lSceAudioIn_stub -lSceMotion_stub -lSceIme_stub` |
| `undefined reference to vglSwapBuffers` (from `SDL_vitamessagebox.c`) | vita2d's vgl bridge not linked | `-lvita2d_vgl` |
| `arm-vita-eabi-g++: command not found` | running make outside the container | run through the image as shown above |

## 7. Notes and limits

- Pin the image (`2026.08` and, if you need it frozen, `2026.08-20261015.2` or
  the digest above). `latest` follows the newest series, `nightly` follows the
  development channel.
- `TITLE_ID`/`PROJECT_TITLEID` is `PICOLOOP1` and is referenced by the source
  (`ux0:/app/PICOLOOP1/...`); changing it means changing those paths too.
- `samples/` is optional for the build and for startup, but the LGPT sampler
  machine will be silent without it.
- No runtime verification was done: the VPK is verified as a well-formed
  package (SELF, TITLE_ID, contents) but was not run on hardware or emulator.
- Debug symbols are **not** kept: the `.velf` rule runs `strip -g` on
  `picoloop_vita.elf` in place before `vita-elf-create`, so the `.elf` left
  behind has no `.debug_*` sections. For crash analysis, keep a copy before the
  strip (e.g. `cp $(PROJECT).elf $(PROJECT).elf.debug` as the first command of
  that rule, or comment the `strip` out for a debug build) and resolve with
  `arm-vita-eabi-addr2line -f -C -e <unstripped elf> <offset>`.
