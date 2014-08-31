using namespace std;

#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include "PatternElement.h"

PatternElement::PatternElement()
{
  printf("PatternElement::PatternElement()\n");
  Note=0;
  Trig=false;
  Channel=0;
  vcomix=64;

  amp=127;

  amp_attack=0;
  amp_decay=127;
  amp_sustain=127;
  amp_release=8;

  fltr_attack=0;
  fltr_decay=127;
  fltr_sustain=127;
  fltr_release=127;



  fltr_cutoff=64;
  fltr_resonance=30;

  lfo_depth=0;
  lfo_speed=0;
    

  oscOneType=SAW;
  oscTwoType=PULSE;

  adsr_note=1;

  machine_type=0;

  trig_time=0;
}

PatternElement::~PatternElement()
{
  //printf("PatternElement::~PatternElement()\n");
}


void PatternElement::init()
{
  Note=0;
  Trig=false;
  Channel=0;
  vcomix=64;

  amp=127;

  amp_attack=0;
  amp_decay=127;
  amp_sustain=127;
  amp_release=8;

  fltr_attack=0;
  fltr_decay=127;
  fltr_sustain=127;
  fltr_release=8;



  fltr_cutoff=64;
  fltr_resonance=80;

  lfo_depth=0;
  lfo_speed=0;
    

  oscOneType=SAW;
  oscTwoType=PULSE;

  adsr_note=1;

  //printf("====PatternElement::init()====\n");
}

int PatternElement::getOscillatorOneType()
{
  return oscOneType;
}

int PatternElement::getOscillatorTwoType()
{
  return oscTwoType;
}

void PatternElement::setOscillatorOneType(int type)
{
  oscOneType=type%5;
}

void PatternElement::setOscillatorTwoType(int type)
{
  oscTwoType=type%5;
}


int PatternElement::getNote()
{
  return PatternElement::Note;
}



int PatternElement::checkSevenBitBoundarie(int val)
{
  if (val < 0)
    return 0;
  if (val > 127)
    return 127;
  return val;
}

void PatternElement::setVCOMix(int mix)
{
  vcomix=this->checkSevenBitBoundarie(mix);  
  printf("[vcomix:%d]\n",vcomix);
}

int PatternElement::getVCOMix()
{
  return vcomix;
}



int PatternElement::getAttack_amp()
{
  return amp_attack;
}

void PatternElement::setAttack_amp(int atk)
{
  amp_attack=this->checkSevenBitBoundarie(atk);  
}

int PatternElement::getDecay_amp()
{
  return amp_decay;
}

void PatternElement::setDecay_amp(int dcy)
{
  amp_decay=this->checkSevenBitBoundarie(dcy);
}


int PatternElement::getSustain_amp()
{
  return amp_sustain;
}

void PatternElement::setSustain_amp(int sus)
{
  amp_sustain=this->checkSevenBitBoundarie(sus);
}


int PatternElement::getRelease_amp()
{
  return amp_release;
}

void PatternElement::setRelease_amp(int rls)
{
  amp_release=this->checkSevenBitBoundarie(rls);
}





int PatternElement::getAttack_fltr()
{
  return fltr_attack;
}

void PatternElement::setAttack_fltr(int atk)
{
  fltr_attack=this->checkSevenBitBoundarie(atk);  
}

int PatternElement::getDecay_fltr()
{
  return fltr_decay;
}

void PatternElement::setDecay_fltr(int dcy)
{
  fltr_decay=this->checkSevenBitBoundarie(dcy);
}


int PatternElement::getSustain_fltr()
{
  return fltr_sustain;
}

void PatternElement::setSustain_fltr(int sus)
{
  fltr_sustain=this->checkSevenBitBoundarie(sus);
}


int PatternElement::getRelease_fltr()
{
  return fltr_release;
}

void PatternElement::setRelease_fltr(int rls)
{
  fltr_release=this->checkSevenBitBoundarie(rls);
}



int PatternElement::getNoteADSR()
{
  return adsr_note;
}

void PatternElement::setNoteADSR(int val)
{
  adsr_note=val;
}


int PatternElement::getMachineType()
{
  return machine_type;
}

void PatternElement::setMachineType(int val)
{
  if      (val>2) val=0;
  else if (val<0) val=2;
  machine_type=val;
}

int PatternElement::getTrigTime()
{
  return trig_time;
}

void PatternElement::setTrigTime(int val)
{
  trig_time=this->checkSevenBitBoundarie(val);
}





int PatternElement::getCutoff()
{
  return fltr_cutoff;
}

void PatternElement::setCutoff(int cut)
{
  fltr_cutoff=this->checkSevenBitBoundarie(cut);
}

int PatternElement::getResonance()
{
  return fltr_resonance;
}

