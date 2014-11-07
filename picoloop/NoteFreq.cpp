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

  if (wtjTable!=0)
    return;

  wtjTable=malloc(sizeof(Sint32**)*128);
  for (i=0;i<109;i++)
    wtjTable[i]=malloc(sizeof(Sint32)*128);

  for (i=1;i<109;i++)
    for (j=0;j<128;j++)
      {
	freq=this->getFNoteFreq(i);

	wtjump_freq=(freq*WAVETABLE_SIZE*(1<<16))/DEFAULTFREQ;	
	wtjump_detune_1=(((this->getFNoteFreq(i+2)*((WAVETABLE_SIZE*(1<<16))/DEFAULTFREQ))))*j;
	wtjump_detune_2=(((this->getFNoteFreq(i-2)*((WAVETABLE_SIZE*(1<<16))/DEFAULTFREQ))))*(128-j);

	wtjump_detune=(wtjump_detune_1-wtjump_detune_2)/512;	  

	wtjTable[i][j]=wtjump_freq+wtjump_detune;


	//printf("wtjTable[%d][%d]=%d freq:%f wtjump_freq:%f wtjump_detune:%f\n",i,j,wtjTable[i][j],freq,wtjump_freq,wtjump_detune);
      }
  //exit(0);
}

float NoteFreq::getFNoteFreq(int note)
{
  
  float note_hertz[]=
    {
      0.0       ,       // 0  N/A
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
      98.00     ,	// 32 A1
      103.83    ,	// 33 A1+
      110.00    ,	// 34 G1
      116.54    ,       // 35 G1+
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
    dt=0;
  if (dt>127)
    dt=127;


  //f_note=this->getFNoteFreq(note);
  //f_detune=this->getFNoteFreq(note
  //wt=(f_note*WAVETABLE_SIZE)/DEFAULTFREQ;
  return wtjTable[nt][dt];
}
