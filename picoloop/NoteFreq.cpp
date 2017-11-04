#include "NoteFreq.h"

NoteFreq::NoteFreq()
{
  wtjTable=0;
}

NoteFreq::~NoteFreq()
{
}


NoteFreq& NoteFreq::getInstance()
{
  static NoteFreq instance;
  return instance;
}


void NoteFreq::init()
{
  int i=0;
  int j=0;

  float freq;
  float wtjump_freq;
  float detune;
  float wtjump_detune;
  float wtjump_detune_1;
  float wtjump_detune_2;
  float value;

  const int   shift=8;
  const int   wtshift=1<<shift;

  if (wtjTable!=0)
    return;

  wtjTable=(Sint32**)malloc(sizeof(Sint32*)*109);
  for (i=0;i<109;i++)
    wtjTable[i]=(Sint32*)malloc(sizeof(Sint32)*128);

  for (i=0;i<109;i++)
    for (j=0;j<128;j++)
      {
	freq=this->getFNoteFreq(i);

	wtjump_freq=(freq*WAVETABLE_SIZE*(wtshift))/DEFAULTFREQ;	
	wtjump_detune_1=(((this->getFNoteFreq(i+1)*((WAVETABLE_SIZE*(wtshift))/DEFAULTFREQ))))*j;
	wtjump_detune_2=(((this->getFNoteFreq(i-1)*((WAVETABLE_SIZE*(wtshift))/DEFAULTFREQ))))*(128-j);
	wtjump_detune=(wtjump_detune_1-wtjump_detune_2)/4096;
	wtjTable[i][j]=wtjump_freq+wtjump_detune;
      }
}

float NoteFreq::getFNoteFreq(int note)
{
  
  float note_hertz[]=
    {
      8.0       ,       // 0  N/A
      16.35     ,	// 1  C-1
      17.32     ,	// 2  C-1+
      18.35     ,	// 3  D-1
      19.45     ,	// 4  D-1+
      20.60     ,	// 5  E-1
      21.83     ,	// 6  F-1
      23.12     ,	// 7  F-1+
      24.50     ,	// 8  G-1
      25.96     ,	// 9  G-1+
      27.50     ,	// 10 A-1
      29.14     ,	// 11 A-1+
      30.87     ,	// 12 B-1

      32.70     ,	// 13 C0
      34.65     ,	// 14 C0+
      36.71     ,	// 15 D0
      38.89     ,	// 16 D0+
      41.20     ,	// 17 E0
      43.65     ,	// 18 F0
      46.25     ,	// 19 F0+
      49.00     ,	// 20 G0
      51.91     ,	// 21 G0+
      55.00     ,	// 22 A0
      58.27     ,	// 23 A0+
      61.74     ,	// 24 B0

      65.41     ,	// 25 C1
      69.30     ,	// 26 C1+
      73.42     ,	// 27 D1
      77.78     ,	// 28 D1+ 
      82.41     ,	// 29 E1
      87.31     ,	// 30 F1
      92.50     ,	// 31 F1+
      98.00     ,	// 32 G1
      103.83    ,	// 33 G1+
      110.00    ,	// 34 A1
      116.54    ,       // 35 A1+
      123.47    ,	// 36 B1

      130.81    ,	// 37 C2
      138.59    ,	// 38 C2+
      146.83    ,	// 39 D2
      155.56    ,	// 40 D2+
      164.81    ,	// 41 E2
      174.61    ,	// 42 F2
      185.00    ,	// 43 F2+
      196.00    ,	// 44 G2
      207.65    ,	// 45 G2+
      220.00    ,	// 46 A2
      233.08    ,	// 47 A2+
      246.94    ,	// 48 B2
           
      261.63    ,	// 49 C3
      277.18    ,	// 50 C3+
      293.66    ,	// 51 D3
      311.13    ,	// 52 D3+
      329.63    ,	// 53 E3
      349.23    ,	// 54 F3
      369.99    ,	// 55 F3+
      392.00    ,	// 56 G3
      415.30    ,	// 57 G3+
      440.00    ,	// 58 A3
      466.16    ,	// 59 A3+
      493.88    ,	// 60 B3

      523.25    ,	// 61 C4
      554.37    ,	// 62 C4+
      587.33    ,	// 63 D4
      622.25    ,	// 64 D4+
      659.25    ,	// 65 E4
      698.46    ,	// 66 F4
      739.99    ,	// 67 F4+
      783.99    ,	// 68 G4
      830.61    ,	// 69 G4+
      880.00    ,	// 70 A4
      932.33    ,	// 71 A4+
      987.77    ,	// 72 B4

      1046.50   ,	// 73 C5
      1108.73   ,	// 74 C5+
      1174.66   ,	// 75 D5
      1244.51   ,	// 76 D5+
      1318.51   ,	// 77 E5
      1396.91   ,	// 78 F5
      1479.98   ,	// 79 F5+
      1567.98   ,	// 80 G5
      1661.22   ,	// 81 G5+
      1760.00   ,	// 82 A5
      1864.66   ,	// 83 A5+
      1975.53   ,	// 84 B5

      2093.00   ,	// 85 C6
      2217.46   ,	// 86 C6+
      2349.32   ,	// 87 D6
      2489.02   ,	// 88 D6+
      2637.02   ,	// 89 E6
      2793.83   ,	// 90 F6
      2959.96   ,	// 91 F6+
      3135.96   ,	// 92 G6
      3322.44   ,	// 93 G6+
      3520.00   ,	// 94 A6
      3729.31   ,	// 95 A6+
      3951.07   ,	// 96 B6

      4186.01   ,	// 97  C7
      4434.92   ,	// 98  C7+
      4698.63   ,	// 99  D7
      4978.03   ,	// 100 D7+
      5274.04   ,	// 101 E7
      5587.65   ,	// 102 F7
      5919.91   ,	// 103 F7+
      6271.93   ,	// 104 G7
      6644.88   ,	// 105 G7+
      7040.00   ,	// 106 A7
      7458.62   ,	// 107 A7+
      7902.13   ,	// 108 B7                                   
    };

  if (note<=0) 
    return note_hertz[0];
  if (note>=108)
    return note_hertz[108];

  return note_hertz[note];
}

