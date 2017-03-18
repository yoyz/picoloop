#include "LgptsamplerMachine.h"

#define SAM 256

LgptsamplerMachine::LgptsamplerMachine()
{
  float fi;
  int   i;

  DPRINTF("LgptsamplerMachine::LgptsamplerMachine()\n");  
  cutoff=125;
  resonance=10;
  note=0;
  detune=0;
  //buffer_lgpt=0;
  buffer=0;
  buffer16=0;
  index=0;
}



LgptsamplerMachine::~LgptsamplerMachine()
{
  DPRINTF("LgptsamplerMachine::~LgptsamplerMachine()\n");
}

// PicodrumVCO & LgptsamplerMachine::getPicodrumVCO()
// {
//   return vco;
// }



void LgptsamplerMachine::init()
{
  //if (buffer_lgpt==0)
  //{     
      //buffer_lgpt = (Sint16*)malloc(sizeof(Sint16)*SAM);
  //}
  printf("lgpt init\n");
  if (buffer==0)
    buffer=(lgptfixed*)malloc(sizeof(fixed)*1024);
  if (buffer16==0)
    buffer16=(int16_t*)malloc(sizeof(int16_t)*1024);
  index=0; // use by this->tick()


  //printf("buffer   : 0x%08.8X\n",buffer);
  //printf("buffer16 : 0x%08.8X\n",buffer16);
  
  // adsr_amp.init();

  // adsr_amp.setInput(&vco); 

  // filter.init();
  sample_num=0;


  //this->reset();
  // this->getADSRAmp().init();

  // this->getPicodrumVCO().init();
  // this->getPicodrumVCO().setNoteDetune(0,0);      
  //this->getPicodrumVCO().setSynthFreq(0);      
  //this->getADSRAmp().setNoteADSR(0);

  note=0;
  detune=0;

  //DEBSystem DEB;
  //SampleInstrument SI;
  SamplePool * SP=SamplePool::GetInstance();
  SyncMaster * SM=SyncMaster::GetInstance();
  int argc;
  char ** argv;
  DEB.Boot(argc,argv);

  //Path::SetAlias("samples","samples") ;
  //SP->Load();
  SP->loadSample("samples/70_SUREG_B.wav");
  SP->loadSample("samples/808-cowbell.wav");
  SM->SetTempo(120);
  SI.Init();
  SI.AssignSample(0);
  /*
  printf("<<<<SP.GetNameListSize()=%d>>>>>\n",SP->GetNameListSize());
  int i;
  for (i=0;i<SP->GetNameListSize();i++)
    {
      char ** tab_str=SP->GetNameList();
      printf("%s\n",tab_str[i]);
    }
  */

  afterinit=1;
}

int LgptsamplerMachine::checkI(int what,int val)
{
  switch (what)
    {
    case OSC1_TYPE:
      if (val<0) return 0;
      if (val>PICO_WAVETABLE_SIZE-2) return PICO_WAVETABLE_SIZE-1;
      return val;
      break;

    case OSC2_TYPE:
      if (val<0) return 0;
      if (val>PICO_WAVETABLE_SIZE-2) return PICO_WAVETABLE_SIZE-1;
      return val;
      break;

    case FILTER1_TYPE:
      if (val<0) return 0;
      if (val>=FILTER_TYPE_SIZE-1) return FILTER_TYPE_SIZE-1;
      return val;
      break;

    case FILTER1_ALGO:
      if (val<0) return 0;
      if (val>=FILTER_ALGO_SIZE-1) return FILTER_ALGO_SIZE-1;
      return val;
      break;

    default:
      if (val<0)   return 0;
      if (val>127) return 127;
      DPRINTF("WARNING: LgptsamplerMachine::checkI(%d,%d)\n",what,val);
      return val;
      break;      
    }
}


int LgptsamplerMachine::getI(int what)
{

  //if (what==NOTE_ON) return this->getADSRAmp().getNoteOn();
}

void LgptsamplerMachine::setF(int what,float val)
{
  //  if (what==OSC1_FREQ)           this->getPicodrumVCO().setSynthFreq(val);
  //if (what==LFO1_FREQ)           this->getPicodrumVCO().setLfoSpeed(val);
}


