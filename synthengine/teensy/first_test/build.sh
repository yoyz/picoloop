#!/bin/bash
set -x
g++ -O0 -c -ggdb main.cpp            -o main.o
g++ -O0 -c -ggdb WMath.cpp           -o WMath.o
g++ -O0 -c -ggdb synth_waveform.cpp  -o synth_waveform.o
g++ -O0 -c -ggdb AudioStream.cpp     -o AudioStream.o

g++ -ggdb  main.o WMath.o synth_waveform.o AudioStream.o -o main
