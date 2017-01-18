#include "AudioEngine.h"

extern int menu_config_audiopulseclock_out;
#if defined(PSVITA)
void psvitaaudiothread(SceSize argc, void *argp);
#endif

PulseSync::PulseSync()
{
  nb_tick=0;
  nb_tick_before_step_change=0;
  nb_tick_before_step_change_real=0;

  tick_length_high=3;
  tick_length_low=tick_length_high*2;

  tick_length_high=3;
  tick_length_low=3;
  tick_length_high=128;
  tick_length_low=nb_tick_before_step_change_real-128;



  tick_height_std=0;

  //tick_height_low=-8000;
  // tick_height_high=20000;
  // tick_height_low=-20000;

  tick_height_high=18000;
  tick_height_low=-18000;

  tick_height_high=32000;
  tick_height_low=-32000;



  step=0;
}

int PulseSync::setNbTickBeforeStepChange(int val)
{
  nb_tick_before_step_change=val;
  nb_tick_before_step_change_real=val*2;
  tick_length_high=nb_tick_before_step_change_real/2;
  tick_length_high=nb_tick_before_step_change_real/2;
  stepdec1=tick_height_high/tick_length_high;

  // tick_length_low=nb_tick_before_step_change_real-(nb_tick_before_step_change_real/8);
  // stepdec2=tick_length_low/(nb_tick_before_step_change_real/16);

  tick_length_high=(nb_tick_before_step_change_real*1)/2;
  tick_length_low=((nb_tick_before_step_change_real*1)/4)+tick_length_high;

  stepdec2=tick_height_high/(nb_tick_before_step_change_real/3);
  //tick_length_high=nb_tick_before_step_change_real/2;
}

int PulseSync::tick() // volca
{
  //Sint16 out=tick_height_std;
  //Sint16 last=128;
  nb_tick++;
  //printf("nb_tick:%d\n tick_length_high:%d tick_length_low:%d\n",	 nb_tick,	 tick_length_high, 	 tick_length_low);
  if (nb_tick<tick_length_high)
    {  
      out=tick_height_high;
      last_out=out;
    }
    
    if (nb_tick > tick_length_high &&
	nb_tick < tick_length_low)
      {
	out=last_out-stepdec2;
	last_out=out;
      }

    if (nb_tick > tick_length_low)
      {
	out=tick_height_low;
	last_out=out;
      }

        
    if (nb_tick>nb_tick_before_step_change_real)
      {
	DPRINTF("STEP CHANGE\n");
	//nb_tick=0;
	step++;
	if (step>=2)
	  {
	    step=0;
	    nb_tick=0;
	  }
      }

  return out;
}




//#include "SineOscillator.h"

void sdlcallback(void *unused, Uint8 *stream, int len);
int   rtcallback( 
	       void *outputBuffer, 
	       void *inputBuffer, 
	       unsigned int nBufferFrames,
	       double streamTime, 
	       RtAudioStreamStatus status, 
	       void *user_data );

//AudioEngine::AudioEngine() : inst(), AM()
AudioEngine::AudioEngine() : AM(),
			     AD(),
			     buffer_out_left(  new Sint16[INTERNAL_BUFFER_SIZE]),
			     buffer_out_right( new Sint16[INTERNAL_BUFFER_SIZE])

			     //buffer_out( new Sint16[BUFFER_FRAME])
{
  int i;
  callback_called=0;                   // the number of time callback has been called
  freq=DEFAULTFREQ;
  samples=DEFAULTSAMPLES;
  channels=DEFAULTCHANNELS;
  polyphony=DEFAULTPOLYPHONY;
  tick_left=0;
  tick_right=0;

  FORMAT=RTAUDIO_SINT16;
  bufferFrames = 512;          // Weird ?
  bufferFrames = BUFFER_FRAME; // Weird ?
  nbCallback=0;


  dump_audio_env=getenv("DUMP_AUDIO");
  if (dump_audio_env!=NULL)
    {
      if (atoi(dump_audio_env)==1)
	dump_audio=1;
      else
	dump_audio=0;
    }
  
  for (i=0;i<INTERNAL_BUFFER_SIZE;i++)
    buffer_out_right[i]=0;
  for (i=0;i<INTERNAL_BUFFER_SIZE;i++)
    buffer_out_left[i]=0;
  if (dump_audio)
    {
      WFW.setName("audioout.wav");
      WFW.setNbChannel(1);
      WFW.setBitRate(DEFAULTBITRATE);
      WFW.setFrequency(DEFAULTFREQ);
      WFW.createEmptyWaveFile();
    }

  fwrite_byte_counter=0;
  //buffer_out=(Sint16*)malloc(sizeof(Sint16)*BUFFER_FRAME);
  bufferGenerated=0;

  nb_tick=0;
  nb_tick_before_step_change=0;
  //nb_tick_before_six_midi_send_clock=0;
  //nb_tick_before_midi_send_clock=0;
  //nb_tick_midi_send_clock=0;
  //nb_tick_midi_send_clock_mulsix=0;
  seqCallback=0;

  #ifdef __PSVITA_AUDIO__
  AD.sdlAudioSpecWanted->callback=psvitaaudiothread;
  //AD.sdlAudioSpecWanted->userdata=this;
  AD.userdata=this;
  //  DPRINTF("AudioDriver::AudioDriver() this:0x%08.8X",this);
  #endif
  
  #ifdef __SDL_AUDIO__
  AD.sdlAudioSpecWanted->callback=sdlcallback;
  //AD.sdlAudioSpecWanted->userdata=this;
  AD.sdlAudioSpecWanted->userdata=this;
  #endif

  #ifdef __RTAUDIO__
  AD.internal_callback=rtcallback;
  AD.userdata=this;
  #endif
}

