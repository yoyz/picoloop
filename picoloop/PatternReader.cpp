#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Pattern.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "Master.h"
#include "PatternReader.h"
#include "SYSTEM.h"

#include <sys/types.h>
#include <sys/stat.h>


using namespace std;

PatternReader::PatternReader() : twoDPVector(MAX_PATTERN_BY_PROJECT,vector <Pattern>(TRACK_MAX)), 
				 loadedData(MAX_PATTERN_BY_PROJECT, vector     <int>(TRACK_MAX))

				 //, 
				 //				 savedData(16,vector       <int>(TRACK_MAX))
{
  printf("PatternReader::PatternReader()\n");
  bank=0;
}

PatternReader::~PatternReader()
{
  printf("PatternReader::~PatternReader()\n");
}

void PatternReader::init()
{
  int x;
  int y;
  
  for (x=0;x<MAX_PATTERN_BY_PROJECT;x++)
    for (y=0;y<TRACK_MAX;y++)
      {
	loadedData[x][y]=DATA_HAS_NOT_BEEN_CHECK;
	//	savedData[x][y]=0;
	twoDPVector[x][y].init();
      }
  bank=0;
}

void PatternReader::setBank(int b)
{
  if (b>=0 && b < 256)
    bank=b;
}

int PatternReader::getBank()
{
  return bank;
}


int PatternReader::saveSong(SongSequencer & SS)
{
  char filename[1024];
  unsigned char line[MAX_PATTERN_BY_PROJECT];
  int i;
  int j;
  sprintf(filename,"bank/bank%d/song.pic",bank);
  fd=fopen(filename,"w");
  if (fd==0)
    {
      return 0;
    }
  for (j=0;j<TRACK_MAX;j++)
    {
      for (i=0;i<MAX_PATTERN_BY_PROJECT;i++)
	{
	  line[i]=SS.songVector[i][j];
	}
      fwrite(line,sizeof(unsigned char),sizeof(unsigned char)*MAX_PATTERN_BY_PROJECT,fd);
    }
  fclose(fd);
}

int PatternReader::loadSong(SongSequencer & SS)
{
  char  filename[1024];
  unsigned char line[MAX_PATTERN_BY_PROJECT];
  int i;
  int j;
  sprintf(filename,"bank/bank%d/song.pic",bank);
  fd=fopen(filename,"r");
  if (fd==0)
    {
      return 0;
    }
  for (j=0;j<TRACK_MAX;j++)
    {
      fread(line,sizeof(unsigned char),sizeof(unsigned char)*MAX_PATTERN_BY_PROJECT,fd);
      for (i=0;i<MAX_PATTERN_BY_PROJECT;i++)
	{
	  SS.songVector[i][j]=line[i];
	}
      //fwrite(line,sizeof(char),sizeof(char)*MAX_PATTERN_BY_PROJECT,fd);
    }
  fclose(fd);
}

bool PatternReader::PatternRemove(int PatternNumber,int TrackNumber)
{
  char filename[1024];
  sprintf(filename,"bank/bank%d/dataP%dT%d.pic",bank,PatternNumber,TrackNumber);
  if (unlink(filename)==0)
    {
      loadedData[PatternNumber][TrackNumber]=DATA_DOES_NOT_EXIST_ON_STORAGE;
      return true;
    }
  else
    return false;
}


