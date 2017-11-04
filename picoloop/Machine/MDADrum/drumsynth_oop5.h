//Make a .WAV file from a .DS file

//#include <windows.h>  //for APIENTRY
#ifndef  __DRUMSYNTH_OOP5__
#define  __DRUMSYNTH_OOP5__


/* int APIENTRY ds2wav(char *dsfile, char *wavfile, HWND hWnd); */
/* int APIENTRY ds2mem(char *dsfile, short *buffer, float t, float o, float n, float b, float tune, float time); */
/* int APIENTRY ds2buf(long samples, short *buffer); */
#include <stdint.h>
#include <unistd.h>

//int ds2wav(char *dsfile, char *wavfile, HWND hWnd);
int ds2wav(char *dsfile, char *wavfile);
int ds2mem(char *dsfile, short *buffer, float t, float o, float n, float b, float tune, float time);
int ds2buf(long samples, short *buffer);



typedef  const char * LPCTSTR;
//typedef  

typedef uint32_t DWORD;
typedef uint16_t WORD;
typedef DWORD FOURCC;             /* Type of FOUR Character Codes */

/*  Visual Basic Declarations:

Declare Function ds2wav Lib "ds2wav.dll" (ByVal dsfile As String, ByVal wavfile As String) As Long
'
' Makes a WAV file from a DS file
' Returns: 0 - success
'          1 - version failed (this DLL out of date)
'          2 - input failed
'          3 - output failed

Declare Function ds2mem Lib "ds2wav.dll" (ByVal dsfile As String, ByVal buffer As Integer, _
                        ByVal t As Single, ByVal o As Single, ByVal n As Single, _
                        ByVal b As Single, ByVal tune As Single, ByVal time As Single) As Long
'
' Makes a waveform in memory from a DS file (ds2wav sets buffer)
' Returns number of 16-bit samples available in ds2wav's buffer[] or zero if failed
'
' t    = tone      (volume for each section: suggest square-law range 0.0...1.0...4.0)
' o    = overtone
' n    = noise
' b    = noise bands
' tune = tuning offset in semitones
' time = time scaling factor


Declare Function ds2buf Lib "ds2wav.dll" (ByVal samples As Long, buffer As Integer) As Long
'
' Copies a number of waveform samples to your own buffer
' Returns number of 16-bit samples copied into buffer[] or zero if failed
'
*/

#define Fs      44100
#define TwoPi   6.2831853f
#define MDA_MAX     0
#define ENV     1
#define PNT     2 
#define dENV    3
#define NEXTT   4


struct WAVEHEADER 
{  
  FOURCC riff;
  DWORD  riffLength;
  FOURCC wave;
  FOURCC fmt;
  DWORD  waveLength;
  WORD   wFormatTag; 
  WORD   nChannels; 
  DWORD  nSamplesPerSec; 
  DWORD  nAvgBytesPerSec; 
  WORD   nBlockAlign; 
  WORD   wBitsPerSample; 
  FOURCC data;
  DWORD  dataLength;
};

struct WAVEINFO
{
  FOURCC list;
  DWORD  listLength;
  FOURCC info;
  FOURCC isft;
  DWORD  isftLength;
  char   software[16];
  FOURCC icmt;
  DWORD  icmtLength;
};

class drumsynth
{
  public:
  drumsynth();
  ~drumsynth();
  void init();
  void set_tune(float t);
  void set_time(float t);
    
  int load_patch(char *dsfile);
  int LongestEnv(void);
  float LoudestEnv(void);
  void UpdateEnv(int e, long t);
  void getEnv(int env, LPCTSTR sec, LPCTSTR key, LPCTSTR ini);
  float waveform(float ph, int form);
  //int ds2mem(char *dsfile, short *buffer, float t, float o, float n, float b, float tune, float time);
  //int ds2buf(long samples, short *buffer);
  //int ds2wav(char *dsfile, char *wavfile);
  //int setdsfile(char * mydsfile);
  //int setwavefile(char * mywavefile);

  int generate(short *buffer, int len);

  int   busy;
  float envpts[8][3][32];    //envelope/time-level/point
  float envData[8][6];       //envelope running status
  int   chkOn[8], sliLev[8]; //section on/off and level
  float timestretch;         //overall time scaling
  short DD[1200], clippoint;
  float DF[1200];
  float phi[1200];
  
  short *wave; //for waveform generation in memory buffer
  long  wavewords;
  long wavemode;
  float mem_t; 
  float mem_o; 
  float mem_n; 
  float mem_b; 
  float mem_tune;
  float mem_time;

  long  Length, progress, pos, tpos, tplus, totmp, t, i, j;
  float x[3];
  float MasterTune, randmax, randmax2;
  int   MainFilter, HighPass;
  
  long  NON, NT, TON, DiON, TDroop, DStep;
  float a, b, c, d, g, TT, TTT, TL, NL, F1, F2, Fsync;
  float TphiStart, Tphi, TDroopRate, ddF, DAtten, DGain;
  
  long  BON, BON2, BFStep, BFStep2, botmp;
  float BdF, BdF2, BPhi, BPhi2, BF, BF2, BQ, BQ2, BL, BL2;

  long  OON, OF1Sync, OF2Sync, OMode, OW1, OW2;
  float Ophi1, Ophi2, OF1, OF2, OL, Ot, OBal1, OBal2, ODrive;
  float Ocf1, Ocf2, OcF, OcQ, OcA, Oc[6][2];  //overtone cymbal mode
  float Oc0, Oc1, Oc2;

  float MFfb, MFtmp, MFres, MFin, MFout;
  float DownAve; 
  long  DownStart, DownEnd, jj;
  
  int prepare;
  FILE *fp;
  struct WAVEHEADER WH;
  struct WAVEINFO   WI;
  char sec[32];
  char ver[32]; 
  char comment[256];
  char percent[16];
  int commentLen;
  char * dsfile;
  char * wavefile;
};


#endif
