#include "PicodrumMachine.h"


//PicodrumMachine::PicodrumMachine() : sineosc(), sawosc(), fuzzypulseosc(), adsr()

//PicodrumMachine::PicodrumMachine()// : adsr(), vco()
//PicodrumMachine::PicodrumMachine() : adsr(), vco_osc()
//PicodrumMachine::PicodrumMachine() : adsr_amp(), adsr_fltr(), vco(), bq(), bq2(), one_osc(), tanh_table(new Sint16[128])
PicodrumMachine::PicodrumMachine() : adsr_amp(), vco(), bq()
{
  float fi;
  int   i;

  printf("PicodrumMachine::PicodrumMachine()\n");  
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
  cutoff=125;
  resonance=10;
}



PicodrumMachine::~PicodrumMachine()
{
  printf("PicodrumMachine::~PicodrumMachine()\n");
}


void PicodrumMachine::init()
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

  adsr_amp.setInput(&vco); 

  //adsr_fltr.setInput(bq
  bq.setBiquad(0, 0.2, 0.5, 0.1);
  sample_num=0;


  this->reset();
  this->getADSRAmp().init();

  this->getVCO().init();
  this->getVCO().setSynthFreq(0);      
  this->getADSRAmp().setNoteADSR(0);
}


/*
void PicodrumMachine::setOscillator(SineOscillator NS)
{
  S=NS;
}
*/

int PicodrumMachine::get(int what)
{
  //printf("void PicodrumMachine::get(int what,int val) Not fully implemented\n");
  if (what==NOTE_ON) return this->getADSRAmp().getNoteOn();
}

void PicodrumMachine::set(int what,int val)
{
  float f_val_cutoff;
  float f_val_resonance;

  if (what==NOTE_ON && val==1) 
    { 
      this->getBiquad().reset();
      this->getADSRAmp().reset();
      this->getVCO().reset();
      this->getADSRAmp().setNoteOn(); 
    }
  if (what==NOTE_ON && val==0) 
    { 
      this->getADSRAmp().setNoteOff(); 
    }

  if (what==OSC1_FREQ)           this->getVCO().setSynthFreq(val);
  if (what==OSC1_TYPE)           this->getVCO().setOscillator(0,val);
  if (what==OSC2_TYPE)           this->getVCO().setOscillator(1,val);

  if (what==OSC12_MIX)           this->getVCO().setVCOMix(val);

  if (what==ADSR_ENV0_ATTACK)    this->getADSRAmp().setAttack(val);
  if (what==ADSR_ENV0_RELEASE)   this->getADSRAmp().setRelease(val);



  if (what==FILTER1_CUTOFF)      
    { 
      f_val_cutoff=val;
      f_val_resonance=resonance;
      cutoff=val;
      this->getBiquad().setBiquad(0, (f_val_cutoff/256)-0.005, (f_val_resonance/8)+0.005, 0.0);
      //this->getBiquad().setFc((f_val/256)+0.005);  
      this->getBiquad().calcBiquad(); 
      this->getADSRAmp().reset();

  }
  if (what==FILTER1_RES)         
    { 
      f_val_cutoff=cutoff;
      f_val_resonance=val;
      resonance=val;
      //this->getBiquad().setQ((f_val/8)+0.005);   
      this->getBiquad().setBiquad(0, (f_val_cutoff/256)-0.005, (f_val_resonance/8)+0.005, 0.0);
      this->getBiquad().calcBiquad(); 
      this->getADSRAmp().reset();
    }

  
}


ADSR & PicodrumMachine::getADSRAmp()
{
  return adsr_amp;
}




VCO & PicodrumMachine::getVCO()
{
  //  printf("PicodrumMachine::getVCO() this=0x%08.8X\n",this);
  //  return vco_pointer;
  return vco;
}

Biquad & PicodrumMachine::getBiquad()
{
  return bq;
}

void PicodrumMachine::reset()
{
  sample_num=0;
  last_sample=0;
}

/*
void PicodrumMachine::setSynthFreq(int sfreq)
{
  freq=sfreq;
  vco->setFreq(sfreq);
  //adsr.setOscillator(s);
  adsr.setVCO(vco);
}
*/

/*

void PicodrumMachine::setSynthFreq(int sfreq)
{
  freq=sfreq;
  s->setFreq(sfreq);
  adsr.setOscillator(s);
}


Oscillator * PicodrumMachine::getOscillator()
{
  return s;
}

void PicodrumMachine::setSineOsc()
{
  s=&sineosc;
  s->setFreq(freq);
  adsr.setOscillator(s);
}


void PicodrumMachine::setSawOsc()
{
  s=&sawosc;
  s->setFreq(freq);
  adsr.setOscillator(s);
}

void PicodrumMachine::setFuzzyPulseOsc()
{
  s=&fuzzypulseosc;
  s->setFreq(freq);
  adsr.setOscillator(s);
}
*/

int PicodrumMachine::tick()
{
  //if (&s==0) { printf("sine:%d\n",sineosc.tick()); exit(0);  }
  //  return s->tick();
  float  f_in;
  float  f_out;
  Sint16 s_in;
  Sint16 s_out;


  s_in=adsr_amp.tick();
  s_in=s_in/2;
  //return s_in;

  //FILTER
  s_out=bq.process(s_in);
  return s_out;

}
