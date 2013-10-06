using namespace std;

#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include "PatternElement.h"

PatternElement::PatternElement()
{
  Note=0;
  Trig=false;
  Channel=0;
  attack=0;
  decay=127;
  sustain=127;
  release=8;
}

PatternElement::~PatternElement()
{
  //printf("PatternElement::~PatternElement()\n");
}

int PatternElement::getNote()
{
  return PatternElement::Note;
}


char * PatternElement::getNoteCharStar()
{
  char * ret=   "NUL";
  char * C0=    "C0 ";
  char * C0plus="C0+";
  char * D0=    "D0 ";
  char * D0plus="D0+";
  char * E0=    "E0 ";
  char * F0=    "F0 ";
  char * F0plus="F0+";
  char * G0=    "G0 ";
  char * G0plus="G0+";
  char * A0=    "A0 ";
  char * A0plus="A0+";
  char * B1=    "B1 ";

  char * C1=    "C1 ";
  char * C1plus="C1+";
  char * D1=    "D1 ";
  char * D1plus="D1+";
  char * E1=    "E1 ";
  char * F1=    "F1 ";
  char * F1plus="F1+";
  char * G1=    "G1 ";
  char * G1plus="G1+";
  char * A1=    "A1 ";
  char * A1plus="A1+";
  char * B2=    "B2 ";

  char * C2=    "C2 ";
  char * C2plus="C2+";
  char * D2=    "D2 ";
  char * D2plus="D2+";
  char * E2=    "E2 ";
  char * F2=    "F2 ";
  char * F2plus="F2+";
  char * A2=    "A2 ";
  char * A2plus="A2+";
  char * G2=    "G2 ";
  char * G2plus="G2+";
  char * B3=    "B3 ";

  char * C3=    "C3 ";
  char * C3plus="C3+";
  char * D3=    "D3 ";
  char * D3plus="D3+";
  char * E3=    "E3 ";
  char * F3=    "F3 ";
  char * F3plus="F3+";
  char * G3=    "G3 ";
  char * G3plus="G3+";
  char * A3=    "A3 ";
  char * A3plus="A3+";
  char * B4=    "B4 ";

  char * C4=    "C4 ";
  char * C4plus="C4+";
  char * D4=    "D4 ";
  char * D4plus="D4+";
  char * E4=    "E4 ";
  char * F4=    "F4 ";
  char * F4plus="F4+";
  char * G4=    "G4 ";
  char * G4plus="G4+";
  char * A4=    "A4 ";
  char * A4plus="A4+";
  char * B5=    "B5 ";

  char * C5=    "C5 ";
  char * C5plus="C5+";
  char * D5=    "D5 ";
  char * D5plus="D5+";
  char * E5=    "E5 ";
  char * F5=    "F5 ";
  char * F5plus="F5+";
  char * G5=    "G5 ";
  char * G5plus="G5+";
  char * A5=    "A5 ";
  char * A5plus="A5+";
  char * B6=    "B6 ";

  char * C6=    "C6 ";
  char * C6plus="C6+";
  char * D6=    "D6 ";
  char * D6plus="D6+";
  char * E6=    "E6 ";
  char * F6=    "F6 ";
  char * F6plus="F6+";
  char * G6=    "G6 ";
  char * G6plus="G6+";
  char * A6=    "A6 ";
  char * A6plus="A6+";
  char * B7=    "B7 ";

  char * C7=    "C7 ";
  char * C7plus="C7+";
  char * D7=    "D7 ";
  char * D7plus="D7+";

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

      27.5	,	// 1  C0
      29.1353	,	// 2  C0+
      30.8677	,	// 3  D0
      32.7032	,	// 4  D0+
      34.6479	,	// 5  E0
      36.7081	,	// 6  F0
      38.8909	,	// 7  F0+
      41.2035	,	// 8  G0
      43.6536	,	// 9  G0+
      46.2493	,	// 10 A0
      48.9995	,	// 11 A0+
      51.9130	,	// 12 B1

      55	,	// 13 C1
      58.2705	,	// 14 C1+
      61.7354	,	// 15 D1
      65.4064	,	// 16 D1+
      69.2957	,	// 17 E1
      73.4162	,	// 18 F1
      77.7817	,	// 19 F1+
      82.4069	,	// 20 G1
      87.3071	,	// 21 G1+
      92.4986	,	// 22 A1
      97.9989	,	// 23 A1+
      103.826	,	// 24 B2

      110	,	// 25 C2
      116.541	,	// 26 C2+
      123.471	,	// 27 D2
      130.813	,	// 28 D2+ 
      138.591	,	// 29 E2
      146.832	,	// 30 F2
      155.563	,	// 31 F2+
      164.814	,	// 32 A2
      174.614	,	// 33 A2+
      184.997	,	// 34 G2
      195.998	,	// 35 G2+
      207.652	,	// 36 B3

      220	,	// 37 C3
      233.082	,	// 38 C3+
      246.942	,	// 39 D3
      261.626	,	// 40 D3+
      277.183	,	// 41 E3
      293.665	,	// 42 F3
      311.127	,	// 43 F3+
      329.628	,	// 44 G3
      349.228	,	// 45 G3+
      369.994	,	// 46 A3
      391.995	,	// 47 A3+
      415.305	,	// 48 B3

      440	,	// 49 C4
      466.164	,	// 50 C4+
      493.883	,	// 51 D4
      523.251	,	// 52 D4+
      554.365	,	// 53 E4
      587.33	,	// 54 F4
      622.254	,	// 55 F4+
      659.255	,	// 56 G4
      698.456	,	// 57 G4+
      739.989	,	// 58 A4
      783.991	,	// 59 A4+
      830.609	,	// 60 B4

      880	,	// 61 C5
      932.328	,	// 62 C5+
      987.767	,	// 63 D5
      1046.5	,	// 64 D5+
      1108.73	,	// 65 E5
      1174.66	,	// 66 F5
      1244.51	,	// 67 F5+
      1318.51	,	// 68 G5
      1396.91	,	// 69 G5+
      1479.98	,	// 70 A5
      1567.98	,	// 71 A5+
      1661.22	,	// 72 B5

      1760	,	// 73 C6
      1864.66	,	// 74 C6+
      1975.53	,	// 75 D6
      2093	,	// 76 D6+
      2217.46	,	// 77 E6
      2349.32	,	// 78 F6
      2489.02	,	// 79 F6+
      2637.02	,	// 80 G6
      2793.83	,	// 81 G6+
      2959.96	,	// 82 A6
      3135.96	,	// 83 A6+
      3322.44	,	// 84 B6

      3520	,	// 85 C7
      3729.31	,	// 86 C7+
      3951.07	,	// 87 D7
      4186.01	,	// 88 D7+
    };


  return note_hertz[PatternElement::Note];
}

int PatternElement::checkSevenBitBoundarie(int val)
{
  if (val < 0)
    return 0;
  if (val > 127)
    return 127;
  return val;
}

int PatternElement::getAttack()
{
  return attack;
}

void PatternElement::setAttack(int atk)
{
  attack=this->checkSevenBitBoundarie(atk);
  
}

int PatternElement::getDecay()
{
  return decay;
}

void PatternElement::setDecay(int dcy)
{
  decay=this->checkSevenBitBoundarie(dcy);
}


int PatternElement::getSustain()
{
  return sustain;
}

void PatternElement::setSustain(int sus)
{
  sustain=this->checkSevenBitBoundarie(sus);
}


int PatternElement::getRelease()
{
  return release;
}

void PatternElement::setRelease(int rls)
{
  release=this->checkSevenBitBoundarie(rls);
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