void LgptsamplerMachine::setI(int what,int val)
{
  float f_val_cutoff;
  float f_val_resonance;
  int noteShift=0;

  if (what==NOTE_ON && val==1) 
    {
      //SI.Start(0,72,true);
      SI.AssignSample(osc1_type%2);
      SI.Start(0,note,true);
      
      SI.ProcessCommand(0,SIP_FILTCUTOFF,0x0000+cutoff);
      SI.ProcessCommand(0,MAKE_FOURCC('F','R','E','S'),0x0000+resonance);
      // SI.ProcessCommand(0,MAKE_FOURCC('L','S','T','A'),lfo1_freq);
      // SI.ProcessCommand(0,MAKE_FOURCC('L','L','E','N'),lfo1_depth);
      // SI.ProcessCommand(0,MAKE_FOURCC('L','M','O','D'),osc1_type%4);
      // SI.ProcessCommand(0,MAKE_FOURCC('S','T','R','T'),pb_freq);
      // SI.ProcessCommand(0,MAKE_FOURCC('E','N','D','_'),pb_depth);

      // this->getPicodrumVCO().setNoteDetune(note+noteShift,detune);
      // this->getADSRAmp().reset();
      // this->getPicodrumVCO().reset();
      // this->getPicodrumVCO().setPicodrumVCOPhase(phase);
      // this->getADSRAmp().setNoteOn(); 
    }
  if (what==NOTE_ON && val==0) 
    { 
      //this->getADSRAmp().setNoteOff(); 
    }

  if (what==NOTE1)           note=val;

  //if (what==OSC1_FREQ)           this->getPicodrumVCO().setSynthFreq(val);
  if (what==OSC1_TYPE)           osc1_type=val;
  if (what==OSC2_TYPE)           osc2_type=val;
  // if (what==OSC2_TYPE)           { this->getPicodrumVCO().setOscillator(1,val); osc2_type=val; }

  // if (what==OSC12_MIX)           this->getPicodrumVCO().setPicodrumVCOMix(val);

  // if (what==LFO1_DEPTH)          this->getPicodrumVCO().setLfoDepth(val);
  // if (what==LFO_TYPE)            this->getPicodrumVCO().setLfoType(val);

  // if (what==PITCHBEND_DEPTH)     this->getPicodrumVCO().setPitchBendDepth(val);
  // if (what==PITCHBEND_SPEED)     this->getPicodrumVCO().setPitchBendSpeed(val);

  // if (what==OSC1_PHASE)          { phase=val; this->getPicodrumVCO().setPicodrumVCOPhase(val); }


  if (what==LFO1_FREQ)           lfo1_freq=val;
  if (what==LFO1_DEPTH)          lfo1_depth=val;
  if (what==PITCHBEND_DEPTH)     pb_depth=val;
  if (what==PITCHBEND_SPEED)     pb_freq=val;


  // if (what==ADSR_ENV0_ATTACK)    this->getADSRAmp().setAttack(val);
  // if (what==ADSR_ENV0_RELEASE)   this->getADSRAmp().setRelease(val);


  
   if (what==FILTER1_CUTOFF)      
     cutoff=val;

   if (what==FILTER1_RESONANCE)         
     resonance=val;

  // if (what==FILTER1_TYPE)     filter.setFilterType(val);
  // if (what==FILTER1_ALGO)     filter.setFilterAlgo(val);

  
}


// PicodrumADSR & LgptsamplerMachine::getADSRAmp()
// {
//   return adsr_amp;
// }




// PicodrumVCO & LgptsamplerMachine::getPicodrumVCO()
// {
//   //  DPRINTF("LgptsamplerMachine::getPicodrumVCO() this=0x%08.8X\n",this);
//   //  return vco_pointer;
//   return vco;
// }

//Biquad & LgptsamplerMachine::getBiquad()
//{
//  return bq;
//}

