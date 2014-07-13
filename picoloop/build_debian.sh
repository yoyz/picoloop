#!/bin/bash
make -j 64 -f Makefile.RtAudio clean
make -j 64 -f Makefile.PatternPlayer  clean

make -j 64 -f Makefile.RtAudio
make -j 64 -f Makefile.PatternPlayer

