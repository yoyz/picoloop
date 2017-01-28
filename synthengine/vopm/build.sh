#!/bin/bash
set -x
g++ -O3 -c OPM.cpp OPMdrv.cpp op.cpp lfo.cpp
g++ -c main.cpp       -o main.o
g++ -c main-load.cpp  -o main-load.o
g++ OPM.o OPMdrv.o op.o lfo.o main.o      -o main
g++ OPM.o OPMdrv.o op.o lfo.o main-load.o -o main-load
