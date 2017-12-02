#!/bin/bash
set -x
g++ -c -ggdb main.cpp            -o main.o
g++ -c -ggdb WMath.cpp           -o WMath.o
g++ -c -ggdb synth_waveform.cpp  -o synth_waveform.o
g++ -c -ggdb AudioStream.cpp     -o AudioStream.o

g++ -ggdb  main.o WMath.o synth_waveform.o AudioStream.o -o main
