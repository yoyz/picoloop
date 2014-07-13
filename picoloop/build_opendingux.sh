#!/bin/bash
make -j 64 -f Makefile.RtAudio clean
make -j 64 -f Makefile.PatternPlayer  clean

make -j 64 -f Makefile.RtAudio_opendingux
make -j 64 -f Makefile.PatternPlayer_opendingux 