bool PatternReader::PatternDataExist(int PatternNumber,int TrackNumber)
{
  int match=0;
  //char * line;
  int PatNum,TrackNum,PatSize;
  bool retcode=true;
  bool found=false;
  char line[1024];
  char filename[1024];

  //printf("?EXIST (%d %d)\n");

  if (loadedData[PatternNumber][TrackNumber]==DATA_EXIST_ON_STORAGE | 
      loadedData[PatternNumber][TrackNumber]==DATA_LOADED_FROM_STORAGE)
    return true;
  if (loadedData[PatternNumber][TrackNumber]==DATA_DOES_NOT_EXIST_ON_STORAGE)
    return false;

  //line=(char*)malloc(1024);
  //fd=fopen(fn.c_str(),"r+");
  sprintf(filename,"bank/bank%d/dataP%dT%d.pic",bank,PatternNumber,TrackNumber);
  fd=fopen(filename,"r+");
  if (fd==0)
    {
      loadedData[PatternNumber][TrackNumber]=DATA_DOES_NOT_EXIST_ON_STORAGE;
      return false;
    }

  while (match==0 && retcode==true)
    {
      char * catcheof;
      catcheof=fgets(line,512,fd);
      //printf("[%s]\n",st);
      //sleep(1);

      //match('Pattern 1 Track 1 Param PatternSize 16')
      sscanf(line,"Pattern %d Track %d Param PatternSize %d",
	     &PatNum,&TrackNum,&PatSize);
      if (PatNum    ==PatternNumber &&
	  TrackNum  ==TrackNumber)
	match=1;

      if ( catcheof==NULL)
	retcode=false;
    }

  if (match)
    found=true;

  if (found==true)
    loadedData[PatternNumber][TrackNumber]=DATA_EXIST_ON_STORAGE;
  else
    loadedData[PatternNumber][TrackNumber]=DATA_DOES_NOT_EXIST_ON_STORAGE;


  //free(line);
  fclose(fd);
  return found;
}

