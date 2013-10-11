#include "SineOscillator.h"

using namespace std;


SineOscillator::SineOscillator() 
{
  frequency=440;
  amplitude=127;
  sample_num=0;
  printf("SineOscillator::SineOscillator()\n");
  if(1)
    {
      //printf("Malloc sine\n");
      //table = (Sint16*)malloc(sizeof(Sint16)*8192);
      table=NULL;
    }
  table_fill=0;
}

SineOscillator::~SineOscillator()
{
  printf("SineOscillator::~SineOscillator()\n");
  if (table!=NULL)
    free(table);

}


Sint16 SineOscillator::tick()
{
  float  f;
  Uint16 u;
  Sint16 s;
  int debug=0;
  if (frequency==0) return(0);
  if (table==NULL)
    table=(Sint16*)malloc(sizeof(Sint16)*32768);

  if (debug) printf("Sint16 SineOscillator::tick()\n");
  sample_num++;

  //f=(sin((sample_num/3.14159)/22)*10000);  
  //f=(sin(((sample_num/3.14159)/22)/440*freq)*amp*210

  // Good one
  //f=(sin(((sample_num/3.14159)/22)/440*freq)*( amp*210));
  //f=(sin(((sample_num/3.14159)/22)/440*freq));

  // sin(XXX) => -1 <=> 1
  // note 440Hz
  //f=(sin(((sample_num/3.14159)/22)/440*frequency));


  if (sample_num % (int)(44100/frequency) <table_fill)
    {
      //    printf("hit\n");
      return table[sample_num % (int)(44100/frequency)];
    }


  f=sin((2*3.14159*sample_num*frequency)/44100);

  if (debug) printf("%f\n",f);

  // Amplification from -1 1 to -32000 32000
  f=(f*amplitude*255);
  //if (f < 0) { f=f*(amp*255); if ( f >=0 ) { f=std::numeric_limits<float>::min(); } }
  //if (f > 0) { f=f*(amp*255); if ( f <=0 ) { f=std::numeric_limits<float>::max(); } }


  u=(Uint16)f;
  s=u;
  if (debug) printf("%f\n",f);
  if (debug) printf("%d\n",u);
  //u = u ^ 0xA00F;
  //if (sample_num % 16 == 0) { u = u & 0xCF0F; }

  s=u;

  table_fill=sample_num;
  table[table_fill]=s;
  //printf("table_fill : %d\n",table_fill);

  if (debug) printf("%d\n\n",u);
  //if (sample_num % 32 != 0) { return last_tick; }
  last_tick=u;
  //  exit(0);
  return s;
}
