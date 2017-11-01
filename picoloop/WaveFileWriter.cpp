#include "WaveFileWriter.h"

WaveFileWriter::WaveFileWriter()
{
  fileName="";
  nbChannel=1;
  bitPerSample=16;
  frequency=44100;

  isOpen=0;
  fwrite_byte_counter=0;
}
WaveFileWriter::~WaveFileWriter()
{
}

void WaveFileWriter::setName(std::string fn)
{
  fileName=fn;
}

void WaveFileWriter::setNbChannel(int nb)
{
  nbChannel=nb;
}

void WaveFileWriter::setBitRate(int br)
{
}

void WaveFileWriter::setFrequency(int freq)
{
}

int WaveFileWriter::createEmptyWaveFile()
{
  fileToWrite = fopen(fileName.c_str(),"w");
  waveHeader.riff[0]='R';          // "RIFF"
  waveHeader.riff[1]='I';          // "RIFF"
  waveHeader.riff[2]='F';          // "RIFF"
  waveHeader.riff[3]='F';          // "RIFF"
  waveHeader.riffLength=0;             // 0 - 8 => 0
  waveHeader.wave[0]='W';
  waveHeader.wave[1]='A';
  waveHeader.wave[2]='V';
  waveHeader.wave[3]='E';

  waveHeader.fmt[0]=0x66;
  waveHeader.fmt[1]=0x6d;
  waveHeader.fmt[2]=0x74;
  waveHeader.fmt[3]=0x20;
  waveHeader.waveLength=16;            // allways 16 ?
  waveHeader.wFormatTag=1;             // 1==PCM
  waveHeader.nChannels=1;              // 1==mono
  waveHeader.frequency=frequency;    // 44100 defined in Master.h
  waveHeader.bytePerSec=frequency*nbChannel*bitPerSample/8; // 
  waveHeader.bytePerBloc=bitPerSample/8*nbChannel;            // 
  waveHeader.wBitsPerSample=bitPerSample;  //  16 bit

  waveHeader.data[0]='d';               // «data»  (0x64,0x61,0x74,0x61)
  waveHeader.data[1]='a';               // «data»  (0x64,0x61,0x74,0x61)
  waveHeader.data[2]='t';               // «data»  (0x64,0x61,0x74,0x61)
  waveHeader.data[3]='a';               // «data»  (0x64,0x61,0x74,0x61)
  waveHeader.dataLength=0;              // fileSize - sizeof(WAVEHEADER)  

  fwrite(&waveHeader,sizeof(WHWFW),1,fileToWrite);
  fflush(fileToWrite);
  return 0;
}

void WaveFileWriter::fillBuffer(void * buff,int nbSample)
{
  fwrite_byte_counter+=fwrite(buff,(bitPerSample/8)*nbChannel,nbSample,fileToWrite)*sizeof(char)*bitPerSample*nbChannel;
}

int WaveFileWriter::closeAndUpdate()
{
  fseek(fileToWrite,0,SEEK_SET);
  /*
  waveHeader.riffLength=fwrite_byte_counter+36;   // data+header-8
  waveHeader.dataLength=fwrite_byte_counter;      // data
  */

  waveHeader.riffLength=(fwrite_byte_counter*(bitPerSample/8)*nbChannel)+36;   // data+header-8
  waveHeader.dataLength=fwrite_byte_counter*(bitPerSample/8)*nbChannel;      // data

  fwrite(&waveHeader,sizeof(WHWFW),1,fileToWrite);
  fclose(fileToWrite);
  return 0;
}
