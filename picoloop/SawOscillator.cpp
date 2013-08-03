#include "SawOscillator.h"

using namespace std;


SawOscillator::SawOscillator()
{
  frequency=440;
  amplitude=127;
  sample_num=0;
}

SawOscillator::~SawOscillator()
{

}


Sint16 SawOscillator::tick()
{
  float  f;
  Uint16 u;
  Sint16 s;
  int debug=0;
  sample_num++;
  if (debug) printf("Sint16 SawOscillator::tick()\n");
  //f=(sin((sample_num/3.14159)/22)*10000);  
  //f=(sin(((sample_num/3.14159)/22)/440*freq)*amp*210

  // Good one
  //f=(sin(((sample_num/3.14159)/22)/440*freq)*( amp*210));
  //f=(sin(((sample_num/3.14159)/22)/440*freq));

  // sin(XXX) => -1 <=> 1
  // note 440Hz
  //f=(sin(((sample_num/3.14159)/22)/440*frequency));
  //f=(sample_num%100);

  //  s=( (sample_num % frequency)/frequency ) * amplitude;

  //f=(float)(frequency)/(sample_num-frequency+1);
  //f=(float)((sample_num % frequency)/frequency);
  //f=(float)((last_tick+frequency))/frequency;
  //f=(float)
  //if (debug) printf("f:%f\n",f);
  //  f=(float)(f*2)-1;
  //if (debug) printf("f:%f\n",f);
  //f=(float)f*amplitude*255;
  //if (debug) printf("f:%f\n",f);
  //s=f;
  u=(((last_tick+frequency))%(amplitude*255*2));
  s=u;
  if (debug) printf("u:%d\n",u);
  if (debug) printf("s:%d\n",s);
  // Amplification from -1 1 to -32000 32000
  //f=(f*amplitude*255);
  //if (f < 0) { f=f*(amp*255); if ( f >=0 ) { f=std::numeric_limits<float>::min(); } }
  //if (f > 0) { f=f*(amp*255); if ( f <=0 ) { f=std::numeric_limits<float>::max(); } }


  //u=(Uint16)f;
  //s=u;
  //  if (debug) printf("%f\n",f);
  //if (debug) printf("%d\n",u);
  //u = u ^ 0xA00F;
  //if (sample_num % 16 == 0) { u = u & 0xCF0F; }

  //  s=u;
  //if (debug) printf("%d\n\n",u);
  //if (sample_num % 32 != 0) { return last_tick; }
  last_tick=s;
  //  exit(0);
  return s;
}