int NoteFreq::getINoteFreq(int note)
{
  int freq;
  freq=this->getFNoteFreq(note);
  return freq;
}


int NoteFreq::getWTJumpDetune(int note,int detune)
{
  float f_note;
  float f_detune;
  int   wt;
  int   dt;
  int   nt;

  nt=note;
  dt=detune;

  if (nt<0)
    nt=0;
  if (nt>88)
    nt=88;

  if (dt<0)
    {
      if (dt>-128 && nt>0)
	{
	  nt--;
	  dt=dt+128;
	}
      else
	dt=0;
    }
  if (dt>127)
    dt=127;

  return wtjTable[nt][dt];
}


int NoteFreq::getWTJumpDetune(int notedetune)
{
  Sint32 hi;
  Sint32 lo;
  if (notedetune>12000) notedetune=12000;
  if (notedetune<0) notedetune=0;

  hi=notedetune>>7;
  lo=notedetune-((notedetune>>7)<<7);
  return wtjTable[hi][lo];
}


const char * NoteFreq::getNoteCharStar(int note)
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
  static const char * B0=    "B0 ";
  
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
  static const char * B1=    "B1 ";
  
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
  static const char * B2=    "B2 ";
  
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
  static const char * B3=    "B3 ";
  
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
  static const char * B4=    "B4 ";
  
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
  static const char * B5=    "B5 ";
  
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
  static const char * B6=    "B6 ";
  
  static const char * C7=    "C7 ";
  static const char * C7plus="C7+";
  static const char * D7=    "D7 ";
  static const char * D7plus="D7+";
  
  switch(note)
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
    case 12:  return  B0;      break;

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
    case 24:  return  B1;      break;

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
    case 36:  return  B2;      break;

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
    case 48:  return  B3;      break;

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
    case 60:  return  B4;      break;

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
    case 72:  return  B5;      break;

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
    case 84:  return  B6;      break;

    case 85:  return  C7;      break;
    case 86:  return  C7plus;  break;
    case 87:  return  D7;      break;
    case 88:  return  D7plus;  break;
    default:  return  ret;     break;
    }
  
}
