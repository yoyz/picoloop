# Building picoloop for Windows

Windows binaries are **cross-built on Linux** with mingw-w64; nothing needs a
Windows machine or MSVC. There are four targets: SDL 1.2 and SDL2, 32- and
64-bit.

| makefile | arch | output | audio |
|---|---|---|---|
| `Makefile.PatternPlayer_windows_mingw_SDL_32bit` | i686 | `picoloop_windows_mingw_SDL_32bit.exe` | SDL 1.2 audio |
| `Makefile.PatternPlayer_windows_mingw_RtAudio_32bit` | i686 | `picoloop_windows_mingw_RtAudio_32bit.exe` | RtAudio/DirectSound |
| `Makefile.PatternPlayer_windows_mingw_SDL2_32bit` | i686 | `picoloop_windows_mingw_SDL2_32bit.exe` | SDL2 audio |
| `Makefile.PatternPlayer_windows_mingw_SDL2_64bit` | x86-64 | `picoloop_windows_mingw_SDL2_64bit.exe` | SDL2 audio |

The SDL 1.2 pair uses the SDL 1.2 headers/libraries vendored in `SDL_windows/`.
The SDL2 pair needs the MinGW SDL2 development package, which is **not**
vendored - fetch it once with the script in section 2.

Reference layout:

    picoloop/picoloop/                                    source (this repo)
    .../SDL_windows/                                      vendored SDL 1.2 (headers + libs + DLLs)
    .../platform/windows/fetch_sdl2.sh                     SDL2 fetch/layout script
    .../SDL2_windows/                                      fetched SDL2 (gitignored)
    .../windows_mingw_SDL_32bit/ ...                       object trees (gitignored)
    .../picoloop_windows_mingw_*.exe                       binaries (gitignored)
    .../font.ttf, picoloop.ini                             NOT needed (embedded / optional)

## 1. Prerequisites

    apt install g++-mingw-w64-i686 g++-mingw-w64-x86-64      # Debian/Ubuntu
    dnf install mingw32-gcc-c++ mingw64-gcc-c++              # Fedora

Check that the compilers are there (the SDL2 targets exercise both):

    i686-w64-mingw32-g++ --version
    x86_64-w64-mingw32-g++ --version

Both are used in their default **win32** thread model. That is fine for this
codebase (see section 6), and it means the binaries do not need
`libwinpthread-1.dll`.

## 2. Fetch the SDL2 development files

    platform/windows/fetch_sdl2.sh

Pinned content (the script verifies both SHA-256 sums before unpacking):

```
SDL2      2.32.10  SDL2-devel-2.32.10-mingw.tar.gz       sha256 83a5d740…e3938ff2
SDL2_ttf  2.24.0   SDL2_ttf-devel-2.24.0-mingw.tar.gz    sha256 3a09e0a9…74908ef2c3
```

Resulting layout (gitignored; `SDL2_DIR` overrides the destination):

    SDL2_windows/include/SDL2/    headers, both arches
    SDL2_windows/lib32/           libSDL2*.a, libSDL2_ttf*.a, libSDL2main.a
    SDL2_windows/lib64/           same, x86-64
    SDL2_windows/bin32/SDL2.dll, SDL2_ttf.dll
    SDL2_windows/bin64/SDL2.dll, SDL2_ttf.dll

Both versions deliberately match what the RG35XX SP port runs (2.32.8 /
2.24.0), so the SDL2 code paths are exercised against the same generation of
SDL.

## 3. Build

From the source directory (`picoloop/picoloop`):

    # SDL 1.2
    make -f Makefile.PatternPlayer_windows_mingw_SDL_32bit      -j"$(nproc)"
    make -f Makefile.PatternPlayer_windows_mingw_RtAudio_32bit  -j"$(nproc)"

    # SDL2 (needs section 2 first)
    make -f Makefile.PatternPlayer_windows_mingw_SDL2_32bit     -j"$(nproc)"
    make -f Makefile.PatternPlayer_windows_mingw_SDL2_64bit     -j"$(nproc)"

Clean:

    make -f Makefile.PatternPlayer_windows_mingw_SDL2_64bit clean

Each target compiles every translation unit of its engine set - 215 objects for
the SDL2 targets, 12 engines including ER-1 - so a cold build takes about 20 s
on 12 cores.

### 3.1 Outputs

| binary | size (verified build) |
|---|---:|
| `picoloop_windows_mingw_SDL_32bit.exe` | 10.4 MB (PE32, i386) |
| `picoloop_windows_mingw_RtAudio_32bit.exe` | 10.4 MB (PE32, i386) |
| `picoloop_windows_mingw_SDL2_32bit.exe` | 10.4 MB (PE32, i386) |
| `picoloop_windows_mingw_SDL2_64bit.exe` | 10.6 MB (PE32+, x86-64) |

All four are console-subsystem binaries (`Subsystem: Windows CUI`), like the
historical Windows targets - a console window opens next to the game and shows
nothing unless you build with `DEBUGPRINTF_STDOUT`. Add `-mwindows` to
`LDFLAGS` for a windowless release build.

## 4. Runtime requirements

`libgcc` and `libstdc++` are linked statically (`-static-libgcc
-static-libstdc++`), so only SDL and the system DLLs are needed.

SDL2 targets - copy these next to the `.exe` (from `SDL2_windows/bin32` or
`bin64`, matching the architecture):

    SDL2.dll
    SDL2_ttf.dll

SDL 1.2 targets - these four are already committed at the repo root
(`SDL.dll`, `SDL_ttf.dll`, `libfreetype-6.dll`, `zlib1.dll`):

    SDL.dll
    SDL_ttf.dll
    libfreetype-6.dll     (needed by SDL_ttf.dll)
    zlib1.dll             (needed by libfreetype-6.dll)

