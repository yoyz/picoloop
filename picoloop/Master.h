#ifndef __MASTER____
#define __MASTER____
#include <SDL/SDL.h>


#define INTERNAL_BUFFER_SIZE 128*1024   
#define DEFAULTFREQ      44100
#define DEFAULTBITRATE   16
//#define DEFAULTSAMPLES   512
//#define DEFAULTSAMPLES   1024
#define DEFAULTSAMPLES   2048
#define DEFAULTCHANNELS  2
#define DEFAULTPOLYPHONY 16
#define DEFAULTFORMAT    AUDIO_S16

#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT	240
#define SCREEN_DEPTH	16
#define SCREEN_MULT     1
//#define WAVETABLE_SIZE  1024*16
#define WAVETABLE_SIZE  1024*16
//#define WAVETABLE_SIZE  128
#define FPU 0

//#define MAX_PATTERN_BY_PROJECT 256
//#define MAX_PATTERN_BY_PROJECT 16
#define MAX_PATTERN_BY_PROJECT 256
#define MAX_BANK 256      
#define TRACK_MAX 4

#define BUFFER_FRAME DEFAULTSAMPLES
//#define DEFAULT_FREQ 44100


//INTERNAL

#define FILTER_TYPE_LOWPASS   0
#define FILTER_TYPE_BANDPASS  1
#define FILTER_TYPE_HIPASS    2

#define FILTER_ALGO_NOFILTER  0
#define FILTER_ALGO_BIQUAD    1
#define FILTER_ALGO_AMSYNTH   2



#ifdef OPENDINGUX
// BEGIN DINGOO A320 SDL
#define BUTTON_B            SDLK_LALT
#define BUTTON_A            SDLK_LCTRL
#define BUTTON_X            SDLK_SPACE
#define BUTTON_Y            SDLK_LSHIFT

#define BUTTON_UP           SDLK_UP
#define BUTTON_DOWN         SDLK_DOWN
#define BUTTON_LEFT         SDLK_LEFT
#define BUTTON_RIGHT        SDLK_RIGHT

#define BUTTON_SELECT       SDLK_ESCAPE
#define BUTTON_START        SDLK_RETURN
// BEGIN DINGOO A320 SDL
#endif

#ifdef LINUX_DESKTOP
// BEGIN DINGOO A320 SDL use by linux too
#define BUTTON_B            SDLK_LALT
#define BUTTON_A            SDLK_LCTRL
#define BUTTON_X            SDLK_SPACE
#define BUTTON_Y            SDLK_LSHIFT

#define BUTTON_UP           SDLK_UP
#define BUTTON_DOWN         SDLK_DOWN
#define BUTTON_LEFT         SDLK_LEFT
#define BUTTON_RIGHT        SDLK_RIGHT

#define BUTTON_SELECT       SDLK_ESCAPE
#define BUTTON_START        SDLK_RETURN
// BEGIN DINGOO A320 SDL
#endif


#ifdef PSP

#define PSP_BUTTON_TRIANGLE 0 // X
#define PSP_BUTTON_CIRCLE   1 // A
#define PSP_BUTTON_CROSS    2 // B
#define PSP_BUTTON_SQUARE   3 // Y
#define PSP_BUTTON_L        4
#define PSP_BUTTON_R        5
#define PSP_BUTTON_DOWN     6
#define PSP_BUTTON_LEFT     7
#define PSP_BUTTON_UP       8
#define PSP_BUTTON_RIGHT    9
#define PSP_BUTTON_SELEC    10
#define PSP_BUTTON_START    11


// BEGIN PSP SDL 
#define BUTTON_B            PSP_BUTTON_CROSS
#define BUTTON_A            PSP_BUTTON_CIRCLE
#define BUTTON_X            PSP_BUTTON_TRIANGLE
#define BUTTON_Y            PSP_BUTTON_SQUARE

#define BUTTON_UP           PSP_BUTTON_UP
#define BUTTON_DOWN         PSP_BUTTON_DOWN
#define BUTTON_LEFT         PSP_BUTTON_LEFT
#define BUTTON_RIGHT        PSP_BUTTON_RIGHT

#define BUTTON_SELECT       PSP_BUTTON_SELECT
#define BUTTON_START        PSP_BUTTON_START
// BEGIN PSP SDL
#endif


#ifdef GP2X

#define GP2X_BUTTON_UP              0
#define GP2X_BUTTON_DOWN            4
#define GP2X_BUTTON_LEFT            2
#define GP2X_BUTTON_RIGHT           6
#define GP2X_BUTTON_UPLEFT          1
#define GP2X_BUTTON_UPRIGHT         7
#define GP2X_BUTTON_DOWNLEFT        3
#define GP2X_BUTTON_DOWNRIGHT       5
#define GP2X_BUTTON_CLICK           18
#define GP2X_BUTTON_A               12
#define GP2X_BUTTON_B               13
#define GP2X_BUTTON_X               14
#define GP2X_BUTTON_Y               15
#define GP2X_BUTTON_L               10
#define GP2X_BUTTON_R               11
#define GP2X_BUTTON_START           8
#define GP2X_BUTTON_SELECT          9
#define GP2X_BUTTON_VOLUP           16
#define GP2X_BUTTON_VOLDOWN         17

// BEGIN GP2X SDL 
#define BUTTON_B            GP2X_BUTTON_B
#define BUTTON_A            GP2X_BUTTON_A
#define BUTTON_X            GP2X_BUTTON_X
#define BUTTON_Y            GP2X_BUTTON_Y

#define BUTTON_UP           GP2X_BUTTON_UP
#define BUTTON_DOWN         GP2X_BUTTON_DOWN
#define BUTTON_LEFT         GP2X_BUTTON_LEFT
#define BUTTON_RIGHT        GP2X_BUTTON_RIGHT

#define BUTTON_SELECT       GP2X_BUTTON_SELECT
#define BUTTON_START        GP2X_BUTTON_START
// BEGIN GP2X SDL
#endif





enum {
  FMTYPE_2_OP_FM,
  FMTYPE_2_OP_AM
};

enum 
  {
    SYNTH_PICOSYNTH,
    SYNTH_OPL2,
    SYNTH_PICODRUM
  };

enum 
  {
    PICO_WAVETABLE_SINE,
    PICO_WAVETABLE_SAW,
    PICO_WAVETABLE_PULSE,
    PICO_WAVETABLE_TRGL,
    PICO_WAVETABLE_NOISE,
  };



#endif
