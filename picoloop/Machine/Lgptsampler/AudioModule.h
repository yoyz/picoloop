#ifndef _AUDIO_MODULE_H_
#define _AUDIO_MODULE_H_

#include "lgptfixed.h"

class AudioModule {
public:
      virtual ~AudioModule() {} ;
      virtual bool Render(lgptfixed *buffer,int samplecount)=0 ;
} ;

#endif
