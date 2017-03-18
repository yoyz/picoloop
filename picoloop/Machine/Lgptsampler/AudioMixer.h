#ifndef _AUDIO_MIXER_H_
#define _AUDIO_MIXER_H_

#include "AudioModule.h"
#include "T_SimpleList.h"
#include "WavFileWriter.h"
#include <string>

class AudioMixer: public AudioModule,public T_SimpleList<AudioModule> {
public:
	AudioMixer(const char *name) ;
	virtual ~AudioMixer() ;
	virtual bool Render(lgptfixed *buffer,int samplecount) ;
	void SetFileRenderer(const char *path) ;
	void EnableRendering(bool enable) ;
	void SetVolume(lgptfixed volume) ;
private:
	bool enableRendering_ ;
	std::string renderPath_ ;
	WavFileWriter *writer_ ;
	lgptfixed volume_ ;
	std::string name_ ; 
} ;
#endif