bool PatternReader::readPatternData(int PatternNumber,int TrackNumber, Pattern & P)
{
  PatternElement Pe;
  FILE *fd;
  int PatNum;
  int TrackNum;
  int PatSize;
  int PatBPM;
  int PatBPMDivider;
  int PatSwing;
  int n[16];
  int t[16];
  int i;
  bool retcode=true;
  int match=0;
  //char * line;
  //line=(char*)malloc(1024);
  char line[1024];
  char filename[1024];
  char strParam[64];
  int  machineParam;


  if (loadedData[PatternNumber][TrackNumber]==DATA_LOADED_FROM_STORAGE)
    {
      P=twoDPVector[PatternNumber][TrackNumber];
      return true;
    }

  sprintf(filename,"bank/bank%d/dataP%dT%d.pic",bank,PatternNumber,TrackNumber);


  //check if file name can be open
  //fd=fopen(fn.c_str(),"r+");
  fd=fopen(filename,"r+");
  if (fd==0)
    {
      printf("[data file %s not found]\n",fn.c_str());
      loadedData[PatternNumber][TrackNumber]=DATA_DOES_NOT_EXIST_ON_STORAGE;
      //twoDPVector[PatternNumber][TrackNumber].init();
      P.init();
      twoDPVector[PatternNumber][TrackNumber]=P;
      return false;
    }
  
  while (match==0 && retcode==true)
    {
      char * catcheof;
      catcheof=fgets(line,512,fd);
      //printf("[%s]\n",st);
      //sleep(1);
      
      //match('Pattern 1 Track 1 Param PatternSize 16')
      sscanf(line,"Pattern %d Track %d Param PatternSize %d",
	     &PatNum,&TrackNum,&PatSize);
      //printf("%d %d %d",PatNum,TrackNum,PatSize);
      if (PatNum    ==PatternNumber &&
	  TrackNum  ==TrackNumber)
	match=1;
      if ( catcheof==NULL)
	retcode=false;
    }

  if (retcode==false)
    {
      P.setPatternSize(16);
      return false;
    }
  
  P.setPatternSize(PatSize);
  
  if (fgets(line,512,fd))
    {
      //match('Pattern 1 Track 1 Param PatternBPM 80')
      sscanf(line,"Pattern %d Track %d Param PatternBPM %d",
	     &PatNum,&TrackNum,&PatBPM);
      if (PatNum    ==PatternNumber &&
	  TrackNum  ==TrackNumber)
	{
	  P.setBPM(PatBPM);
	}
    }

  if (fgets(line,512,fd))
    {
      //match('Pattern 1 Track 1 Param PatternBPMDivider 4')
      sscanf(line,"Pattern %d Track %d Param PatternBPMDivider %d",
	     &PatNum,&TrackNum,&PatBPMDivider);
      if (PatNum    ==PatternNumber &&
	  TrackNum  ==TrackNumber)
	{
	  P.setBPMDivider(PatBPMDivider);
	}
    }


  if (fgets(line,512,fd))
    {
      //match('Pattern 1 Track 1 Param PatternBPMDivider 4')
      sscanf(line,"Pattern %d Track %d Param PatternSwing %d",
	     &PatNum,&TrackNum,&PatSwing);
      if (PatNum    ==PatternNumber &&
	  TrackNum  ==TrackNumber)
	{
	  P.setSwing(PatSwing);
	}
    }

  machineParam=LFO1_DEPTH;
  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Note 41 0 52 0 22 0 12 0 21 11 14 12 43 12 54')
  sscanf(line,
	 "Pattern %d Track %d Param %s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,strParam,
	 &n[0], &n[1], &n[2], &n[3],
	 &n[4], &n[5], &n[6], &n[7],
	 &n[8], &n[9], &n[10],&n[11],
	 &n[12],&n[13],&n[14],&n[15]);

      if (PatNum    ==PatternNumber &&
	  TrackNum  ==TrackNumber   &&
	  strcmp(strParam,this->getParameterCharStar(machineParam))==0)
	for (i=0;i<PatSize;i++)
	  {
	    Pe=P.getPatternElement(i);      
	    Pe.set(machineParam,n[i]);
	    P.setPatternElement(i,Pe);      
	  }
      else
	retcode=false;


  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Note 41 0 52 0 22 0 12 0 21 11 14 12 43 12 54')
  sscanf(line,
	 "Pattern %d Track %d Param PitchLfoSpeed %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &n[0], &n[1], &n[2], &n[3],
	 &n[4], &n[5], &n[6], &n[7],
	 &n[8], &n[9], &n[10],&n[11],
	 &n[12],&n[13],&n[14],&n[15]);

      if (PatNum    ==PatternNumber &&
	  TrackNum  ==TrackNumber)
	for (i=0;i<PatSize;i++)
	  {
	    Pe=P.getPatternElement(i);      
	    Pe.set(LFO1_FREQ,n[i]);
	    P.setPatternElement(i,Pe);      
	  }
      else
	retcode=false;



  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Note 41 0 52 0 22 0 12 0 21 11 14 12 43 12 54')
  sscanf(line,
	 "Pattern %d Track %d Param Amp %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &n[0], &n[1], &n[2], &n[3],
	 &n[4], &n[5], &n[6], &n[7],
	 &n[8], &n[9], &n[10],&n[11],
	 &n[12],&n[13],&n[14],&n[15]);

      if (PatNum    ==PatternNumber &&
	  TrackNum  ==TrackNumber)
	for (i=0;i<PatSize;i++)
	  {
	    Pe=P.getPatternElement(i);      
	    Pe.set(AMP,n[i]);
	    P.setPatternElement(i,Pe);      
	  }
      else
	retcode=false;

  
  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Note 41 0 52 0 22 0 12 0 21 11 14 12 43 12 54')
  sscanf(line,
	 "Pattern %d Track %d Param Note %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &n[0], &n[1], &n[2], &n[3],
	 &n[4], &n[5], &n[6], &n[7],
	 &n[8], &n[9], &n[10],&n[11],
	 &n[12],&n[13],&n[14],&n[15]);

      if (PatNum    ==PatternNumber &&
	  TrackNum  ==TrackNumber)
	for (i=0;i<PatSize;i++)
	  {
	    Pe=P.getPatternElement(i);      
	    Pe.set(NOTE,n[i]);
	    P.setPatternElement(i,Pe);      
	  }
      else
	retcode=false;



  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Trig 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param Trig %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);      
	Pe.set(NOTE_ON,t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;


  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Decay 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param VCOMix %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);      
	Pe.set(VCO_MIX,t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;


  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Attack 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param Attack %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);      
	Pe.set(ADSR_AMP_ATTACK,t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;





  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Decay 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param Decay %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);      
	Pe.set(ADSR_AMP_DECAY,t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;




  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Sustain 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param Sustain %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);      
	Pe.set(ADSR_AMP_SUSTAIN,t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;


  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Release 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param Release %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);      
	Pe.set(ADSR_AMP_RELEASE,t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;


  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Release 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param OscOneType %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);
	Pe.set(OSC1_TYPE,t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;

  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Release 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param OscTwoType %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);    
	Pe.set(OSC2_TYPE,t[i]);  
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;

  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Release 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param Cutoff %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);    
	Pe.set(FILTER1_CUTOFF,t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;

  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Release 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param Resonance %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);    
	Pe.set(FILTER1_RESONANCE,t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;

  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Release 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param PhaseOsc1 %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);    
	Pe.set(OSC1_PHASE,t[i]);  
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;



  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Attack 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param AttackFltr %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);      
	Pe.set(ADSR_FLTR_ATTACK,t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;





  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Decay 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param DecayFltr %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);      
	Pe.set(ADSR_FLTR_DECAY,t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;




  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Sustain 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param SustainFltr %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);      
	Pe.set(ADSR_FLTR_SUSTAIN,t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;


  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Release 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param ReleaseFltr %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);      
	Pe.set(ADSR_FLTR_RELEASE,t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;


  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Release 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param NoteADSR %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);      
	//Pe.setNoteADSR(t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;


  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Release 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param MachineType %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);      
	Pe.setMachineType(t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;


  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Release 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param FilterAlgo %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);      
	Pe.set(FILTER1_ALGO,t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;



  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Release 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param FilterType %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);      
	Pe.set(FILTER1_TYPE,t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;



  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Release 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param Osc1Amp %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);      
	//Pe.setOsc1Amp(t[i]);
	Pe.set(OSC1_AMP,t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;


  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Release 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param Osc2Amp %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);      
	Pe.set(OSC2_AMP,t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;

  //if (retcode==false)
  //exit(0);


  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Release 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param FxDepth %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);      
	Pe.set(FX1_DEPTH,t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;


  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Release 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param FxSpeed %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);      
	Pe.set(FX1_SPEED,t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;

  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Release 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param FmType %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);      
	Pe.setFmType(t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;


  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Release 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param TrigTime %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);      
	//Pe.setTrigTime(t[i]);
	Pe.set(TRIG_TIME_DURATION,t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;


  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Release 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param PitchBendDepth %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);      
	Pe.set(PITCHBEND_DEPTH,t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;


  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Release 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param PitchBendSpeed %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);      
	Pe.set(PITCHBEND_SPEED,t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;


  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Release 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param LfoType %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);      
	Pe.setLfoType(t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;



  if (retcode==true)
    {
      loadedData[PatternNumber][TrackNumber]=DATA_LOADED_FROM_STORAGE;
      twoDPVector[PatternNumber][TrackNumber]=P;
    }
  
  //free(line);
  fclose(fd);
  return retcode;
}


