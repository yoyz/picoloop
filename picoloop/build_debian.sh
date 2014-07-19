#!/bin/bash
make -j 64 -f Makefile.RtAudio_debian clean
make -j 64 -f Makefile.PatternPlayer_debian_RtAudio clean

make -j 64 -f Makefile.RtAudio_debian
make -j 64 -f Makefile.PatternPlayer_debian_RtAudio

