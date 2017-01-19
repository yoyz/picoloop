#!/bin/bash

# resid 0.16
g++ -c sid.cc          -o sid.o
g++ -c wave.cc         -o wave.o
g++ -c filter.cc       -o filter.o
g++ -c envelope.cc     -o envelope.o
g++ -c extfilt.cc      -o extfilt.o
g++ -c pot.cc          -o pot.o
g++ -c voice.cc        -o voice.o
g++ -c wave6581_PS_.cc -o wave6581_PS_.o
g++ -c wave6581_PST.cc -o wave6581_PST.o
g++ -c wave6581_P_T.cc -o wave6581_P_T.o
g++ -c wave6581__ST.cc -o wave6581__ST.o
g++ -c wave8580_PS_.cc -o wave8580_PS_.o
g++ -c wave8580_PST.cc -o wave8580_PST.o
g++ -c wave8580_P_T.cc -o wave8580_P_T.o
g++ -c wave8580__ST.cc -o wave8580__ST.o

# reMID 0.9.1
g++ -I. -c sid_chips.cc -o sid_chips.o       # this one was tweaked because of jack dependency...

# TODO write the "main.c"
# the key process function is in vim ../reMID-0.9.1/src/jack_audio.c
# need to double check the version.c file in resid which may be a problem in the build

g++ main.cpp sid.o wave.o filter.o envelope.o extfilt.o pot.o voice.o wave6581_PS_.o wave6581_PST.o wave6581_P_T.o wave6581__ST.o wave8580_PS_.o wave8580_PST.o wave8580_P_T.o wave8580__ST.o -o main