bool PatternReader::writePattern(int PatternNumber, int TrackNumber, Pattern & P)
{  
  FILE         * fd=NULL;
  int            PatNum=0;
  int            TrackNum=0;
  int            PatSize=0;  
  int            match=0;
  vector<string> data;
  bool           retcode=true;
  bool           found=false;
  //char         * line=NULL;
  //char         * line2=NULL;
  char         * catcheof=NULL;
  //line=(char*)malloc(1024);
  //line2=(char*)malloc(1024);
  char line[1024];
  char line2[1024];
  char filename[1024];
  char path[1024];
  mode_t mode;

  //mode=0666;
  //mode=S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IWGRP|S_IROTH|S_IWOTH;

  //mode=0755;
  
  sprintf(path,    "bank");
  //mkdir(path,mode);
  MKDIR(path);

  sprintf(path,    "bank/bank%d",bank);
  //mkdir(path,mode);
  MKDIR(path);

  sprintf(filename,"bank/bank%d/dataP%dT%d.pic",bank,PatternNumber,TrackNumber);

  //  printf("SIZE:%d\n",P.getSize());
  //  exit(1);

  //  loadedData[PatternNumber][TrackNumber]==0;
  //savedData[PatternNumber][TrackNumber]==0;

  
  //fd=fopen(fn.c_str(),"r+");
  fd=fopen(filename,"r+");
  if (fd==0)
    {
      printf("[data file %s not found]\n",fn.c_str());
      retcode=false;
      //exit(213);
    }
  else
    {

      while (retcode==true)
	{
	  catcheof=fgets(line,512,fd);
	  sscanf(line,"Pattern %d Track %d ",
		 &PatNum,&TrackNum);
	  if (PatNum    ==  PatternNumber &&
	      TrackNum  ==  TrackNumber)
	    found=true;
	  else
	    {
	      data.insert(data.end(),line);
	    }
	  if ( catcheof==NULL)
	    retcode=false;
	}
      fclose(fd);
    }
  //  data.insert(data.end(),"\n");

  //insert 'Pattern 2 Track 2 Param PatternSize 16'
  sprintf(line,"Pattern %d Track %d Param PatternSize %d\n",
	  PatternNumber,
	  TrackNumber, 
	  P.getSize());
  data.insert(data.end(),line);


  sprintf(line,"Pattern %d Track %d Param PatternBPM %d\n",
	  PatternNumber,
	  TrackNumber, 
	  P.getBPM());
  data.insert(data.end(),line);

  sprintf(line,"Pattern %d Track %d Param PatternBPMDivider %d\n",
	  PatternNumber,
	  TrackNumber, 
	  P.getBPMDivider());
  data.insert(data.end(),line);


  sprintf(line,"Pattern %d Track %d Param PatternSwing %d\n",
	  PatternNumber,
	  TrackNumber, 
	  P.getSwing());
  data.insert(data.end(),line);


  //line=""; //Weird ?


  //insert 'Pattern 2 Track 2 Param Note 84 0 52 0 22 0 12 0 21 11 14 12 43 12 54 13'
  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param PitchLfoDepth ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).get(LFO1_DEPTH));
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);


  //insert 'Pattern 2 Track 2 Param Note 84 0 52 0 22 0 12 0 21 11 14 12 43 12 54 13'
  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param PitchLfoSpeed ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).get(LFO1_FREQ));
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);


  //insert 'Pattern 2 Track 2 Param Note 84 0 52 0 22 0 12 0 21 11 14 12 43 12 54 13'
  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param Amp ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).get(AMP));
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);


  //insert 'Pattern 2 Track 2 Param Note 84 0 52 0 22 0 12 0 21 11 14 12 43 12 54 13'
  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param Note ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).get(NOTE));
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);

  //insert 'Pattern 2 Track 2 Param Trig 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0 0'
  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param Trig ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).get(NOTE_ON));
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);

  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param VCOMix ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).get(VCO_MIX));
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);

  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param Attack ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).get(ADSR_AMP_ATTACK));
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);

  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param Decay ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).get(ADSR_AMP_DECAY));
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);

  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param Sustain ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).get(ADSR_AMP_SUSTAIN));
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);

  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param Release ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).get(ADSR_AMP_RELEASE));
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);

  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param OscOneType ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).get(OSC1_TYPE));
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);

  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param OscTwoType ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).get(OSC2_TYPE));    
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);


  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param Cutoff ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).get(FILTER1_CUTOFF));
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);

  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param Resonance ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).get(FILTER1_RESONANCE));
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);


  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param PhaseOsc1 ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).get(OSC1_PHASE));
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);


  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param AttackFltr ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).get(ADSR_FLTR_ATTACK));
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);

  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param DecayFltr ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).get(ADSR_FLTR_DECAY));
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);

  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param SustainFltr ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).get(ADSR_FLTR_SUSTAIN));
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);

  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param ReleaseFltr ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).get(ADSR_FLTR_RELEASE));
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);


  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param NoteADSR ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",0);
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);

  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param MachineType ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).getMachineType());
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);



  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param FilterAlgo ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).get(FILTER1_ALGO));
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);

  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param FilterType ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).get(FILTER1_TYPE));
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);

  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param Osc1Amp ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).get(OSC1_AMP));
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);


  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param Osc2Amp ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).get(OSC2_AMP));
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);


  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param FxDepth ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).get(FX1_DEPTH));
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);


  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param FxSpeed ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).get(FX1_SPEED));
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);

  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param FmType ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).getFmType());
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);

  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param TrigTime ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).get(TRIG_TIME_DURATION));
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);


  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param PitchBendDepth ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).get(PITCHBEND_DEPTH));
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);


  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param PitchBendSpeed ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).get(PITCHBEND_SPEED));
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);


  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param LfoType ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).getLfoType());
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);




  sprintf(line,"\n");
  data.insert(data.end(),line);

  sprintf(line,"\n");
  data.insert(data.end(),line);

  //fd=fopen(fn.c_str(),"w");
  fd=fopen(filename,"w");
  for (int i=0; i< data.size();i++)
    {
      fprintf(fd,"%s",data[i].c_str());
    }
  fclose(fd);

  //loadedData[PatternNumber][TrackNumber]=DATA_EXIST_ON_STORAGE;
  //twoDPVector[PatternNumber][TrackNumber]=P;

  //if (retcode==true)
  //{
  loadedData[PatternNumber][TrackNumber]=DATA_LOADED_FROM_STORAGE;
  twoDPVector[PatternNumber][TrackNumber]=P;
      ///}
}


