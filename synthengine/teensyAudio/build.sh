#!/bin/bash
set -x
gcc -c data_waveforms.c 	-o data_waveforms.o
g++ -c AudioStream.cpp 		-o AudioStream.o
g++ -c synth_waveform.cpp 	-o synth_waveform.o
g++ -c synth_sine.cpp  		-o synth_sine.o
g++ -c mixer.cpp  		-o mixer.o
g++ -c main.cpp			-o main.o

g++ data_waveforms.o AudioStream.o synth_waveform.o synth_sine.o mixer.o main.o -o main
