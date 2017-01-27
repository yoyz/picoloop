#!/bin/bash
set -x
g++ -DFIXED -O3 -c osctab.c        -o osctab.o
g++ -DFIXED -O3 -c lfotab.c        -o lfotab.o
g++ -DFIXED -O3 -c coeff.c         -o coeff.o
g++ -DFIXED -O3 -c envtab.c        -o envtab.o
g++ -DFIXED -O3 -c synthengine.cpp -o synthengine.o
g++ -DFIXED -O3 -c osc.cpp         -o osc.o
g++ -DFIXED -O3 -c lfo.cpp         -o lfo.o
g++ -DFIXED -O3 -c filter.cpp      -o filter.o
g++ -DFIXED -O3 -c env.cpp         -o env.o
g++ -DFIXED -O3 -c echo.cpp        -o echo.o
g++ -DFIXED -O3 -c sdl.cpp         -o sdl.o
g++ -DFIXED -O3 -c main-fixed.cpp        -o main.o
g++ -DFIXED -O3 -c bench-fixed.cpp       -o bench.o
g++ -DFIXED -O3   main.o  -o main-fixed   synthengine.o osc.o lfo.o filter.o env.o echo.o osctab.o lfotab.o coeff.o envtab.o sdl.o
g++ -DFIXED -O3   bench.o -o bench-fixed  synthengine.o osc.o lfo.o filter.o env.o echo.o osctab.o lfotab.o coeff.o envtab.o sdl.o

g++ -O3 -c osctab.c        -o osctab.o
g++ -O3 -c lfotab.c        -o lfotab.o
g++ -O3 -c coeff.c         -o coeff.o
g++ -O3 -c envtab.c        -o envtab.o
g++ -O3 -c synthengine.cpp -o synthengine.o
g++ -O3 -c osc.cpp         -o osc.o
g++ -O3 -c lfo.cpp         -o lfo.o
g++ -O3 -c filter.cpp      -o filter.o
g++ -O3 -c env.cpp         -o env.o
g++ -O3 -c echo.cpp        -o echo.o
g++ -O3 -c sdl.cpp         -o sdl.o
g++ -O3 -c main-float.cpp        -o main.o
g++ -O3 -c bench-float.cpp       -o bench.o
g++ -O3   main.o  -o main-float   synthengine.o osc.o lfo.o filter.o env.o echo.o osctab.o lfotab.o coeff.o envtab.o sdl.o
g++ -O3   bench.o -o bench-float  synthengine.o osc.o lfo.o filter.o env.o echo.o osctab.o lfotab.o coeff.o envtab.o sdl.o


# g++ -DFIXED -c test.cpp -o test.o && g++ -DFIXED synthengine.o osc.o lfo.o test.o filter.o env.o echo.o osctab.o lfotab.o coeff.o envtab.o sdl.o && ./a.out
