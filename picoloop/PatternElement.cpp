using namespace std;

#include <string>
#include <iostream>
#include <sstream>
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

int PatternElement::getNote()
{
  return PatternElement::Note;
}

/*
char * PatterElement::getNoteCharStar()
{
  char * C0="C0";
  char * C0plus="C0+";
  char * D0="D0";
  char * D0plus="D0+";
  char * E0="E0";
  char * F0="F0";
  char * F0plus="F0+";
  char * G0="G0";
  char * G0plus="G0+";
  char * A0="A0";
  char * A0plus="A0+";
  char * B1="B1";

  char * C1plus="C1+";
  char * D1="D1";
  char * D1plus="D1+";
  char * E1="E1";
  char * F1="F1";
  char * F1plus="F1+";
  char * G1="G1";
  char * G1plus="G1+";
  char * A1="A1";
  char * A1plus="A1+";
  char * B2="B2";

  char * C2plus="C2+";
  char * D2="D2";
  char * D2+ ="D2+ ";
  char * E2="E2";
  char * F2="F2";
  char * F2plus="F2+";
  char * A2="A2";
  char * A2plus="A2+";
  char * G2="G2";
  char * G2plus="G2+";
  char * B3="B3";

  char * C3plus="C3+";
  char * D3="D3";
  char * D3plus="D3+";
  char * E3="E3";
  char * F3="F3";
  char * F3plus="F3+";
  char * G3="G3";
  char * G3plus="G3+";
  char * A3="A3";
  char * A3plus="A3+";
  char * B3="B3";

  char * C4plus="C4+";
  char * D4="D4";
  char * D4plus="D4+";
  char * E4="E4";
  char * F4="F4";
  char * F4plus="F4+";
  char * G4="G4";
  char * G4plus="G4+";
  char * A4="A4";
  char * A4plus="A4+";
  char * B4="B4";

  char * C5plus="C5+";
  char * D5="D5";
  char * D5plus="D5+";
  char * E5="E5";
  char * F5="F5";
  char * F5plus="F5+";
  char * G5="G5";
  char * G5plus="G5+";
  char * A5="A5";
  char * A5plus="A5+";
  char * B5="B5";

  char * C6plus="C6+";
  char * D6="D6";
  char * D6plus="D6+";
  char * E6="E6";
  char * F6="F6";
  char * F6plus="F6+";
  char * G6="G6";
  char * G6plus="G6+";
  char * A6="A6";
  char * A6plus="A6+";
  char * B6="B6";

  char * C7plus="C7+";
  char * D7="D7";
  char * D7plus="D7+";
}
*/


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