const char * LgptsamplerMachine::getMachineParamCharStar(int machineParam,int paramValue)
{
  static const char * str_null       = "NULL ";

  static const char * str_sine       = "SINE ";
  static const char * str_saw        = "SAW  ";
  static const char * str_pulse      = "PULSE";
  static const char * str_trgl       = "TRGL ";

  static const char * str_smsine     = "SSIN ";
  static const char * str_smsaw      = "SSAW ";
  static const char * str_smpulse    = "SPULS";
  static const char * str_smtrgl     = "STRGL";

  static const char * str_lfsr_noise = "LFSR ";
  static const char * str_noise      = "NOISE";

  const        char * str_osc[PICO_WAVETABLE_SIZE];


  static const char * str_fltr_algo_nofilter = "NOFL";
  static const char * str_fltr_algo_biquad   = "BIQU";
  static const char * str_fltr_algo_amsynth  = "AMST";
  
  const        char * str_fltr_algo[FILTER_ALGO_SIZE];


  static const char * str_fltr_type_lp   = "LP";
  static const char * str_fltr_type_bp   = "BP";
  static const char * str_fltr_type_hp   = "HP";

  const        char * str_fltr_type[FILTER_TYPE_SIZE];

  static const char * str_lfo_type_lfo   = "PLFO";
  static const char * str_lfo_type_pb    = "PB";

  const        char * str_lfo_type[LFO_TYPE_SIZE];


  str_osc[PICO_WAVETABLE_SINE]        = str_sine;
  str_osc[PICO_WAVETABLE_SAW]         = str_saw;
  str_osc[PICO_WAVETABLE_PULSE]       = str_pulse;
  str_osc[PICO_WAVETABLE_TRGL]        = str_trgl;

  str_osc[PICO_WAVETABLE_SMSINE]      = str_smsine;
  str_osc[PICO_WAVETABLE_SMSAW]       = str_smsaw;
  str_osc[PICO_WAVETABLE_SMPULSE]     = str_smpulse;
  str_osc[PICO_WAVETABLE_SMTRGL]      = str_smtrgl;

  str_osc[PICO_WAVETABLE_NOISE]       = str_noise;
  str_osc[PICO_WAVETABLE_LFSRNOISE]   = str_lfsr_noise;

  str_fltr_algo[FILTER_ALGO_NOFILTER] = str_fltr_algo_nofilter;
  str_fltr_algo[FILTER_ALGO_BIQUAD]   = str_fltr_algo_biquad;
  str_fltr_algo[FILTER_ALGO_AMSYNTH]  = str_fltr_algo_amsynth;

  str_fltr_type[FILTER_TYPE_LP]       = str_fltr_type_lp;
  str_fltr_type[FILTER_TYPE_BP]       = str_fltr_type_bp;
  str_fltr_type[FILTER_TYPE_HP]       = str_fltr_type_hp;

  switch (machineParam)
    {
    case OSC1_TYPE:
      return str_osc[this->checkI(OSC1_TYPE,paramValue)];
    case OSC2_TYPE:
      return str_osc[this->checkI(OSC2_TYPE,paramValue)];;
     
    case FILTER1_ALGO:
      return str_fltr_algo[this->checkI(FILTER1_ALGO,paramValue)];

    case FILTER1_TYPE:
      return str_fltr_type[this->checkI(FILTER1_TYPE,paramValue)];

    }
  return str_null;
}




void LgptsamplerMachine::reset()
{
  sample_num=0;
  last_sample=0;
  // this->getADSRAmp().reset();
  // this->getPicodrumVCO().reset();
  //adsr_amp.reset();
}


Sint16 LgptsamplerMachine::tick()
{
  float  f_in;
  float  f_out;
  Sint16 s_in;
  Sint16 s_out;
  int        i;

  // there is something wrong here, need to debug it
  //  return filter.process(adsr_amp.tick());
  //return adsr_amp.tick();
  if (index>=SAM | index<0)
    index=0;

  if (index==0)
    {
      SI.Render(0,buffer,SAM/2,true);
      i=0;
      for (i=0;i<SAM;i++)
	buffer[i]=fp2i(buffer[i]);
      
      for (i=0;i<SAM;i++)
	buffer16[i]=buffer[i];
    }
   
  //     while (i<SAM)
  // 	{
  // 	  buffer_picodrum[i]=filter.process(buffer_picodrum[i]);
  // 	  i++;
  // 	}
  //   }
    
  s_out=buffer16[index];
  
  index++;
  // sample_num++;
  return s_out;
}
