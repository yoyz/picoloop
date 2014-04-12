using namespace std;

//#include "Note.h"

#include <string>
#ifndef __PATTERNELEMENT__
#define __PATTERNELEMENT__

enum 
  {
    SINE,
    SAW,
    PULSE,
    WTBL,
    NOISE,
  };

class PatternElement
{
public:
  PatternElement();
  ~PatternElement();
  void   init();
  bool   setNote(int Note);
  int    getNote();
  float  getNoteFreq();
  char * getNoteCharStar();
  char * getOscOneTypeCharStar();
  char * getOscTwoTypeCharStar();
  bool   setTrig(bool Trig);
  bool   getTrig();

  int    getVCOMix();
  void   setVCOMix(int mix);
  int    getAttack();
  void   setAttack(int atk);
  int    getDecay();
  void   setDecay(int dc);
  int    getSustain();
  void   setSustain(int sus);
  int    getRelease();
  void   setRelease(int rls);

  int    getOscillatorOneType();
  int    getOscillatorTwoType();
  
  void   setOscillatorOneType(int type);
  void   setOscillatorTwoType(int type);

  bool   setChannel(int Channel);
  bool   setKeyValue(string key, string value);
  string getValueOfKey(string key);
  bool   clearKey(string key);
  string getStr();
  void   print();
private:
  int  checkSevenBitBoundarie(int val);
  int  Note;
  bool Trig;
  int  Channel;

  int  vcomix;
  int  attack;
  int  decay;
  int  sustain;
  int  release;

  int oscOneType;
  int oscTwoType;
  
};

#endif
/*
#ifndef __NOTE__
#define __NOTE__


float note_hertz[]=
{
  0.0,
  27.5	,	// 1
  29.1353	,	// 2
  30.8677	,	// 3
  32.7032	,	// 4
  34.6479	,	// 5
  36.7081	,	// 6
  38.8909	,	// 7
  41.2035	,	// 8
  43.6536	,	// 9
  46.2493	,	// 10
  48.9995	,	// 11
  51.9130	,	// 12
  55	,	// 13
  58.2705	,	// 14
  61.7354	,	// 15
  65.4064	,	// 16
  69.2957	,	// 17
  73.4162	,	// 18
  77.7817	,	// 19
  82.4069	,	// 20
  87.3071	,	// 21
  92.4986	,	// 22
  97.9989	,	// 23
  103.826	,	// 24
  110	,	// 25
  116.541	,	// 26
  123.471	,	// 27
  130.813	,	// 28
  138.591	,	// 29
  146.832	,	// 30
  155.563	,	// 31
  164.814	,	// 32
  174.614	,	// 33
  184.997	,	// 34
  195.998	,	// 35
  207.652	,	// 36
  220	,	// 37
  233.082	,	// 38
  246.942	,	// 39
  261.626	,	// 40
  277.183	,	// 41
  293.665	,	// 42
  311.127	,	// 43
  329.628	,	// 44
  349.228	,	// 45
  369.994	,	// 46
  391.995	,	// 47
  415.305	,	// 48
  440	,	// 49
  466.164	,	// 50
  493.883	,	// 51
  523.251	,	// 52
  554.365	,	// 53
  587.33	,	// 54
  622.254	,	// 55
  659.255	,	// 56
  698.456	,	// 57
  739.989	,	// 58
  783.991	,	// 59
  830.609	,	// 60
  880	,	// 61
  932.328	,	// 62
  987.767	,	// 63
  1046.5	,	// 64
  1108.73	,	// 65
  1174.66	,	// 66
  1244.51	,	// 67
  1318.51	,	// 68
  1396.91	,	// 69
  1479.98	,	// 70
  1567.98	,	// 71
  1661.22	,	// 72
  1760	,	// 73
  1864.66	,	// 74
  1975.53	,	// 75
  2093	,	// 76
  2217.46	,	// 77
  2349.32	,	// 78
  2489.02	,	// 79
  2637.02	,	// 80
  2793.83	,	// 81
  2959.96	,	// 82
  3135.96	,	// 83
  3322.44	,	// 84
  3520	,	// 85
  3729.31	,	// 86
  3951.07	,	// 87
  4186.01	,	// 88
};


#endif
*/