void PatternElement::setResonance(int res)
{
  fltr_resonance=this->checkSevenBitBoundarie(res);
}



bool PatternElement::setNote(int N)
{
  Note=this->checkSevenBitBoundarie(N);
}

bool PatternElement::setTrig(bool T)
{
  return Trig=T;
}

bool PatternElement::getTrig()
{
  return Trig;
}

int PatternElement::getAmp()
{
  return amp;
}

void PatternElement::setAmp(int a)
{
  amp=this->checkSevenBitBoundarie(a);
}


int PatternElement::getLfoDepth()
{
  return lfo_depth;
}

void PatternElement::setLfoDepth(int val)
{
  lfo_depth=this->checkSevenBitBoundarie(val);
}

int PatternElement::getLfoSpeed()
{
  return lfo_speed;
}

void PatternElement::setLfoSpeed(int val)
{
  lfo_speed=this->checkSevenBitBoundarie(val);
}

void PatternElement::setPhaseOsc1(int val)
{
  phaseOsc1=this->checkSevenBitBoundarie(val);
}

int PatternElement::getPhaseOsc1()
{
  return phaseOsc1;
}



string PatternElement::getStr()
{
  stringstream ss;
  string s;
  ss << "Note:" << Note << "\tTrig:" << Trig << "\tChannel:" << Channel ;
  s=ss.str();
  return s;
    //cout << "Note:" << Note << " Trig:" << Trig << " Channel:" << Channel << "\n";
}

void PatternElement::print()
{
  cout << getStr() << "\n";
}


const char * PatternElement::getMachineTypeCharStar()
{
  static const char * str_picosynth   = "PSYNTH ";
  static const char * str_picodrum    = "PDRUM  ";
  static const char * str_opl2        = "OPL2 ";

  switch (machine_type)
    {
    case PICOSYNTH:  return str_picosynth;   break;
    case PICODRUM:   return str_picodrum;    break;
    case OPL2:       return str_opl2;        break; 
    }
  return str_picosynth;
}


const char * PatternElement::getOscOneTypeCharStar()
{
  static const char * str_sine  = "SINE ";
  static const char * str_saw   = "SAW  ";
  static const char * str_pulse = "PULSE";
  static const char * str_wtbl  = "WTBL";
  static const char * str_trgl  = "TRGL";
  static const char * str_noise = "NOISE";

  switch (oscOneType)
    {
    case SINE:  return str_sine;  break;
    case SAW:   return str_saw;   break; 
    case PULSE: return str_pulse; break;      
      //case WTBL:  return str_wtbl;  break;            
    case TRGL:  return str_trgl;  break;            
    case NOISE: return str_noise; break;            
    }
  return str_sine;
}


const char * PatternElement::getOscTwoTypeCharStar()
{
  static const char * str_sine  = "SINE ";
  static const char * str_saw   = "SAW  ";
  static const char * str_pulse = "PULSE";
  //char * str_wtbl  = "WTBL";
  //char * str_wtbl  = "WTBL";
  static const char * str_trgl  = "TRGL";
  static const char * str_noise = "NOISE";
  
  switch (oscTwoType)
    {
    case SINE:  return str_sine;  break;
    case SAW:   return str_saw;   break; 
    case PULSE: return str_pulse; break;
      //case WTBL:  return str_wtbl;  break;            
    case TRGL:  return str_trgl;  break;            
    case NOISE: return str_noise; break;                  
    }
  return str_sine;
}