int AudioEngine::getNumberOfAudioOutputDevice()
{
  return AD.getNumberOfAudioOutputDevice();
}

void AudioEngine::setAudioOutput(int deviceNumber)
{
  AD.setAudioOutput(deviceNumber);
}

char * AudioEngine::getAudioOutputName(int deviceNumber)
{
  return AD.getAudioOutputName(deviceNumber);
}

int AudioEngine::getNbCallback()
{
  return nbCallback;
}

// related to  sequencer callbackw
int AudioEngine::setNbTickBeforeStepChange(int val) 
{
  nb_tick_before_step_change=val;
  PS.setNbTickBeforeStepChange(val);
}


// a function which need to be trigged when "nb_tick_before_step_change" sample are generated
void AudioEngine::setupSequencerCallback(void (*ptrfunc)(void)) 
{
  seqCallback=ptrfunc;
}

// process BUFFER_FRAME sample and call the callback when needed
void AudioEngine::processBuffer(int len)
{
  //for (int i=0;i<BUFFER_FRAME;i++)
  for (int i=0;i<len;i++)
    {
      nb_tick++;
      processBuffer_updateMidiSendClockCounter();
      if (
	  (nb_tick<nb_tick_before_step_change && 
	   menu_config_midiClockMode!=MENU_CONFIG_Y_MIDICLOCK_SYNCIN
	   )
	  ||
	  (counter_recv_midi_clock_six==0 && 
	   menu_config_midiClockMode==MENU_CONFIG_Y_MIDICLOCK_SYNCIN))
	{
	  //buffer_out_left[i]=AM.tick();
	  //buffer_out_right[i]=buffer_out_left[i];
	  //buffer_out_right[i]=PS.tick();
	  if (menu_config_audiopulseclock_out==0)
	    {
	      buffer_out_right[i]=AM.tick();
	      buffer_out_left[i]=buffer_out_right[i];
	    }
	  else
	    {
	      if (menu_config_audiopulseclock_out==1) // left
		{
		  buffer_out_right[i]=AM.tick();
		  buffer_out_left[i]=PS.tick();
		}
	      if (menu_config_audiopulseclock_out==2) // right
		{
		  buffer_out_left[i]=AM.tick();
		  buffer_out_right[i]=PS.tick();
		}
	    }	  
	}


      // If we have play the same 'step' for 'nb_tick_before_step_change' sample
      //  OR
      // If we are midi slave and we have received the sixth clock
      //  we change to the next step
      if (
	  (nb_tick>=nb_tick_before_step_change &&
	   menu_config_midiClockMode!=MENU_CONFIG_Y_MIDICLOCK_SYNCIN) 
	  ||
	  (counter_recv_midi_clock_six && 
	   menu_config_midiClockMode==MENU_CONFIG_Y_MIDICLOCK_SYNCIN))
        {
	  (*seqCallback)();	 
          nb_tick=0;
	  if (counter_recv_midi_clock_six>0)
	      counter_recv_midi_clock_six--;	  

	  if (menu_config_audiopulseclock_out==0)
	    {
	      buffer_out_right[i]=AM.tick();
	      buffer_out_left[i]=buffer_out_right[i];
	    }
	  else
	    {
	      if (menu_config_audiopulseclock_out==1) // left
		{
		  buffer_out_right[i]=AM.tick();
		  buffer_out_left[i]=PS.tick();
		}
	      if (menu_config_audiopulseclock_out==2) // right
		{
		  buffer_out_left[i]=AM.tick();
		  buffer_out_right[i]=PS.tick();
		}
	    }	  
        }
    }
  bufferGenerated=0;
}


int AudioEngine::bufferIsGenerated()
{
  return bufferGenerated;
}

Sint16 * AudioEngine::getBufferOutLeft()
{
  return buffer_out_left;
}

Sint16 * AudioEngine::getBufferOutRight()
{
  return buffer_out_right;
}



