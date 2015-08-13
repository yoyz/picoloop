#include <stdio.h>
#include "drumsynth.h"

int main(int argc,char ** argv)
{
  //printf("%d\n",ds2wav("Snare.ds","S"));
  ds2wav(argv[1],argv[2]);
}
