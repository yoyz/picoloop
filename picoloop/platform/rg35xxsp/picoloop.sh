#!/bin/bash
# Reference launcher for the RG35XX SP (KNULLI) picoloop port.
# Deploy as /userdata/roms/ports/picoloop/picoloop.sh (EmulationStation runs it
# from Ports). It stops PipeWire, sets up the codec/speaker ALSA mixers, then
# runs ./picoloop from the port directory.

# --- DEBUGGING ------------------------------------------------------------
# DEBUG_GDB=0 (default, native): fastest startup, normal ASLR, and a crash
#   report is written to picoloop.err by the in-app handler (signal, si_code,
#   fault address, pc, backtrace). Pull it with
#     adb pull /userdata/roms/ports/picoloop/picoloop.err .
# DEBUG_GDB=1: run under gdb and dump a full backtrace (all threads, registers)
#   to gdb.log on crash. Slower startup, extra memory, ASLR disabled.
DEBUG_GDB=0
# --------------------------------------------------------------------------

# Free the ALSA playback device before RtAudio tries to open it.
# NOTE: `fuser` writes only the PIDs to stdout (the "pid/command" listing goes
# to stderr), so grepping its output for "pipewire" never matched and PipeWire
# kept pcmC0D0p open - RtAudio then failed with
#   "pcm device (hw:0,0) won't open for output ... Device or resource busy"
# and picoloop quit. Test for any holder instead.
if [ -n "$(fuser /dev/snd/pcmC0D0p 2>/dev/null)" ]; then
  /etc/init.d/S06audio stop >/dev/null 2>&1
  STOPPED_AUDIO=1
  sleep 1            # let ALSA release the device
fi
# Force codec speaker path + full output volume
amixer -c 0 -q cset numid=2 63
amixer -c 0 -q cset numid=3 31
amixer -c 0 -q cset numid=5 on
amixer -c 0 -q cset numid=6 on
amixer -c 0 -q cset numid=7 on
amixer -c 0 -q cset numid=8 on
amixer -c 0 -q cset numid=9 on
amixer -c 0 -q cset numid=4 on
cd /userdata/roms/ports/picoloop
STOPPED_AUDIO=${STOPPED_AUDIO:-0}

if [ "$DEBUG_GDB" = "1" ]; then
  GDB_LOG=/userdata/roms/ports/picoloop/gdb.log
  rm -f "$GDB_LOG"

  gdb -batch \
    -ex "set pagination off" \
    -ex "set confirm off" \
    -ex "handle SIGPIPE nostop noprint pass" \
    -ex "handle SIGXFSZ nostop noprint pass" \
    -ex run \
    -ex "printf \"\n===== STOPPED - dumping state =====\n\"" \
    -ex "printf \"crashed pc: %p\n\", \$pc" \
    -ex "printf \"===== BACKTRACE (current thread) =====\n\"" \
    -ex "bt" \
    -ex "printf \"===== ALL THREADS =====\n\"" \
    -ex "info threads" \
    -ex "thread apply all bt full" \
    -ex "printf \"===== REGISTERS =====\n\"" \
    -ex "info registers" \
    ./picoloop 2>&1 | tee "$GDB_LOG"
else
  # Native run (default). stdout is discarded to avoid writing to the SD card
  # during playback, but stderr is kept: the crash handler in PatternPlayer.cpp
  # prints signal, si_code, fault address, pc and a backtrace there.
  # Retrieve it with: adb pull /userdata/roms/ports/picoloop/picoloop.err .
  ./picoloop >/dev/null 2>picoloop.err
fi

# Hand the audio device back to EmulationStation if we took it away.
if [ "$STOPPED_AUDIO" = "1" ]; then
  /etc/init.d/S06audio start >/dev/null 2>&1
fi