/*
void AudioEngine::setSynthFreq(int sfreq)
{
  S.setFreq(sfreq);
}
*/

AudioMixer & AudioEngine::getAudioMixer()
{
  return AM;
}

void AudioEngine::setDefault()
{
  freq=DEFAULTFREQ;
  samples=DEFAULTSAMPLES;
  channels=DEFAULTCHANNELS;
  polyphony=DEFAULTPOLYPHONY;
}

int AudioEngine::getTickLeft()
{
  return tick_left;
}

int AudioEngine::getTickRight()
{
  return tick_right;
}





//void AudioEngine::sdlcallback(void *unused, Uint8 *stream, int len)
void AudioEngine::callback(void *unused, Uint8 *stream, int len)
{
  //  printf("AudioEngine::calback() begin nBufferFrame=%d nbCallback=%d\n",nBufferFrames,nbCallback);
  //DPRINTF("AudioEngine::callback() AE:0x%08.8X ", this);
  int     buffer_size=0;

  //Workaround a linux sdl 1.2 audio bug 
  //                   sdl seem to have a bug on this...
  #ifdef __PSVITA_AUDIO__
  buffer_size=len;
  #endif
  
  #ifdef __SDL_AUDIO__
  buffer_size=AD.getBufferFrame();
  #ifdef PSP
  //buffer_size=len;
  //buffer_size=64;
  buffer_size=AD.getBufferFrame();
  #endif
  #endif

  //Should be the "Normal case" because we use ...,int len) provided by the caller
  #ifdef __RTAUDIO__
  buffer_size=len;
  #endif

  typedef Sint16 MY_TYPE;
  MY_TYPE *buffer = (MY_TYPE *) stream;
  
  //  this->bufferGenerated=0;
  if (bufferGenerated==0)
    //this->processBuffer(BUFFER_FRAME);
    this->processBuffer(buffer_size);


  if (dump_audio)
    {
      WFW.fillBuffer(buffer_out_right,buffer_size);
    }


    
    //for (int i=0;i<len;i++)
    for (int i=0;i<buffer_size;i++)
    {
      tick_left=buffer_out_left[i];
      tick_right=buffer_out_right[i];
      buffer[(2*i)]=    tick_left;
      buffer[(2*i)+1]=  tick_right;

    }
    callback_called++;
}

#if defined(PSVITA)
void psvitaaudiothread(SceSize argc, void *argp)
{
  void * unused;
  void * stream;
  int i;
  void ** argp_deref = (void **)argp;
  AudioEngine * AE           = argp_deref[0];
  Uint8       * audio_buffer = argp_deref[1];
  int           len          = argp_deref[2];
  int        sce_port        = argp_deref[3];
  DPRINTF("psvitaaudiothread(argc:%d argp[0]:0x%08.8X argp[1]:0x%08.8X argp[2]:0x%08.8X,argp[3]:0x%08.8X)\n",argc,argp_deref[0], argp_deref[1], argp_deref[2],argp_deref[3]);
  while (1)
    {
      int a;
      i++;

      if (i==0)
	DPRINTF("audiothread AE:0x%08.8X AM:0x%08.8X", AE, AE->AM);
      AE->callback(AE,audio_buffer,len);
      sceAudioOutOutput(sce_port, audio_buffer);
    }
}
#endif

void sdlcallback(void *unused, Uint8 *stream, int len)
{
  ((AudioEngine*)unused)->callback(unused,stream,len);
}


int rtcallback( 
	       void *outputBuffer, 
	       void *inputBuffer, 
	       unsigned int nBufferFrames,
	       double streamTime, 
	       RtAudioStreamStatus status, 
	       void *user_data )
{
  //printf("callback\n");
  ((AudioEngine*)user_data)->callback(user_data,
				      (Uint8*)outputBuffer,
				      nBufferFrames);

  return 0;
}



 int AudioEngine::startAudio()
 {
  //return 1;
  //SDL_PauseAudio(0);
   AD.startAudio();
}

// enable audio callback
//int AudioEngine::startAudio()
 //{
  //dac.startStream();
 //}

// disable audio callback
int AudioEngine::stopAudio()
{
  //  dac.stopStream();

}


// disable audio callback
int AudioEngine::stopAudioSdl()
{
  //SDL_CloseAudio();
  //return 1;
  //SDL_PauseAudio(1);
  AD.stopAudio();
}

int AudioEngine::openAudio()
{
  AD.openAudio();
}

int AudioEngine::closeDumpAudioFile()
{
  if (dump_audio)
    WFW.closeAndUpdate();
  
 return 0;
}

int AudioEngine::closeAudio()
{
  //dac.closeStream();  
  AD.closeAudio();
  closeDumpAudioFile();
}

int AudioEngine::closeAudioSdl()
{
  //SDL_LockAudio();
  //SDL_CloseAudio();
  //SDL_QuitSubSystem(SDL_INIT_AUDIO);
}
