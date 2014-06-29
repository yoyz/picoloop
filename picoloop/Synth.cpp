#include "Synth.h"

const int AMPLITUDE = 28000;
const int FREQUENCY = 44100;

struct BeepObject
{
    double freq;
    int samplesLeft;
};


Synth::Synth()
{
  /*
    SDL_AudioSpec desiredSpec;

    desiredSpec.freq = FREQUENCY;
    desiredSpec.format = AUDIO_S16SYS;
    desiredSpec.channels = 1;
    desiredSpec.samples = 2048;
    desiredSpec.callback = audio_callback;
    desiredSpec.userdata = this;

    SDL_AudioSpec obtainedSpec;

    // you might want to look for errors here
    SDL_OpenAudio(&desiredSpec, &obtainedSpec);

    // start play audio
    SDL_PauseAudio(0);
  */
}

Synth::~Synth()
{
    //SDL_CloseAudio();
}

/*
void Synth::generateSamples(Uint8 *stream, int length)
{
  int i=0;
  double v=0;
  Uint16 a; 
  Uint16 * _stream=(Uint16*)stream;
  printf("l:%d\n",length);
  while(i<length)
    {
      a=Uint16(AMPLITUDE * float(std::sin(i*FREQUENCY)));
      _stream[i]=a;
      //stream[i]=i/8;
      //a=int(float(std::sin(i)*AMPLITUDE));
      //stream[i]=a;
      i++;
      v=v+44100;
      //printf("a:%d\n",a);
    }
}
*/


void Synth::generateSamples(Uint8 *stream, int length)
{
  Uint16 * _stream=(Uint16*)stream;
  int i = 0;
  while (i < length) 
      {
	
        if (beeps.empty()) 
	  {
	    while (i < length) 
	      {
                _stream[i] = 0;
                i++;
	      }
            return;
	  }
        BeepObject& bo = beeps.front();

        int samplesToDo = std::min(i + bo.samplesLeft, length);
        bo.samplesLeft -= samplesToDo - i;

        while (i < samplesToDo) 
	  {
	    _stream[i] = AMPLITUDE * std::sin(v * 2 * M_PI / FREQUENCY);
	    i++;
	    v += bo.freq;
	  }
	
        if (bo.samplesLeft == 0) 
	  {
	    beeps.pop();
	  }
      }
}

void Synth::initialize(double freq, int duration)
{
    BeepObject bo;
    bo.freq = freq;
    bo.samplesLeft = duration * FREQUENCY / 1000;

    SDL_LockAudio();
    beeps.push(bo);
    SDL_UnlockAudio();
}

void Synth::wait()
{
    int size;
    do {
        SDL_Delay(20);
        SDL_LockAudio();
        size = beeps.size();
        SDL_UnlockAudio();
    } while (size > 0);

}

/*
void audio_callback(void *_beeper, Uint8 *_stream, int _length)
{
    Sint16 *stream = (Sint16*) _stream;
    int length = _length / 2;
    Beeper* beeper = (Beeper*) _beeper;

    beeper->generateSamples(stream, length);
}


int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_AUDIO);

    int duration = 1000;
    double Hz = 440;

    Beeper b;
    b.beep(Hz, duration);
    b.wait();

    return 0;
}
*/