/*
void PatternReader::setFileName(string filename)
{
  fn=filename;
}
*/


const char * PatternReader::getParameterCharStar(int param)
{
  static const char * ret="NULL";

  static const char * adsr_env0_attack="Attack";         
  static const char * adsr_env0_decay="Decay";          
  static const char * adsr_env0_sustain="Sustain";        
  static const char * adsr_env0_release="Release";        
  
  static const char * osc1_type="OscOneType";     
  static const char * osc2_type="OscTwoType";     
  
  static const char * filter1_cutoff="Cutoff";         
  static const char * filter1_resonance="Resonance";      
  
  static const char * osc1_phase="PhaseOsc1";      
  
  static const char * adsr_env1_attack="AttackFltr";     
  static const char * adsr_env1_decay="DecayFltr";      
  static const char * adsr_env1_sustain="SustainFltr";    
  static const char * adsr_env1_release="ReleaseFltr";    
  
  static const char * note_adsr="NoteADSR";       
  
  static const char * machine_type="MachineType";    
  static const char * filter1_algo="FilterAlgo";     
  static const char * filter1_type="FilterType";     
  
  static const char * osc1_amp="Osc1Amp";        
  static const char * osc2_amp="Osc2Amp";        
  
  static const char * fx1_depth="FxDepth";        
  static const char * fx1_speed="FxSpeed";        
  
  static const char * fm_type="FmType";         
  
  static const char * trig_time_duration="TrigTime";       
  
  static const char * pitchbend_depth="PitchBendDepth"; 
  static const char * pitchbend_speed="PitchBendSpeed"; 
  
  static const char * lfo_type="LfoType";
    


  switch (param)
    {
    case ADSR_ENV0_ATTACK:   return adsr_env0_attack;         break;
    case ADSR_ENV0_DECAY:    return adsr_env0_decay;          break;
    case ADSR_ENV0_SUSTAIN:  return adsr_env0_sustain;        break;
    case ADSR_ENV0_RELEASE:  return adsr_env0_release;        break;

    case OSC1_TYPE:          return osc1_type;     break;
    case OSC2_TYPE:          return osc2_type;     break;
      
    case FILTER1_CUTOFF:     return filter1_cutoff;         break;
    case FILTER1_RESONANCE:  return filter1_resonance;      break;
    
    case OSC1_PHASE:         return osc1_phase;      break;
    
    case ADSR_ENV1_ATTACK:   return adsr_env1_attack;     break;
    case ADSR_ENV1_DECAY:    return adsr_env1_decay;      break;
    case ADSR_ENV1_SUSTAIN:  return adsr_env1_sustain;    break;
    case ADSR_ENV1_RELEASE:  return adsr_env1_release;    break;

    case NOTE_ADSR:          return note_adsr;       break;

    case MACHINE_TYPE:       return machine_type;    break;
    case FILTER1_ALGO:       return filter1_algo;     break;
    case FILTER1_TYPE:       return filter1_type;     break;

    case OSC1_AMP:           return osc1_amp;        break;
    case OSC2_AMP:           return osc2_amp;        break;

    case FX1_DEPTH:          return fx1_depth;        break;
    case FX1_SPEED:          return fx1_speed;        break;

    case FM_TYPE:            return fm_type;         break;

    case TRIG_TIME_DURATION: return trig_time_duration;       break;

    case PITCHBEND_DEPTH:    return pitchbend_depth; break;
    case PITCHBEND_SPEED:    return pitchbend_speed; break;

    case LFO_TYPE:           return lfo_type;                 break;
    

    }
  return ret;
}
