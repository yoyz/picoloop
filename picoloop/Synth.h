#include <SDL/SDL.h>
#include <SDL/SDL_audio.h>
#include <queue>
#include <cmath>

#ifndef __SYNTH____
#define __SYNTH____



class Synth
{
private:
    double v;
    std::queue<BeepObject> beeps;
public:
    Synth();
    ~Synth();
    void initialize(double freq, int duration);
    void generateSamples(Uint8 *stream, int length);
    void wait();
};

#endif