const char * PatternElement::getNoteCharStar()
{
  static const char * ret=   "NUL";
  static const char * C0=    "C0 ";
  static const char * C0plus="C0+";
  static const char * D0=    "D0 ";
  static const char * D0plus="D0+";
  static const char * E0=    "E0 ";
  static const char * F0=    "F0 ";
  static const char * F0plus="F0+";
  static const char * G0=    "G0 ";
  static const char * G0plus="G0+";
  static const char * A0=    "A0 ";
  static const char * A0plus="A0+";
  static const char * B1=    "B1 ";
  
  static const char * C1=    "C1 ";
  static const char * C1plus="C1+";
  static const char * D1=    "D1 ";
  static const char * D1plus="D1+";
  static const char * E1=    "E1 ";
  static const char * F1=    "F1 ";
  static const char * F1plus="F1+";
  static const char * G1=    "G1 ";
  static const char * G1plus="G1+";
  static const char * A1=    "A1 ";
  static const char * A1plus="A1+";
  static const char * B2=    "B2 ";
  
  static const char * C2=    "C2 ";
  static const char * C2plus="C2+";
  static const char * D2=    "D2 ";
  static const char * D2plus="D2+";
  static const char * E2=    "E2 ";
  static const char * F2=    "F2 ";
  static const char * F2plus="F2+";
  static const char * A2=    "A2 ";
  static const char * A2plus="A2+";
  static const char * G2=    "G2 ";
  static const char * G2plus="G2+";
  static const char * B3=    "B3 ";
  
  static const char * C3=    "C3 ";
  static const char * C3plus="C3+";
  static const char * D3=    "D3 ";
  static const char * D3plus="D3+";
  static const char * E3=    "E3 ";
  static const char * F3=    "F3 ";
  static const char * F3plus="F3+";
  static const char * G3=    "G3 ";
  static const char * G3plus="G3+";
  static const char * A3=    "A3 ";
  static const char * A3plus="A3+";
  static const char * B4=    "B4 ";
  
  static const char * C4=    "C4 ";
  static const char * C4plus="C4+";
  static const char * D4=    "D4 ";
  static const char * D4plus="D4+";
  static const char * E4=    "E4 ";
  static const char * F4=    "F4 ";
  static const char * F4plus="F4+";
  static const char * G4=    "G4 ";
  static const char * G4plus="G4+";
  static const char * A4=    "A4 ";
  static const char * A4plus="A4+";
  static const char * B5=    "B5 ";
  
  static const char * C5=    "C5 ";
  static const char * C5plus="C5+";
  static const char * D5=    "D5 ";
  static const char * D5plus="D5+";
  static const char * E5=    "E5 ";
  static const char * F5=    "F5 ";
  static const char * F5plus="F5+";
  static const char * G5=    "G5 ";
  static const char * G5plus="G5+";
  static const char * A5=    "A5 ";
  static const char * A5plus="A5+";
  static const char * B6=    "B6 ";
  
  static const char * C6=    "C6 ";
  static const char * C6plus="C6+";
  static const char * D6=    "D6 ";
  static const char * D6plus="D6+";
  static const char * E6=    "E6 ";
  static const char * F6=    "F6 ";
  static const char * F6plus="F6+";
  static const char * G6=    "G6 ";
  static const char * G6plus="G6+";
  static const char * A6=    "A6 ";
  static const char * A6plus="A6+";
  static const char * B7=    "B7 ";
  
  static const char * C7=    "C7 ";
  static const char * C7plus="C7+";
  static const char * D7=    "D7 ";
  static const char * D7plus="D7+";
  
  switch(Note)
    {
    case 1:   return  C0;      break;
    case 2:   return  C0plus;  break;
    case 3:   return  D0;      break;
    case 4:   return  D0plus;  break;
    case 5:   return  E0;      break;
    case 6:   return  F0;      break;
    case 7:   return  F0plus;  break;
    case 8:   return  G0;      break;
    case 9:   return  G0plus;  break;
    case 10:  return  A0;      break;
    case 11:  return  A0plus;  break;
    case 12:  return  B1;      break;

    case 13:  return  C1;      break;
    case 14:  return  C1plus;  break;
    case 15:  return  D1;      break;
    case 16:  return  D1plus;  break;
    case 17:  return  E1;      break;
    case 18:  return  F1;      break;
    case 19:  return  F1plus;  break;
    case 20:  return  G1;      break;
    case 21:  return  G1plus;  break;
    case 22:  return  A1;      break;
    case 23:  return  A1plus;  break;
    case 24:  return  B2;      break;

    case 25:  return  C2;      break;
    case 26:  return  C2plus;  break;
    case 27:  return  D2;      break;
    case 28:  return  D2plus;  break;
    case 29:  return  E2;      break;
    case 30:  return  F2;      break;
    case 31:  return  F2plus;  break;
    case 32:  return  G2;      break;
    case 33:  return  G2plus;  break;
    case 34:  return  A2;      break;
    case 35:  return  A2plus;  break;
    case 36:  return  B3;      break;

    case 37:  return  C3;      break;
    case 38:  return  C3plus;  break;
    case 39:  return  D3;      break;
    case 40:  return  D3plus;  break;
    case 41:  return  E3;      break;
    case 42:  return  F3;      break;
    case 43:  return  F3plus;  break;
    case 44:  return  G3;      break;
    case 45:  return  G3plus;  break;
    case 46:  return  A3;      break;
    case 47:  return  A3plus;  break;
    case 48:  return  B4;      break;

    case 49:  return  C4;      break;
    case 50:  return  C4plus;  break;
    case 51:  return  D4;      break;
    case 52:  return  D4plus;  break;
    case 53:  return  E4;      break;
    case 54:  return  F4;      break;
    case 55:  return  F4plus;  break;
    case 56:  return  G4;      break;
    case 57:  return  G4plus;  break;
    case 58:  return  A4;      break;
    case 59:  return  A4plus;  break;
    case 60:  return  B5;      break;

    case 61:  return  C5;      break;
    case 62:  return  C5plus;  break;
    case 63:  return  D5;      break;
    case 64:  return  D5plus;  break;
    case 65:  return  E5;      break;
    case 66:  return  F5;      break;
    case 67:  return  F5plus;  break;
    case 68:  return  G5;      break;
    case 69:  return  G5plus;  break;
    case 70:  return  A5;      break;
    case 71:  return  A5plus;  break;
    case 72:  return  B6;      break;

    case 73:  return  C6;      break;
    case 74:  return  C6plus;  break;
    case 75:  return  D6;      break;
    case 76:  return  D6plus;  break;
    case 77:  return  E6;      break;
    case 78:  return  F6;      break;
    case 79:  return  F6plus;  break;
    case 80:  return  G6;      break;
    case 81:  return  G6plus;  break;
    case 82:  return  A6;      break;
    case 83:  return  A6plus;  break;
    case 84:  return  B7;      break;

    case 85:  return  C7;      break;
    case 86:  return  C7plus;  break;
    case 87:  return  D7;      break;
    case 88:  return  D7plus;  break;
    default:  return  ret;     break;
    }
  
}


