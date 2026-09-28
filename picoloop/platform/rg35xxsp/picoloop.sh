#!/bin/bash
# Reference launcher for the RG35XX SP (KNULLI) picoloop port.
# Deploy as /userdata/roms/ports/picoloop/picoloop.sh (EmulationStation runs it
# from Ports). It stops PipeWire, sets up the codec/speaker ALSA mixers, then
# runs ./picoloop from the port directory.

# --- DEBUGGING ------------------------------------------------------------
# Set DEBUG_GDB to 1 to run the binary under gdb and dump a full backtrace on
# crash to /userdata/roms/ports/picoloop/gdb.log. Set back to 0 for normal runs.
DEBUG_GDB=0
# --------------------------------------------------------------------------

# Stop broken PipeWire that may hold the ALSA device
if fuser /dev/snd/pcmC0D0p 2>/dev/null | grep -q pipewire; then
  /etc/init.d/S06audio stop >/dev/null 2>&1
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

if [ "$DEBUG_GDB" = "1" ]; then
  GDB_LOG=/userdata/roms/ports/picoloop/gdb.log
  rm -f "$GDB_LOG"

  exec gdb -batch \
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
  exec ./picoloop
fi