#!/bin/bash
make -j 64 -f Makefile.RtAudio_opendingux               clean
make -j 64 -f Makefile.PatternPlayer_opendingux_RtAudio clean

make -j 64 -f Makefile.RtAudio_opendingux     
make -j 64 -f Makefile.PatternPlayer_opendingux_RtAudio

