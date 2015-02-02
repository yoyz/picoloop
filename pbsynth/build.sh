#!/bin/bash
set -x
g++ -DFIXED -c osctab.c        -o osctab.o
g++ -DFIXED -c lfotab.c        -o lfotab.o
g++ -DFIXED -c coeff.c         -o coeff.o
g++ -DFIXED -c envtab.c        -o envtab.o
g++ -DFIXED -c synthengine.cpp -o synthengine.o
g++ -DFIXED -c osc.cpp         -o osc.o
g++ -DFIXED -c lfo.cpp         -o lfo.o
g++ -DFIXED -c filter.cpp      -o filter.o
g++ -DFIXED -c env.cpp         -o env.o
g++ -DFIXED -c echo.cpp        -o echo.o
g++ -DFIXED -c sdl.cpp         -o sdl.o
g++ -DFIXED -c test.cpp        -o test.o
g++ -DFIXED synthengine.o osc.o lfo.o test.o filter.o env.o echo.o osctab.o lfotab.o coeff.o envtab.o sdl.o

# g++ -DFIXED -c test.cpp -o test.o && g++ -DFIXED synthengine.o osc.o lfo.o test.o filter.o env.o echo.o osctab.o lfotab.o coeff.o envtab.o sdl.o && ./a.out
