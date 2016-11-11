#ifndef __WAVEFILEWRITER__
#define __WAVEFILEWRITER__

#include <stdint.h>
#include <stdio.h>
#include <string>
#include "Master.h"
#include <SDL/SDL.h>

typedef struct WAVEHEADER_WAVEFILEWRITER
{  
  char   riff[4];        // (4 bytes): FileTypeBlocID  «RIFF»  (0x52,0x49,0x46,0x46)
  Sint32 riffLength;     // (4 bytes): FileSize        file size minus 8 bytes
  char   wave[4];        // (4 bytes): FileFormatID    Format = «WAVE»  (0x57,0x41,0x56,0x45)
  
  char   fmt[4];         // (4 bytes): FormatBlocID    «fmt »  id (0x66,0x6D, 0x74,0x20)
  Sint32 waveLength;     // (4 bytes): Length of format data.  Always 16
  Sint16 wFormatTag;     // (2 bytes): storage format : 1==PCM
  Sint16 nChannels;      // (2 bytes): 1==mono, 2==stereo
  Sint32 frequency;      // (4 bytes): Frequency: 44 100 Hz Sample Rate (CD-Quality)
  Sint32 bytePerSec;     // (4 bytes): Frequency(44100) * bytePerBloc
  Sint16 bytePerBloc;    // (2 bytes): nChannels * (BitsPerSample/8)
  Sint16 wBitsPerSample; // (2 bytes): bit used per sample (8, 16, 24)
  char   data[4];        // (4 bytes): «data»  (0x64,0x61,0x74,0x61)
  Sint32 dataLength;     // (4 bytes): fileSize - sizeof(WAVEHEADER)
} WHWFW;




class WaveFileWriter
{
public:
  WaveFileWriter();
  void setName(std::string fileName);
  void setNbChannel(int nbChannel);
  void setBitRate(int bitPerSample);
  void setFrequency(int frequency);
  int  createEmptyWaveFile();
  void fillBuffer(void * buff,int nbSample);
  int  closeAndUpdate();
  
private:
  std::string fileName;
  Sint32 nbChannel;
  Sint32 bitPerSample;
  Sint32 frequency;

  Sint32 fwrite_byte_counter;
  

  int      isOpen;
  WHWFW    waveHeader;

  FILE   * fileToWrite;
};
#endif
