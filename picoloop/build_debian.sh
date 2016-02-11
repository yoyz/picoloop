#!/bin/bash
make      -f Makefile.RtAudio_debian clean
make      -f Makefile.RtAudio_debian clean
make      -f Makefile.PatternPlayer_debian_RtAudio clean

make -j 4 -f Makefile.RtAudio_debian
make -j 4 -f Makefile.RtMidi_debian
make -j 4 -f Makefile.PatternPlayer_debian_RtAudio