float PatternElement::getNoteFreq()
{
  
  float note_hertz[]=
    {
      0.0,

      16.3	,	// 1  C-1
      17.3	,	// 2  C-1+
      18.3	,	// 3  D-1
      19.4	,	// 4  D-1+
      20.5	,	// 5  E-1
      21.8	,	// 6  F-1
      23.1	,	// 7  F-1+
      24.5	,	// 8  G-1
      26.0	,	// 9  G-1+
      27.5	,	// 10 A-1
      29.1	,	// 11 A-1+
      30.8	,	// 12 B-1

      32.7	,	// 13 C0
      34.6	,	// 14 C0+
      36.7	,	// 15 D0
      38.9	,	// 16 D0+
      41.2	,	// 17 E0
      43.6	,	// 18 F0
      46.2	,	// 19 F0+
      49.0	,	// 20 G0
      51.9	,	// 21 G0+
      55.0	,	// 22 A0
      58.0	,	// 23 A0+
      62	,	// 24 B0

      65	,	// 25 C1
      69	,	// 26 C1+
      74	,	// 27 D1
      78	,	// 28 D1+ 
      83	,	// 29 E1
      87	,	// 30 F1
      92.5	,	// 31 F1+
      98	,	// 32 A1
      104	,	// 33 A1+
      110	,	// 34 G1
      117       ,       // 35 G1+
      123	,	// 36 B1

      131	,	// 37 C2
      139	,	// 38 C2+
      147	,	// 39 D2
      156	,	// 40 D2+
      165	,	// 41 E2
      175	,	// 42 F2
      185	,	// 43 F2+
      196	,	// 44 G2
      208	,	// 45 G2+
      220	,	// 46 A2
      233	,	// 47 A2+
      247	,	// 48 B2

      262	,	// 49 C3
      277	,	// 50 C3+
      294	,	// 51 D3
      311	,	// 52 D3+
      330	,	// 53 E3
      349	,	// 54 F3
      370	,	// 55 F3+
      392	,	// 56 G3
      415	,	// 57 G3+
      440	,	// 58 A3
      466	,	// 59 A3+
      494	,	// 60 B3

      523	,	// 61 C4
      554	,	// 62 C4+
      587	,	// 63 D4
      622	,	// 64 D4+
      659	,	// 65 E4
      698.5	,	// 66 F4
      740	,	// 67 F4+
      784	,	// 68 G4
      831	,	// 69 G4+
      880	,	// 70 A4
      932	,	// 71 A4+
      988	,	// 72 B4

      1046.5	,	// 73 C5
      1109	,	// 74 C5+
      1175      ,	// 75 D5
      1244.5	,	// 76 D5+
      1318.5	,	// 77 E5
      1397	,	// 78 F5
      1480	,	// 79 F5+
      1568	,	// 80 G5
      1661	,	// 81 G5+
      1760	,	// 82 A5
      1865	,	// 83 A5+
      1975	,	// 84 B5

      2093	,	// 85 C6
      2217	,	// 86 C6+
      2349	,	// 87 D6
      2489	,	// 88 D6+
      2637	,	// 89 E6
      2794	,	// 90 F6
      2960	,	// 91 F6+
      3136	,	// 92 G6
      3322	,	// 93 G6+
      3520	,	// 94 A6
      3729	,	// 95 A6+
      3951	,	// 96 B6

      4186	,	// 97  C7
      4435	,	// 98  C7+
      4698	,	// 99  D7
      4978	,	// 100 D7+
      5274	,	// 101 E7
      5588	,	// 102 F7
      5920	,	// 103 F7+
      6272	,	// 104 G7
      6645	,	// 105 G7+
      7040	,	// 106 A7
      7458	,	// 107 A7+
      7902	,	// 108 B7


    };


  return note_hertz[PatternElement::Note];
}
