#!/bin/bash
set -x
S=Filter ; 				g++ -D__SDL12__ -c ${S}.cpp -o ${S}.o
S=AMSynthLowPassFilter	;		g++ -D__SDL12__ -c ${S}.cpp -o ${S}.o
S=Biquad		;		g++ -D__SDL12__ -c ${S}.cpp -o ${S}.o
S=example_test		;		g++ -D__SDL12__ -c ${S}.cpp -o ${S}.o
g++ *.o -o test

