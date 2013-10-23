#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Pattern.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "PatternReader.h"

using namespace std;

PatternReader::PatternReader()
{
}


bool PatternReader::PatternDataExist(int PatternNumber,int TrackNumber)
{
  int match=0;
  char * line;
  line=(char*)malloc(1024);
  int PatNum,TrackNum,PatSize;
  bool retcode=true;
  bool found=false;

  fd=fopen(fn.c_str(),"r+");

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
  if (match)found=true;

  free(line);
  fclose(fd);
  return found;
}

bool PatternReader::readPatternData(int PatternNumber,int TrackNumber, Pattern & P)
{
  PatternElement Pe;
  int PatNum,TrackNum,PatSize;
  int n[16];
  int t[16];
  int i;
  bool retcode=true;
  int match=0;
  char * line;
  line=(char*)malloc(1024);

  //check if file name can be open
  fd=fopen(fn.c_str(),"r+");

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

  P.setPatternSize(PatSize);


  
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
	    Pe.setNote(n[i]);
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
	Pe.setTrig(t[i]);
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
	Pe.setAttack(t[i]);
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
	Pe.setDecay(t[i]);
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
	Pe.setSustain(t[i]);
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
	Pe.setRelease(t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;

  
  free(line);
  fclose(fd);
  return retcode;
}


bool PatternReader::writePattern(int PatternNumber, int TrackNumber, Pattern & P)
{
  FILE *fd;
  int PatNum,TrackNum,PatSize;
  vector<string> data;
  int match=0;
  bool retcode=true;
  char * line;
  char * line2;
  char * catcheof;
  line=(char*)malloc(1024);
  line2=(char*)malloc(1024);
  bool found=false;
  
  fd=fopen(fn.c_str(),"r+");
  
  while (retcode==true)
    {
      catcheof=fgets(line,512,fd);
      sscanf(line,"Pattern %d Track %d",
	     &PatNum,&TrackNum);
      if (PatNum    ==PatternNumber &&
	  TrackNum  ==TrackNumber)
	found=true;
      else
	{
	  data.insert(data.end(),line);
	  //printf("%s",line);
	}
      if ( catcheof==NULL)
	retcode=false;
    }
  fclose(fd);

  //  data.insert(data.end(),"\n");

  //insert 'Pattern 2 Track 2 Param PatternSize 16'
  sprintf(line,"Pattern %d Track %d Param PatternSize %d\n",
	  PatternNumber,
	  TrackNumber, 
	  P.getSize());
  data.insert(data.end(),line);

  //line=""; //Weird ?

  //insert 'Pattern 2 Track 2 Param Note 84 0 52 0 22 0 12 0 21 11 14 12 43 12 54 13'
  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param Note ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).getNote());
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);

  //insert 'Pattern 2 Track 2 Param Trig 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0 0'
  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param Trig ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).getTrig());
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);

  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param Attack ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).getAttack());
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);

  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param Decay ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).getDecay());
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);

  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param Sustain ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).getSustain());
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);

  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param Release ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).getRelease());
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);


  sprintf(line,"\n");
  data.insert(data.end(),line);

  sprintf(line,"\n");
  data.insert(data.end(),line);

  fd=fopen(fn.c_str(),"w");
  for (int i=0; i< data.size();i++)
    {
      fprintf(fd,"%s",data[i].c_str());
    }
  fclose(fd);
  //printf("%d",data.size());
  //  for (int i=0; i< data.size();i++)
  //  {
  //        cout << data[i] ;
  //      }
  
  
}



void PatternReader::setFileName(string filename)
{
  fn=filename;
}
