#!/bin/bash
set -x
rm bench main main-load *.o
g++ -O2 -c OPM.cpp OPMdrv.cpp op.cpp lfo.cpp
g++ -O2 -c main.cpp       -o main.o
g++ -O2 -c main-load.cpp  -o main-load.o
g++ -O2 -g -ggdb -c bench.cpp       -o bench.o
g++ OPM.o OPMdrv.o op.o lfo.o main.o        -o main
g++ OPM.o OPMdrv.o op.o lfo.o main-load.o   -o main-load
g++ -ggdb OPM.o OPMdrv.o op.o lfo.o bench.o -o bench
