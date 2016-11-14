#ifndef __WAVEFILEWRITER__
#define __WAVEFILEWRITER__

#include <stdint.h>
#include <stdio.h>
#include <string>
#include "Master.h"
//#include <SDL/SDL.h>
#ifdef   __SDL12__               
#include <SDL/SDL.h>             
#endif // __SDL12__              
#ifdef  __SDL20__                
#include <SDL2/SDL.h>            
#endif // __SDL20__              


/*

From : http://soundfile.sapp.org/doc/WaveFormat/


Offset  Size  Name             Description

The canonical WAVE format starts with the RIFF header:

0         4   ChunkID          Contains the letters "RIFF" in ASCII form
(0x52494646 big-endian form).
4         4   ChunkSize        36 + SubChunk2Size, or more precisely:
  4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
                               This is the size of the rest of the chunk 
                               following this number.  This is the size of the 
                               entire file in bytes minus 8 bytes for the
    two fields not included in this count:
                               ChunkID and ChunkSize.
8         4   Format           Contains the letters "WAVE"
				 (0x57415645 big-endian form).

				 The "WAVE" format consists of two subchunks: "fmt " and "data":
The "fmt " subchunk describes the sound data's format:

12        4   Subchunk1ID      Contains the letters "fmt "
                               (0x666d7420 big-endian form).
16        4   Subchunk1Size    16 for PCM.  This is the size of the
                               rest of the Subchunk which follows this number.
20        2   AudioFormat      PCM = 1 (i.e. Linear quantization)
                               Values other than 1 indicate some 
                               form of compression.
22        2   NumChannels      Mono = 1, Stereo = 2, etc.
24        4   SampleRate       8000, 44100, etc.
28        4   ByteRate         == SampleRate * NumChannels * BitsPerSample/8
32        2   BlockAlign       == NumChannels * BitsPerSample/8
                               The number of bytes for one sample including
                               all channels. I wonder what happens when
                               this number isn't an integer?
      34        2   BitsPerSample    8 bits = 8, 16 bits = 16, etc.
      2   ExtraParamSize   if PCM, then doesn't exist
          X   ExtraParams      space for extra parameters

The "data" subchunk contains the size of the data and the actual sound:

36        4   Subchunk2ID      Contains the letters "data"
                               (0x64617461 big-endian form).
40        4   Subchunk2Size    == NumSamples * NumChannels * BitsPerSample/8
                               This is the number of bytes in the data.
                               You can also think of this as the size
                               of the read of the subchunk following this 
                               number.
44        *   Data             The actual sound data.
*/



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