Nothing else: the font (and, on the SDL2 targets, the window icon) is compiled
into the binary (`EmbeddedAssets.cpp`, `PC_DESKTOP` path), and `picoloop.ini`
is optional. Bank
and pattern files are written relative to the current directory
(`GETPICOLOOPUSERSTORAGE()` is `"./"` on Linux, and the Windows build behaves
the same way), and `picoloop.log` is created next to the binary at runtime
because the makefiles define `-DDEBUGPRINTF`.

## 5. Verification

The SDL2 64-bit binary was smoke-tested under **wine** on Linux:

    WINEPREFIX=/tmp/wine64 WINEARCH=win64 \
    SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy \
    wine64 picoloop_windows_mingw_SDL2_64bit.exe

It initializes SDL2 video/audio, loads the embedded font, probes the joystick
and enters the main loop (`refresh`/`clearScreen` in `picoloop.log`) until
killed. That is a real run of the binary, not just a link check - but with dummy
video and audio drivers, so nothing was heard or seen.

Not verified:

- the 32-bit binaries under wine (this host's wine is wow64-only, no 32-bit
  support), and
- anything on real Windows.

## 6. What the makefiles do

All four follow the same shape (`include Makefile_sources`, `SYNTH` list,
`CFLAGS_SDLVERS`, `OBJDIR`, per-target `.d` dependency files, working `clean`):

    CFLAGS   -c -std=c++11 -O3 -fpermissive ${SYNTH}
             -DWIN32 -D__WIN32__ -DPC_DESKTOP
             -D__SDL12__  (SDL 1.2)   or   -D__SDL20__  (SDL2)
             -D__SDL_AUDIO__ (SDL audio targets) / -D__RTAUDIO__ -D__RTMIDI__
             -DDEBUGPRINTF -DSCREEN_MULT=2 -DDUMP_AUDIO=0   (SDL2 pair)
    SOURCES  AudioDriverSDL.cpp or AudioDriverRTAudio.cpp, SYSTEMWINDOWS.cpp,
             RtMidi.cpp, SysMutex.cpp, Midi*System.cpp, PicoloopIni.cpp,
             EmbeddedAssets.cpp + the engine groups

Notes:

- **SDL_main**: on Windows `SDL_main.h` renames `main` to `SDL_main`, so
  `SDL2main` (`-lmingw32 -lSDL2main -lSDL2`) and SDL 1.2's `libSDLmain.a` must
  be linked before the SDL library. Both makefiles do.
- **`-D_WIN32_WINNT=0x0600 -DWINVER=0x0600`** (SDL2 pair): mingw assumes
  `_WIN32_WINNT_WS03` (0x0502). Once SDL2's headers include `<windows.h>`,
  libstdc++'s win32 thread shim then no longer exposes the Vista+ condition
  variable, and `<mutex>` fails with `__gthread_cond_t does not name a type`.
  Pinning Vista is enough; the alternative (`g++-posix`) would add a
  `libwinpthread-1.dll` dependency.
- **`-DSCREEN_MULT=2`**: the window is 640x480 logical, matching the SDL2
  desktop build. The SDL 1.2 Windows targets keep the `Master.h` default of 1.
- **`Master.h` needs `<stdio.h>`**: its `DEBUGPRINTF` macros use
  `FILE`/`fopen`/`fprintf`; the include was added because on MinGW+SDL2 no
  earlier header pulled it in.
- **`-lSDL_ttf` (SDL 1.2)** works without an import library: GNU ld links
  directly against `SDL_windows/lib/SDL_ttf.dll`.

## 7. Troubleshooting

| symptom | cause | fix |
|---|---|---|
| `'FILE' does not name a type` in `Master.h` | `Master.h` declares `extern FILE * fdebugprintf` without `<stdio.h>` | fixed in `Master.h`; do not remove that include |
| `'__gthread_cond_t' does not name a type` (in `bits/std_mutex.h`) | `_WIN32_WINNT` defaults to Server 2003, SDL2 includes `<windows.h>` first | `-D_WIN32_WINNT=0x0600 -DWINVER=0x0600` (already in the SDL2 makefiles) |
| `undefined reference to 'SDL_main'` | `main` was not renamed / `SDL2main` not linked, or a test `main()` without `(argc, argv)` | link `-lmingw32 -lSDL2main -lSDL2` and keep the real `main(int, char**)` |
| `cannot find -lSDL2` | section 2 not run, or `SDL2_DIR` points at the wrong place | run `platform/windows/fetch_sdl2.sh`, or pass `SDL2_DIR=...` |
| the `.exe` starts then dies with no window | `SDL2.dll`/`SDL2_ttf.dll` not next to the binary | copy them from `SDL2_windows/bin32`/`bin64` |
| SDL 1.2 `.exe` starts then complains about `libfreetype-6.dll` | `SDL_ttf.dll` needs it (and it needs `zlib1.dll`) | ship all four DLLs listed in section 4 |
| `'PSUI' was not declared in this scope` | a makefile without the `SYNTH` list | keep the `SYNTH` block |

## 8. Notes and limits

- The SDL2 fetch is pinned and checksum-verified but needs network access; the
  SDL 1.2 path works offline because those files are in the tree.
- `improvement_plan.md` step 7 lists the committed `SDL.dll`/`SDL_ttf.dll`/
  `libfreetype-6.dll`/`zlib1.dll` as junk to remove. If that happens, the SDL
  1.2 targets need the same treatment as SDL2: a fetch script instead of
  vendored binaries.
- Only the SDL2 64-bit binary was exercised (wine, dummy drivers). The other
  three are verified as building and linking correctly.
- These are console builds; add `-mwindows` in `LDFLAGS` if the console window
  is unwanted.
