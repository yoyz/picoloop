#!/bin/bash
set -x
export CFLAGS='-std=c++11 -DHAVE_CSTDINT'
if [ 0 -eq 1 ] ; then 
g++ $CFLAGS -c sound/channel1.cpp -o sound/channel1.o
g++ $CFLAGS -c sound/channel2.cpp -o sound/channel2.o
g++ $CFLAGS -c sound/channel3.cpp -o sound/channel3.o
g++ $CFLAGS -c sound/channel4.cpp -o sound/channel4.o

g++ $CFLAGS -c sound/length_counter.cpp -o sound/length_counter.o
g++ $CFLAGS -c sound/envelope_unit.cpp  -o sound/envelope_unit.o
g++ $CFLAGS -c sound/duty_unit.cpp      -o sound/duty_unit.o

fi
g++ $CFLAGS -c sound.cpp                -o sound.o

g++ $CFLAGS main.cpp sound.o sound/channel1.o sound/channel2.o sound/channel3.o sound/channel4.o sound/envelope_unit.o  sound/duty_unit.o sound/length_counter.o
