#!/bin/bash
set -x
g++ -O0 -c -ggdb main.cpp            -o main.o
g++ -O0 -c -ggdb WMath.cpp           -o WMath.o
g++ -O0 -c -ggdb synth_waveform.cpp  -o synth_waveform.o
g++ -O0 -c -ggdb synth_pwm.cpp       -o synth_pwm.o
g++ -O0 -c -ggdb AudioStream.cpp     -o AudioStream.o
g++ -O0 -c -ggdb mixer.cpp           -o mixer.o

g++ -ggdb  main.o WMath.o synth_waveform.o synth_pwm.o AudioStream.o mixer.o -o main
