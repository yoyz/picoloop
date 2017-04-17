#include "SamplePool.h"
#include <string.h>
#include <stdlib.h>
#include "Trace.h"
#include "PersistencyService.h" 
#include "Status.h"
#include <string>
#include "Config.h"

#define SAMPLE_LIB "root:samplelib" 

SamplePool::SamplePool() {
	for (int i=0;i<MAX_PIG_SAMPLES;i++) {
		names_[i]=NULL ;
		wav_[i]=NULL ;
	} ;
	count_=0 ;
} ;

SamplePool::~SamplePool() {
	for (int i=0;i<MAX_PIG_SAMPLES;i++) {
	  if (wav_[i])    delete(wav_[i]);
	  if (names_[i])  free(names_[i]);
	} ;
} ;

void SamplePool::Reset() {
	count_=0 ;
	for (int i=0;i<MAX_PIG_SAMPLES;i++) {
	  if (wav_[i])    delete(wav_[i]);
	  if (names_[i])  free(names_[i]);
	} ;
} ;

SoundSource *SamplePool::GetSource(int i) {
	return wav_[i] ;
} ;

char **SamplePool::GetNameList() {
	return names_ ;
} ;

int SamplePool::GetNameListSize() {
	return count_ ;
} ;

bool SamplePool::loadSample(int index,const char *path)
{
  if (count_==MAX_PIG_SAMPLES) return false ;
  //Path sPath(path) ;
  //Path wavPath(path) ;
  WavFile *wave=WavFile::Open(path) ;
  if (wave)
    {
      wav_[index]=wave ;
      //const std::string name=wavPath.GetName() ;
      const std::string name=std::string(path);
      names_[count_]=(char*)malloc(sizeof(char)*name.length()+1) ;
      strcpy(names_[count_],name.c_str()) ;
      count_++ ;
      wave->GetBuffer(0,wave->GetSize(-1)) ;
      wave->Close() ;
      return true ;
    }
  else
    {
      //printf("Failed to load samples %s",wavPath.GetName().c_str());
      printf("Failed to load samples %s",path);
      return false ;
    }
}
