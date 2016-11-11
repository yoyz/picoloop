#include "AudioEngine.h"

extern int menu_config_audiopulseclock_out;


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
/*
int PulseSync::tick() // volca
{
  Sint16 out=tick_height_std;
  Sint16 out=tick_height_std;
  nb_tick++;
  if (nb_tick<tick_length_low)
    {
      out=tick_height_high;
    }
    
    if (nb_tick >= tick_length_low &&
	nb_tick < tick_length_high)
      {
	out=tick_height_low;
      }
    
    if (nb_tick >= tick_length_high)
      {
	out=tick_height_std;
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
*/
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

  // Debug audio allow to dump audio to audioout file which is a raw file
  #ifndef DUMP_AUDIO
  dump_audio=0;
  #else
  dump_audio=DUMP_AUDIO;
  #endif
  
  for (i=0;i<INTERNAL_BUFFER_SIZE;i++)
    buffer_out_right[i]=0;
  for (i=0;i<INTERNAL_BUFFER_SIZE;i++)
    buffer_out_left[i]=0;

  if (dump_audio) 
    {
      fd = fopen("audioout.wav","w");
      //waveHeader.riff=0x46464952;          // "RIFF"
      waveHeader.riff[0]='R';          // "RIFF"
      waveHeader.riff[1]='I';          // "RIFF"
      waveHeader.riff[2]='F';          // "RIFF"
      waveHeader.riff[3]='F';          // "RIFF"
      waveHeader.riffLength=0;             // 0 - 8 => 0
      //waveHeader.wave=0x45564157;          // "WAVE"
      waveHeader.wave[0]='W';
      waveHeader.wave[1]='A';
      waveHeader.wave[2]='V';
      waveHeader.wave[3]='E';
      //waveHeader.fmt=0x20746D66;           // (0x66,0x6D, 0x74,0x20)
      waveHeader.fmt[0]=0x66;
      waveHeader.fmt[1]=0x6d;
      waveHeader.fmt[2]=0x74;
      waveHeader.fmt[3]=0x20;
      waveHeader.waveLength=16;            // allways 16 ?
      waveHeader.wFormatTag=1;             // 1==PCM
      waveHeader.nChannels=1;              // 1==mono
      waveHeader.frequency=DEFAULTFREQ;    // 44100 defined in Master.h
      waveHeader.bytePerSec=DEFAULTFREQ*2; // 
      waveHeader.bytePerBloc=2;            // 
      waveHeader.wBitsPerSample=16;        //  16 bit
      //waveHeader.data=0x61746164;          // «data»  (0x64,0x61,0x74,0x61)
      waveHeader.data[0]='d';               // «data»  (0x64,0x61,0x74,0x61)
      waveHeader.data[1]='a';               // «data»  (0x64,0x61,0x74,0x61)
      waveHeader.data[2]='t';               // «data»  (0x64,0x61,0x74,0x61)
      waveHeader.data[3]='a';               // «data»  (0x64,0x61,0x74,0x61)
      waveHeader.dataLength=0;              // fileSize - sizeof(WAVEHEADER)
      
      fwrite(&waveHeader,sizeof(WHAE),1,fd);
      fflush(fd);
    }
  fwrite_byte_counter=0;
  //buffer_out=(Sint16*)malloc(sizeof(Sint16)*BUFFER_FRAME);
  bufferGenerated=0;

  nb_tick=0;
  nb_tick_before_step_change=0;
  nb_tick_before_six_midi_send_clock=0;
  nb_tick_before_midi_send_clock=0;
  nb_tick_midi_send_clock=0;
  nb_tick_midi_send_clock_mulsix=0;
  seqCallback=0;

  #ifdef __SDL_AUDIO__
  AD.sdlAudioSpecWanted->callback=sdlcallback;
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
  nb_tick_before_midi_send_clock=(val/6);
  nb_tick_before_six_midi_send_clock=val;
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
      if (menu_config_midiClockMode==MENU_CONFIG_Y_MIDICLOCK_SYNCOUT)
	{
	  nb_tick_midi_send_clock++;
	  nb_tick_midi_send_clock_mulsix++;

	  // arm a counter to send the six midi sync signal
	  // there is a / 6 and it prevent midi clock skew
	  if (nb_tick_midi_send_clock_mulsix>nb_tick_before_six_midi_send_clock-1)
	    {
	      counter_send_midi_clock_six++;

	      // This trick allow to transpose the send of the midi clock
	      // it can be used on a system which buffer too much audio
	      // it is linked to the BPM menu
	      nb_tick_midi_send_clock=-counter_delta_midi_clock*50;
	      nb_tick_midi_send_clock_mulsix=-counter_delta_midi_clock*50;

	      midi_tick_number=0;
	      counter_delta_midi_clock=0;
	    }
      
	  // arm a counter to send a midi sync signal
	  if (midi_tick_number<5 &&
	      nb_tick_midi_send_clock>nb_tick_before_midi_send_clock-1)
	    {
	      counter_send_midi_clock++;
	      nb_tick_midi_send_clock=0;
	      midi_tick_number++;
	    }
	}
      // end sync out

      

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

      if (
	  (nb_tick>=nb_tick_before_step_change &&
	   menu_config_midiClockMode!=MENU_CONFIG_Y_MIDICLOCK_SYNCIN) 
	  ||
	  (counter_recv_midi_clock_six && 
	   menu_config_midiClockMode==MENU_CONFIG_Y_MIDICLOCK_SYNCIN))
        {
          //printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!CALLL\n");                                                                                                                      
          //PP->seq_callback_update_step();
	  if (seqCallback)
	    (*seqCallback)();
          nb_tick=0;
	  counter_recv_midi_clock=0;
	  counter_recv_midi_clock_six=0;

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

	  /*	  
	  buffer_out_right[i]=AM.tick();
#ifdef __VOLCASYNC__
	  buffer_out_left[i]=PS.tick();
#else
	  buffer_out_left[i]=buffer_out_right[i];
#endif
	  */
	  // buffer_out_right[i]=AM.tick();
	  // buffer_out_left[i]=PS.tick();
	  //buffer_out_right[i]=buffer_out_left[i];
	  //buffer_out_left[i]=AM.tick();
	  //buffer_out_right[i]=buffer_out_left[i];
	  //buffer_out_right[i]=PS.tick();
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
  //printf("AudioEngine::callback() len=%d\n",len);
  int     buffer_size;
  //int     buffer_size=len;

  //Workaround a linux sdl 1.2 audio bug 
  //                   sdl seem to have a bug on this...
  #ifdef __SDL_AUDIO__
  //buffer_size=BUFFER_FRAME;
  buffer_size=AD.getBufferFrame();
  //buffer_size=len;
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

  #ifdef DUMP_AUDIO
  if (dump_audio)
    {
      //if (callback_called>128)
      fwrite_byte_counter+=fwrite(buffer_out_right,buffer_size,sizeof(Sint16),fd)*buffer_size;
      // fseek(fd,0,SEEK_SET);
      // waveHeader.riffLength=fwrite_byte_counter+44-8; // size of file - 8
      // waveHeader.dataLength=fwrite_byte_counter-44;   // data - size of header
      // fwrite(&waveHeader,44,sizeof(WHAE),fd);
      // fclose(fd);
      // exit(0);
    }
  #endif

    
    //for (int i=0;i<len;i++)
    for (int i=0;i<buffer_size;i++)
    {
      //int tick = S.tick();
      //      int tick = AM.tick();
      //Sint16 tick = AM.tick();
      tick_left=buffer_out_left[i];
      tick_right=buffer_out_right[i];
      //      printf("%d\n",tick);
      /*
      #ifdef LINUX_DESKTOP
      if (debug_audio)
	{	  
	  fwrite(&tick,1,sizeof(Sint16),fd);
	  fwrite(&tick,1,sizeof(Sint16),fd);
	  //printf("%d\t",tick);
	}
      #endif
      */
      buffer[(2*i)]=    tick_left;
      buffer[(2*i)+1]=  tick_right;

      //buffer[i+1]=tick;
      //buffer[i+1]=0;
      //i++;
    }
  //if (debug_audio)
  //fwrite(buffer,sizeof(MY_TYPE)*nBufferFrames,sizeof(MY_TYPE),fd);

  //  printf("AudioEngine::calback() end\n");
  //bufferGenerated=0;
  //return nbCallback++;  
    callback_called++;
}

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
   {
     printf("fwrite_byte_counter:%d\n",fwrite_byte_counter);
     fseek(fd,0,SEEK_SET);
     //waveHeader.riffLength=fwrite_byte_counter+36;   // data+header-8
     waveHeader.riffLength=fwrite_byte_counter+36;   // data+header-8
     waveHeader.dataLength=fwrite_byte_counter;      // data
     fwrite(&waveHeader,sizeof(WHAE),1,fd);
     fclose(fd);
   }
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
