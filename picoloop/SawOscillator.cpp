#include "SawOscillator.h"

using namespace std;


SawOscillator::SawOscillator()
{
  frequency=440;
  amplitude=127;
  sample_num=0;
  phase=1;
  s=0;
  u=0;
  last_u=0;
}

SawOscillator::~SawOscillator()
{

}


Sint16 SawOscillator::tick()
{
  float  f;
  int debug=0;
  int ampmax=amplitude*255*2;
  sample_num++;
  if (debug) printf("Sint16 SawOscillator::tick()\n");

  u=((last_u+frequency))%ampmax;
  if (last_u>u)
    phase=phase*-1;

  //  if (u<frequency)
  //    phase=phase*-1;

  s=u*phase;
  //  if (phase>=0 && last_tick>s)      
  //    { 
  //      printf("Phase change: %d\n",phase);
  //      printf("sample_num: %d\n",sample_num);
  //      phase=phase*-1; 
  //    }

  if (debug) printf("u:%d\n",u);
  if (debug) printf("s:%d\n",s);

  last_tick=s;
  last_u=u;
  return s;
}
