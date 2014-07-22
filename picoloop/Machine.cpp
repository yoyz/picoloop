#include "Machine.h"


//Machine::Machine() : sineosc(), sawosc(), fuzzypulseosc(), adsr()

//Machine::Machine()// : adsr(), vco()
//Machine::Machine() : adsr(), vco_osc()
Machine::Machine() : adsr_amp(), adsr_fltr(), vco(), bq(), bq2(), one_osc()
{
  printf("Machine::Machine()\n");  
  //  adsr=new ADSR();
  //  vco=new VCO();

  //  vco = new VCO();
  //vco_pointer = new VCO();
  /*
  s = &sineosc;
  s->setFreq(0);
  s->setAmplitude(127);

  */
  //  adsr.setOscillator(s);
  //  vco_pointer=&vco_osc;
  //  adsr.setVCO(vco_pointer);

  //  printf("&S:%d\n",S);
}



Machine::~Machine()
{
  printf("Machine::~Machine()\n");
}


void Machine::init()
{
  /*
  adsr_amp.setInput(&vco); 
  bq.setInput(&adsr_amp);
  adsr_fltr.setInput(&bq);
  */
  //adsr_fltr.setInput(&vco);
  //bq.setInput(&adsr_fltr);
  //adsr_amp.setInput(&bq); 

  bq.reset();
  adsr_amp.init();
  adsr_fltr.init();

  adsr_amp.setInput(&vco); 
  adsr_fltr.setInput(&one_osc);

  one_osc.setFreq(100);
  one_osc.init();
  one_osc.reset();
  //adsr_fltr.setInput(bq
  bq.setBiquad(0, 0.2, 0.5, 0.1);
  sample_num=0;

  bq2.reset();
  bq2.setBiquad(0, 0.5, 0.05, 0.0);
  bq2.calcBiquad();
}


/*
void Machine::setOscillator(SineOscillator NS)
{
  S=NS;
}
*/

ADSR & Machine::getADSRAmp()
{
  return adsr_amp;
}


ADSR & Machine::getADSRFltr()
{
  return adsr_fltr;
}


VCO & Machine::getVCO()
{
  //  printf("Machine::getVCO() this=0x%08.8X\n",this);
  //  return vco_pointer;
  return vco;
}

Biquad & Machine::getBiquad()
{
  return bq;
}

void Machine::reset()
{
  sample_num=0;
}

/*
void Machine::setSynthFreq(int sfreq)
{
  freq=sfreq;
  vco->setFreq(sfreq);
  //adsr.setOscillator(s);
  adsr.setVCO(vco);
}
*/

/*

void Machine::setSynthFreq(int sfreq)
{
  freq=sfreq;
  s->setFreq(sfreq);
  adsr.setOscillator(s);
}


Oscillator * Machine::getOscillator()
{
  return s;
}

void Machine::setSineOsc()
{
  s=&sineosc;
  s->setFreq(freq);
  adsr.setOscillator(s);
}


void Machine::setSawOsc()
{
  s=&sawosc;
  s->setFreq(freq);
  adsr.setOscillator(s);
}

void Machine::setFuzzyPulseOsc()
{
  s=&fuzzypulseosc;
  s->setFreq(freq);
  adsr.setOscillator(s);
}
*/

int Machine::tick()
{
  //if (&s==0) { printf("sine:%d\n",sineosc.tick()); exit(0);  }
  //  return s->tick();
  float  f_in;
  float  f_out;

  float  f_Fc;
  float  f_Q;

  Sint16 s_in;
  Sint16 s_out;

  Sint16 s_test;
  //int    num=1024;
  int    num=1024;
  int    i;
  //s_out=adsr_fltr.tick();
  //s_out=adsr_amp.tick();

  sample_num++;
  
  s_in=adsr_amp.tick();
  s_in=s_in/4;

  //if (sample_num==num)
  if (sample_num==num)
    {
      for (i=0;i<num;i++)
	adsr_fltr.tick();

      s_test=adsr_fltr.tick();

      f_Fc=bq.getFc()*s_test;
      f_Fc=f_Fc/16384;

      f_Q=bq.getQ()*s_test;
      f_Q=f_Q/16384;
      
      //bq.reset();
      //if (f_Fc<0.01)
      if (1)
	{
	  bq.setFc(f_Fc);
	  //if (f_Q<0.01)
	  bq.setQ(f_Q);
	  bq.calcBiquad();
	  //bq.setBiquad(0,f_Fc,f_Q,0.0);
	}

      if (0)
	{
	  printf("s_test:%d\n",s_test);
	  printf("Fc:%f\n",bq.getFc());
	  printf("Q:%f\n",bq.getQ());
	  printf("f_Fc:%f\n",f_Fc);
	  printf("f_Q:%f\n",f_Q);
	}


      //bq.setQ((float)((bq.getQ()*s_test))/16384);
      //sample_num=0;
    }


  if (sample_num<=num)
    {
      s_out=bq.process(s_in);
    }
  

  if (sample_num>num &&
      sample_num < num+128)
    {
      s_out=(last_sample+bq.process(s_in))/2;
      //s_out=bq.process(s_in);
      
    }


  if (sample_num>=num+128)
    {
      s_out=bq.process(s_in);
      sample_num=0;
    }

  
  
  //FILTER
  //s_in=s_in/4;
  //s_in=s_in/2;


  //s_out=bq.process(s_in);

  //if (1) printf("s_in:%d s_out:%d\n",s_in,s_out);
  //return s_in;  
  last_sample=s_out;
  //return bq2.process(s_out);
  return s_out;

  //  s_out=bq.process(s_out);
  //s_out=bq.process(s_out);



  /*
  s_in=adsr.tick();

  f_in=s_in;
  f_in=f_in/32768.0;
  f_out=bq.process(f_in);
  f_out=bq.process(f_out);
  f_out=bq.process(f_out);
  f_out=f_out*32768.0;
  s_out=f_out;
  return s_out;
  */

  /*    
  s_in=adsr.tick();
  f_in=s_in;
  //f_in=f_in/32768.0;
  f_in=f_in*0.0000305;
  f_out=bq.process(f_in);
  f_out=f_out*32768.0;
  s_out=f_out;
  return s_out;
  */

  //  printf("%d %d %f %f\n",s_in,s_out,f_in,f_out);
  
  
  //return adsr.tick();
}
