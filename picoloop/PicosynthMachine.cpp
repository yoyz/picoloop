#include "PicosynthMachine.h"


//PicosynthMachine::PicosynthMachine() : sineosc(), sawosc(), fuzzypulseosc(), adsr()

//PicosynthMachine::PicosynthMachine()// : adsr(), vco()
//PicosynthMachine::PicosynthMachine() : adsr(), vco_osc()
//PicosynthMachine::PicosynthMachine() : adsr_amp(), adsr_fltr(), vco(), bq(), bq2(), one_osc(), tanh_table(new Sint16[128])
PicosynthMachine::PicosynthMachine() : adsr_amp(), adsr_fltr(), vco(), bq(), bq2(), one_osc(), tanh_table(new Sint16[256])
{
  float fi;
  int   i;

  printf("PicosynthMachine::PicosynthMachine()\n");  
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
  for (i=0;i<256;i++)
    {
      fi=i;
      //fi=tanh(fi/256);
      fi=tanh(fi/128);
      //tanh_table[i/2]=fi*1024;
      tanh_table[i]=fi*1024;
      //printf("MACHINE tanh[%d]=%d\n",i,tanh_table[i]);
    }
}



PicosynthMachine::~PicosynthMachine()
{
  printf("PicosynthMachine::~PicosynthMachine()\n");
}


void PicosynthMachine::init()
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

  this->reset();
  this->getADSRAmp().init();
  this->getADSRFltr().init();
  this->getVCO().init();
  this->getVCO().setSynthFreq(0);      
  this->getADSRAmp().setNoteADSR(1);
  this->getADSRFltr().setNoteADSR(1);

}


/*
void PicosynthMachine::setOscillator(SineOscillator NS)
{
  S=NS;
}
*/

int PicosynthMachine::get(int what)
{
  //printf("void PicosynthMachine::get(int what,int val) Not fully implemented\n");
  if (what==NOTE_ON) return this->getADSRAmp().getNoteOn();
}

void PicosynthMachine::set(int what,int val)
{
  float f_val;

  if (what==NOTE_ON && val==1) 
    { 
      this->getBiquad().reset();
      this->getADSRAmp().reset();
      this->getADSRFltr().reset();
      this->getVCO().reset();
      this->getADSRAmp().setNoteOn(); 
      this->getADSRFltr().setNoteOn();   
    }
  if (what==NOTE_ON && val==0) 
    { 
      this->getADSRAmp().setNoteOff(); 
      this->getADSRFltr().setNoteOff(); 
    }

  if (what==OSC1_FREQ)           this->getVCO().setSynthFreq(val);
  if (what==OSC1_TYPE)           this->getVCO().setOscillator(0,val);
  if (what==OSC2_TYPE)           this->getVCO().setOscillator(1,val);

  if (what==ADSR_ENV0_ATTACK)    this->getADSRAmp().setAttack(val);
  if (what==ADSR_ENV0_RELEASE)   this->getADSRAmp().setRelease(val);

  if (what==ADSR_ENV1_ATTACK)    this->getADSRFltr().setAttack(val);
  if (what==ADSR_ENV1_RELEASE)   this->getADSRFltr().setRelease(val);

  if (what==FILTER1_CUTOFF)      
    { 
      f_val=val;
      this->getBiquad().setFc((f_val/256)+0.005);  
      this->getBiquad().calcBiquad(); 
  }
  if (what==FILTER1_RES)         
    { 
      f_val=val;
      this->getBiquad().setQ((f_val/8)+0.005);   
      this->getBiquad().calcBiquad(); 
    }

  
}


ADSR & PicosynthMachine::getADSRAmp()
{
  return adsr_amp;
}


ADSR & PicosynthMachine::getADSRFltr()
{
  return adsr_fltr;
}


VCO & PicosynthMachine::getVCO()
{
  //  printf("PicosynthMachine::getVCO() this=0x%08.8X\n",this);
  //  return vco_pointer;
  return vco;
}

Biquad & PicosynthMachine::getBiquad()
{
  return bq;
}

void PicosynthMachine::reset()
{
  sample_num=0;
  last_sample=0;
}

/*
void PicosynthMachine::setSynthFreq(int sfreq)
{
  freq=sfreq;
  vco->setFreq(sfreq);
  //adsr.setOscillator(s);
  adsr.setVCO(vco);
}
*/

/*

void PicosynthMachine::setSynthFreq(int sfreq)
{
  freq=sfreq;
  s->setFreq(sfreq);
  adsr.setOscillator(s);
}


Oscillator * PicosynthMachine::getOscillator()
{
  return s;
}

void PicosynthMachine::setSineOsc()
{
  s=&sineosc;
  s->setFreq(freq);
  adsr.setOscillator(s);
}


void PicosynthMachine::setSawOsc()
{
  s=&sawosc;
  s->setFreq(freq);
  adsr.setOscillator(s);
}

void PicosynthMachine::setFuzzyPulseOsc()
{
  s=&fuzzypulseosc;
  s->setFreq(freq);
  adsr.setOscillator(s);
}
*/

int PicosynthMachine::tick()
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

  Sint32 tmp1;
  Sint32 tmp2;
  Sint32 tmp3;
  Sint16 index;
  //int    num=1024;
  //int    num=2048;
  int    num=8192;
  //int      num=3192;
  //int    num=4096;
  int    i;
  //s_out=adsr_fltr.tick();
  //s_out=adsr_amp.tick();

  //  sample_num++;
  
  s_in=adsr_amp.tick();
  s_in=s_in/4;

  //if (sample_num==num)
  if (sample_num==0)
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


  

  if (sample_num>=0 &&
      //sample_num < 128)
      sample_num < 256)
    {

      /*
      index=sample_num;
      tmp1=(tanh_table[index]*bq.process(s_in))/1024;
      tmp2=(tanh_table[256-index]*last_sample)/1024;
      tmp3=(tmp1+tmp2)/2;
      s_out=tmp3;
      */

      /*
      index=sample_num;
      tmp1=(bq.process(s_in)*index)/256;
      tmp2=((last_sample)*(256-index))/256;
      tmp3=((tmp1+tmp2))/2;
      s_out=tmp3;
      */

      //printf("$$$$$$$$ index:%d last_sample:%d tmp1 :%d tmp2 :%d tmp3:%d s_out:%d\n",index,last_sample,tmp1,tmp2,tmp3,s_out);

      //printf("");
      //tmp3=((last_sample+tmp1))/2;

      //s_out=bq.process(s_in);

      /*
      if (sample_num<256)
	{
	s_out=(last_sample+bq.process(s_in))/2;
	}
      else
	s_out=bq.process(s_in);
      */
      s_out=bq.process(s_in);
      
    }


  if (sample_num>=256 &&
      sample_num<=num+256)
    {
      s_out=bq.process(s_in);
    }


  if (sample_num>=num+256)
    {
      //s_out=bq.process(s_in);
      //printf("$$$$$$$$$$$$$$reset\n");
      sample_num=-1;
    }

  
  
  //FILTER
  //s_in=s_in/4;
  //s_in=s_in/2;


  //s_out=bq.process(s_in);

  //if (1) printf("s_in:%d s_out:%d\n",s_in,s_out);
  //return s_in;  
  sample_num++;
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
