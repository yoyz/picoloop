#ifndef __MASTER____
#define __MASTER____

#if !defined(__SDL20__) && !defined(__SDL12__)
#error "SDL1.2 or SDL2.0 are mandatory, you should put -D__SDL12__ or -D__SDL20__ in the CFLAGS"
#endif

#ifdef   __SDL12__
#include <SDL/SDL.h>
#endif
#ifdef  __SDL20__
#include <SDL2/SDL.h>
#endif


#define PICOLOOP_VERSION "0.77d"
//#define INTERNAL_BUFFER_SIZE 128*1024   
#define INTERNAL_BUFFER_SIZE 16*1024   
//#define DEFAULTFREQ      44100
#define DEFAULTFREQ      44100
#define DEFAULTBITRATE   16
#define PORTED_MACHINE   11
//#define DEFAULTSAMPLES   512
//#define DEFAULTSAMPLES   1024
//#define DEFAULTSAMPLES   128
#define DEFAULTCHANNELS  2
#define DEFAULTPOLYPHONY 16
#define DEFAULTFORMAT    AUDIO_S16

//#define SCREEN_WIDTH	320
//#define SCREEN_HEIGHT	240
//#define SCREEN_DEPTH	16

//#define WAVETABLE_SIZE  1024*16
//#define WAVETABLE_SIZE  1024*16
#define WAVETABLE_SIZE  1024*16
//#define WAVETABLE_SIZE  1024
//#define WAVETABLE_SIZE  128
//#define FPU 0

//#define MAX_PATTERN_BY_PROJECT 256
//#define MAX_PATTERN_BY_PROJECT 16
#define MAX_SONG_LENGHT_BY_PROJECT 1024
#define MAX_BANK 256
#define TRACK_MAX 4
#define MAX_STEP_PER_TRACK 128

//#define BUFFER_FRAME DEFAULTSAMPLES
//#define DEFAULT_FREQ 44100

#define DEFAULT_VOLUME   64


#define KEY_REPEAT_INTERVAL 400

/*
#define DISABLEDBOX_COLOR   0x0AF0FE
#define BOX_COLOR           0xAECD15
#define TRIG_COLOR          0x0E4C15
#define NOTE_COLOR          0x46DC65
#define CURSOR_COLOR        0x1515CD
#define STEP_COLOR          0x242C45
#define SMALLBOX_COLOR      0x442233
*/

#define DISABLEDBOX_COLOR   0
#define ENABLEDBOX_COLOR    1
#define TRIG_COLOR          2
#define NOTE_COLOR          3
#define CURSOR_COLOR        4
#define STEP_COLOR          5
#define SMALLBOX_COLOR      6
#define TEXTCOLOR           7
#define BACKGROUND          8
#define MAXCOLOR            9



#define DISABLE             0
#define ENABLE              1

#define MENU_OFF      0
#define MENU_ON_PAGE1 1
#define MENU_ON_PAGE2 2


#define MENU_CONFIG_Y_MIDICLOCK_SYNCINTERNAL 0
#define MENU_CONFIG_Y_MIDICLOCK_SYNCOUT      1
#define MENU_CONFIG_Y_MIDICLOCK_SYNCIN       2
#define MENU_CONFIG_Y_MIDICLOCK_SYNCMAX      2


//menu
/* enum { */
/*   MENU_OFF, */
/*   MENU_ON_PAGE1, */
/*   MENU_ON_PAGE2 */
/* }; */

#define CURSOR_LOADSAVE 0
#define CURSOR_SONG     1

/* enum { */
/*   CURSOR_LOADSAVE, */
/*   CURSOR_SONG */
/* }; */
  

//menu_cursor

#define GLOBALMENU_AD         0
#define GLOBALMENU_NOTE       1
#define GLOBALMENU_OSC        2
#define GLOBALMENU_VCO        3
#define GLOBALMENU_LFO        4
#define GLOBALMENU_FLTR       5

#define GLOBALMENU_LS         6
#define GLOBALMENU_BANK       7
#define GLOBALMENU_PSH        8
#define GLOBALMENU_MAC        9
#define GLOBALMENU_FX         10
#define GLOBALMENU_BPM        11



// BEGIN MENU AD
//menu_ad
enum {
  MENU_AD_AMP_ATTACK_RELEASE,
  MENU_AD_AMP_DECAY_SUSTAIN,
  MENU_AD_FLTR_ATTACK_RELEASE,
  MENU_AD_FLTR_DECAY_SUSTAIN,
  MENU_AD_TRIGTIME_AMP
};

enum {
  MENU_PAGE0_SUB0,
  MENU_PAGE0_SUB1,
  MENU_PAGE0_SUB2,
  MENU_PAGE0_SUB3,
  MENU_PAGE0_SUB4,
  MENU_PAGE0_SUB5
};
// END   MENU AD

enum {
  MENU_FLTR_CUTOFF_RESONANCE,
  MENU_FLTR_ALGO_TYPE, 
  MENU_FLTR_SATURATION_FEEDBACK
};

enum { // used by cursynth and twytch
  MENU_VCO_OSCMIX_VELOCITY,
  MENU_VCO_KEYTRACKENVDEPTH,
  MENU_VCO_OSCDETUNECROSSMOD,
  MENU_VCO_OSC3AMPOSC4AMP
};

enum { // used by picosynth/picodrum/Dbopl
  MENU_VCO_OSCMIX_PHASE,
  MENU_VCO_OSCAMP,
  MENU_VCO_FMTYPE
};




enum {
  MENU_OSC_OSC1OSC2,
  MENU_OSC_OSC3,
  MENU_OSC_OSC1DETUNE_OSC2DETUNE,
  MENU_OSC_LFO1LFO2,
  MENU_OSC_UNISON1UNISON2,
  MENU_OSC_UNISONDT1UNISONDT2,
  MENU_OSC_OSC1SCALE_OSC2SCALE,
  MENU_OSC_OSC2SCALE
};


enum {
  MENU_LFO_LFOPITCH,
  MENU_LFO_PITCHBEND,
  MENU_LFO_TYPE,
  MENU_LFO_FM
};


enum {
  MENU_CURSYNTHLFO_SRC_AMPENV,          // amp env
  MENU_CURSYNTHLFO_SRC_FILTERENV,       // filter env
  MENU_CURSYNTHLFO_SRC_LFO1,            // lfo 1
  MENU_CURSYNTHLFO_SRC_LFO2,            // lfo 2
  MENU_CURSYNTHLFO_SRC_MODWHEEL,        // mod wheel
  MENU_CURSYNTHLFO_SRC_NOTE,            // note
  MENU_CURSYNTHLFO_SRC_PITCHWHEEL,      // pitch wheel
  MENU_CURSYNTHLFO_SRC_VELOCITY,        // velocity
  MENU_CURSYNTHLFO_SRC_SIZE      
};


enum {
  MENU_CURSYNTHLFO_DST_CROSSMODULATION, // cross modulation 
  MENU_CURSYNTHLFO_DST_CUTOFF,          // cutoff
  MENU_CURSYNTHLFO_DST_OSCMIX,          // osc mix
  MENU_CURSYNTHLFO_DST_PITCH,           // pitch
  MENU_CURSYNTHLFO_DST_RESONANCE,       // resonance
  MENU_CURSYNTHLFO_DST_SIZE
};


enum {
  MENU_FX_DEPTH_SPEED
};

enum {
  MENU_LS_PATTERN,
  MENU_LS_SONG
};



//INTERNAL

/* #define FILTER_TYPE_LOWPASS   0 */
/* #define FILTER_TYPE_BANDPASS  1 */
/* #define FILTER_TYPE_HIPASS    2 */

/* #define FILTER_ALGO_NOFILTER  0 */
/* #define FILTER_ALGO_BIQUAD    1 */
/* #define FILTER_ALGO_AMSYNTH   2 */



#ifdef OPENDINGUX
//#define __FPU__         1
//#define __RAM512MIB__   1
//#define __RTMIDI__      1
#define MAX_PATTERN_BY_PROJECT 32
#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT	240
#define SCREEN_DEPTH	16
#define SCREEN_MULT     1

#define KEY_REPEAT_INTERVAL_SMALLEST  4
#define KEY_REPEAT_INTERVAL_SMALL     8
#define KEY_REPEAT_INTERVAL_MIDDLE    32
#define KEY_REPEAT_INTERVAL_LONG      64
#define KEY_REPEAT_INTERVAL_LONGEST   128

#define DEFAULTSAMPLES   2048

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

#define BUTTON_L            SDLK_TAB
#define BUTTON_R            SDLK_BACKSPACE

#define BUTTON_POWER        SDLK_POWER
#define BUTTON_PAUSE        SDLK_PAUSE

#define KEYPRESSED          SDL_KEYDOWN
#define KEYRELEASED         SDL_KEYUP

// BEGIN DINGOO A320 SDL
#endif

#ifdef PC_DESKTOP
#define MAX_PATTERN_BY_PROJECT 128

#define __RTMIDI__      1
#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT	240
#define SCREEN_DEPTH	16

#ifndef SCREEN_MULT 
#define SCREEN_MULT     1
#endif



// BEGIN DINGOO A320 SDL use by linux too

//#define DEFAULTSAMPLES   2048

// Fine tuning 2048 for MIDI_DELAY_IN_SAMPLE and 512 for DEFAULTSAMPLES
#define MIDI_DELAY_IN_SAMPLE 2048
#define DEFAULTSAMPLES   512

/* #define MIDI_DELAY_IN_SAMPLE 8192 */
/* #define DEFAULTSAMPLES   2048 */


#define KEY_REPEAT_INTERVAL_SMALLEST  4
#define KEY_REPEAT_INTERVAL_SMALL     8
#define KEY_REPEAT_INTERVAL_MIDDLE    32
#define KEY_REPEAT_INTERVAL_LONG      64
#define KEY_REPEAT_INTERVAL_LONGEST   128



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

#define BUTTON_L            SDLK_TAB
#define BUTTON_R            SDLK_BACKSPACE

#define KEYPRESSED          SDL_KEYDOWN
#define KEYRELEASED         SDL_KEYUP

// BEGIN DINGOO A320 SDL
#endif

#ifdef  RASPI3_ILI9486 
#define MAX_PATTERN_BY_PROJECT 128
//#define SCREEN_WIDTH	480
//#define SCREEN_HEIGHT	320
#define SCREEN_WIDTH	640 
#define SCREEN_HEIGHT	480
#define SCREEN_DEPTH	16
#ifndef SCREEN_MULT 
#define SCREEN_MULT     2
#endif
#define MIDI_DELAY_IN_SAMPLE 2048
#define DEFAULTSAMPLES        128

/* #define MIDI_DELAY_IN_SAMPLE 8192 */
/* #define DEFAULTSAMPLES   2048 */


#define KEY_REPEAT_INTERVAL_SMALLEST  16
#define KEY_REPEAT_INTERVAL_SMALL     30
#define KEY_REPEAT_INTERVAL_MIDDLE    48
#define KEY_REPEAT_INTERVAL_LONG      128
#define KEY_REPEAT_INTERVAL_LONGEST   192
/*
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

#define BUTTON_L            SDLK_TAB
#define BUTTON_R            SDLK_BACKSPACE

#define KEYPRESSED          SDL_KEYDOWN
#define KEYRELEASED         SDL_KEYUP
*/
#define BUTTON_B            PS3_BUTTON_X
#define BUTTON_A            PS3_BUTTON_O
#define BUTTON_X            PS3_BUTTON_SQUARE
#define BUTTON_Y            PS3_BUTTON_TRIANGLE

#define BUTTON_UP           PS3_BUTTON_UP
#define BUTTON_DOWN         PS3_BUTTON_DOWN
#define BUTTON_LEFT         PS3_BUTTON_LEFT
#define BUTTON_RIGHT        PS3_BUTTON_RIGHT

#define BUTTON_SELECT       PS3_BUTTON_SELECT
#define BUTTON_START        PS3_BUTTON_START

#define BUTTON_L            PS3_BUTTON_L1
#define BUTTON_R            PS3_BUTTON_R1

#define KEYPRESSED          SDL_JOYBUTTONDOWN
#define KEYRELEASED         SDL_JOYBUTTONUP

#endif


#ifdef LINUX_RASPI1
/*
#define ENABLE_SYNTH_PICOSYNTH
#define ENABLE_SYNTH_OPL2
#define ENABLE_SYNTH_PICODRUM
#define ENABLE_SYNTH_PBSYNTH
*/
#define MAX_PATTERN_BY_PROJECT 128

#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT	240
#define SCREEN_DEPTH	16
#define SCREEN_MULT     2
// BEGIN DINGOO A320 SDL use by linux too

//#define DEFAULTSAMPLES   2048

// Fine tuning 2048 for MIDI_DELAY_IN_SAMPLE and 512 for DEFAULTSAMPLES
#define MIDI_DELAY_IN_SAMPLE 2048
#define DEFAULTSAMPLES   2048 // Raspberry 1 can not handle 512 sample

/* #define MIDI_DELAY_IN_SAMPLE 8192 */
/* #define DEFAULTSAMPLES   2048 */


#define KEY_REPEAT_INTERVAL_SMALLEST  4
#define KEY_REPEAT_INTERVAL_SMALL     8
#define KEY_REPEAT_INTERVAL_MIDDLE    32
#define KEY_REPEAT_INTERVAL_LONG      64
#define KEY_REPEAT_INTERVAL_LONGEST   128



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

#define BUTTON_L            SDLK_TAB
#define BUTTON_R            SDLK_BACKSPACE

#define KEYPRESSED          SDL_KEYDOWN
#define KEYRELEASED         SDL_KEYUP


/*
#define BUTTON_B            PS3_BUTTON_X
#define BUTTON_A            PS3_BUTTON_O
#define BUTTON_X            PS3_BUTTON_SQUARE
#define BUTTON_Y            PS3_BUTTON_TRIANGLE

#define BUTTON_UP           PS3_BUTTON_UP
#define BUTTON_DOWN         PS3_BUTTON_DOWN
#define BUTTON_LEFT         PS3_BUTTON_LEFT
#define BUTTON_RIGHT        PS3_BUTTON_RIGHT

#define BUTTON_SELECT       PS3_BUTTON_SELECT
#define BUTTON_START        PS3_BUTTON_START

#define BUTTON_L            PS3_BUTTON_L1
#define BUTTON_R            PS3_BUTTON_R1

#define KEYPRESSED          SDL_JOYBUTTONDOWN
#define KEYRELEASED         SDL_JOYBUTTONUP
*/

// BEGIN DINGOO A320 SDL
#endif


#ifdef LINUX_PBSYNTHONLY
//#define __FPU__         1
//#define __RAM512MIB__   0
//#define __RTMIDI__      1
#define ENABLE_SYNTH_PBSYNTH
#define            __PBSYNTHONLY__ 1
//int machine_tab[PORTED_MACHINE]={0,0,0};
#define MAX_PATTERN_BY_PROJECT 128

#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT	240
#define SCREEN_DEPTH	16
#define SCREEN_MULT     2
// BEGIN DINGOO A320 SDL use by linux too

//#define DEFAULTSAMPLES   2048

// Fine tuning 2048 for MIDI_DELAY_IN_SAMPLE and 512 for DEFAULTSAMPLES
#define MIDI_DELAY_IN_SAMPLE 2048
#define DEFAULTSAMPLES   2048 // Raspberry 1 can not handle 512 sample

/* #define MIDI_DELAY_IN_SAMPLE 8192 */
/* #define DEFAULTSAMPLES   2048 */


#define KEY_REPEAT_INTERVAL_SMALLEST  4
#define KEY_REPEAT_INTERVAL_SMALL     8
#define KEY_REPEAT_INTERVAL_MIDDLE    32
#define KEY_REPEAT_INTERVAL_LONG      64
#define KEY_REPEAT_INTERVAL_LONGEST   128



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

#define BUTTON_L            SDLK_TAB
#define BUTTON_R            SDLK_BACKSPACE

#define KEYPRESSED          SDL_KEYDOWN
#define KEYRELEASED         SDL_KEYUP

#endif



#ifdef LINUX_POCKETCHIP
//#define __FPU__         1
#define __RAM512MIB__   1
//#define __RTMIDI__      1

#define MAX_PATTERN_BY_PROJECT 128

#define SCREEN_WIDTH	480
#define SCREEN_HEIGHT	272
#define SCREEN_DEPTH	16
#define SCREEN_MULT     1



// Fine tuning 2048 for MIDI_DELAY_IN_SAMPLE and 512 for DEFAULTSAMPLES
#define MIDI_DELAY_IN_SAMPLE 2048
#define DEFAULTSAMPLES   2048 // Raspberry 1 can not handle 512 sample



#define KEY_REPEAT_INTERVAL_SMALLEST  4
#define KEY_REPEAT_INTERVAL_SMALL     8
#define KEY_REPEAT_INTERVAL_MIDDLE    32
#define KEY_REPEAT_INTERVAL_LONG      128
#define KEY_REPEAT_INTERVAL_LONGEST   256



#define BUTTON_B            SDLK_RSHIFT
#define BUTTON_A            SDLK_RCTRL
#define BUTTON_X            SDLK_SPACE
#define BUTTON_Y            SDLK_LSHIFT

#define BUTTON_UP           SDLK_UP
#define BUTTON_DOWN         SDLK_DOWN
#define BUTTON_LEFT         SDLK_LEFT
#define BUTTON_RIGHT        SDLK_RIGHT

#define BUTTON_SELECT       SDLK_ESCAPE
#define BUTTON_START        SDLK_RETURN

#define BUTTON_L            SDLK_TAB
#define BUTTON_R            SDLK_BACKSPACE

#define KEYPRESSED          SDL_KEYDOWN
#define KEYRELEASED         SDL_KEYUP

// END LINUX_POCKETCHIP 
#endif


#ifdef LINUX_RASPIBOY
//#define __FPU__         1
//#define __RAM512MIB__   1
//#define __RTMIDI__      1

#define MAX_PATTERN_BY_PROJECT 32
#define SCREEN_MULT     1
#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT	240
#define SCREEN_DEPTH	16

#define KEY_REPEAT_INTERVAL_SMALLEST  6
#define KEY_REPEAT_INTERVAL_SMALL     6
#define KEY_REPEAT_INTERVAL_MIDDLE    6
#define KEY_REPEAT_INTERVAL_LONG      160
#define KEY_REPEAT_INTERVAL_LONGEST   160

#define DEFAULTSAMPLES   2048

#define RASPIBOY_BUTTON_X        0 // X
#define RASPIBOY_BUTTON_A        1 // A
#define RASPIBOY_BUTTON_B        2 // B
#define RASPIBOY_BUTTON_Y        3 // Y
#define RASPIBOY_BUTTON_L        4
#define RASPIBOY_BUTTON_R        5
#define RASPIBOY_BUTTON_SELECT   8    
#define RASPIBOY_BUTTON_START    9
#define RASPIBOY_BUTTON_DOWN     12
#define RASPIBOY_BUTTON_LEFT     11 
#define RASPIBOY_BUTTON_UP       10
#define RASPIBOY_BUTTON_RIGHT    13


// BEGIN PSP SDL 1
#define BUTTON_B            RASPIBOY_BUTTON_B
#define BUTTON_A            RASPIBOY_BUTTON_A
#define BUTTON_X            RASPIBOY_BUTTON_X
#define BUTTON_Y            RASPIBOY_BUTTON_Y

#define BUTTON_UP           RASPIBOY_BUTTON_UP
#define BUTTON_DOWN         RASPIBOY_BUTTON_DOWN
#define BUTTON_LEFT         RASPIBOY_BUTTON_LEFT
#define BUTTON_RIGHT        RASPIBOY_BUTTON_RIGHT

#define BUTTON_SELECT       RASPIBOY_BUTTON_SELECT
#define BUTTON_START        RASPIBOY_BUTTON_START

#define BUTTON_L            RASPIBOY_BUTTON_L
#define BUTTON_R            RASPIBOY_BUTTON_R

#define KEYPRESSED          SDL_JOYBUTTONDOWN
#define KEYRELEASED         SDL_JOYBUTTONUP
// BEGIN PSP SDL
#endif



#ifdef PSP
//#define __FPU__         1
//#define __RAM512MIB__   1
//#define __RTMIDI__      1

#define MAX_PATTERN_BY_PROJECT 32
#define SCREEN_MULT     1
#define SCREEN_WIDTH	480
#define SCREEN_HEIGHT	272
#define SCREEN_DEPTH	16

#define KEY_REPEAT_INTERVAL_SMALLEST  6
#define KEY_REPEAT_INTERVAL_SMALL     6
#define KEY_REPEAT_INTERVAL_MIDDLE    6
#define KEY_REPEAT_INTERVAL_LONG      160
#define KEY_REPEAT_INTERVAL_LONGEST   160

#define DEFAULTSAMPLES   2048

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

#define BUTTON_SELECT       PSP_BUTTON_SELEC
#define BUTTON_START        PSP_BUTTON_START

#define BUTTON_L            PSP_BUTTON_L
#define BUTTON_R            PSP_BUTTON_R

#define KEYPRESSED          SDL_JOYBUTTONDOWN
#define KEYRELEASED         SDL_JOYBUTTONUP
// BEGIN PSP SDL
#endif

#ifdef PSVITA
//#define __FPU__         1
#define __RAM512MIB__   1
//#define __RTMIDI__      1

#define MAX_PATTERN_BY_PROJECT 128
#include <psp2/ctrl.h>
#include "psp2shell.h"
#include "p2s_cmd.h"
#include <psp2/kernel/clib.h>
#define SCREEN_MULT     2
#define SCREEN_WIDTH	960
#define SCREEN_HEIGHT	544
#define SCREEN_DEPTH	16

#define KEY_REPEAT_INTERVAL_SMALLEST  1
#define KEY_REPEAT_INTERVAL_SMALL     1
#define KEY_REPEAT_INTERVAL_MIDDLE    1
#define KEY_REPEAT_INTERVAL_LONG      20
#define KEY_REPEAT_INTERVAL_LONGEST   20

#define DEFAULTSAMPLES   128


// BEGIN PSP SDL 
#define BUTTON_B            SCE_CTRL_CROSS
#define BUTTON_A            SCE_CTRL_CIRCLE
#define BUTTON_X            SCE_CTRL_TRIANGLE
#define BUTTON_Y            SCE_CTRL_SQUARE

#define BUTTON_UP           SCE_CTRL_UP
#define BUTTON_DOWN         SCE_CTRL_DOWN
#define BUTTON_LEFT         SCE_CTRL_LEFT
#define BUTTON_RIGHT        SCE_CTRL_RIGHT

#define BUTTON_SELECT       SCE_CTRL_SELECT
#define BUTTON_START        SCE_CTRL_START

#define BUTTON_L            SCE_CTRL_LTRIGGER
#define BUTTON_R            SCE_CTRL_RTRIGGER

#define KEYPRESSED          SDL_JOYBUTTONDOWN
#define KEYRELEASED         SDL_JOYBUTTONUP
// BEGIN PSP SDL
#endif





#ifdef GP2X
//#define __FPU__         1
//#define __RAM512MIB__   1
//#define __RTMIDI__      1

#define MAX_PATTERN_BY_PROJECT 32
#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT	240
#define SCREEN_DEPTH	16
#define SCREEN_MULT     1

#define KEY_REPEAT_INTERVAL_SMALLEST  1
#define KEY_REPEAT_INTERVAL_SMALL     4
#define KEY_REPEAT_INTERVAL_MIDDLE    32
#define KEY_REPEAT_INTERVAL_LONG      64
#define KEY_REPEAT_INTERVAL_LONGEST   128

#define DEFAULTSAMPLES            256

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

#define BUTTON_L            GP2X_BUTTON_L
#define BUTTON_R            GP2X_BUTTON_R

// BEGIN GP2X SDL

#define KEYPRESSED          SDL_JOYBUTTONDOWN
#define KEYRELEASED         SDL_JOYBUTTONUP

#endif

#define PS4_BUTTON_UP              0
#define PS4_BUTTON_DOWN            4
#define PS4_BUTTON_LEFT            2
#define PS4_BUTTON_RIGHT           6
#define PS4_BUTTON_UPLEFT          1
#define PS4_BUTTON_UPRIGHT         7
#define PS4_BUTTON_DOWNLEFT        3
#define PS4_BUTTON_DOWNRIGHT       5
#define PS4_BUTTON_CLICK           18
#define PS4_BUTTON_A               12
#define PS4_BUTTON_B               13
#define PS4_BUTTON_X               14
#define PS4_BUTTON_Y               15
#define PS4_BUTTON_L               10
#define PS4_BUTTON_R               11
#define PS4_BUTTON_START           8
#define PS4_BUTTON_SELECT          9
#define PS4_BUTTON_VOLUP           16
#define PS4_BUTTON_VOLDOWN         17

/*
// PC DESKTOP
#define PS3_BUTTON_UP              4
#define PS3_BUTTON_DOWN            6
#define PS3_BUTTON_LEFT            7
#define PS3_BUTTON_RIGHT           5
#define PS3_BUTTON_CLICKL          1
#define PS3_BUTTON_CLICKR          2
#define PS3_BUTTON_X               14
#define PS3_BUTTON_O               13
#define PS3_BUTTON_SQUARE          15
#define PS3_BUTTON_TRIANGLE        12
#define PS3_BUTTON_L1              10
#define PS3_BUTTON_R1              11
#define PS3_BUTTON_L2              8
#define PS3_BUTTON_R2              9
#define PS3_BUTTON_START           3
#define PS3_BUTTON_SELECT          0
#define PS3_BUTTON_VOLUP           16
#define PS3_BUTTON_VOLDOWN         17
*/

#define PS3_BUTTON_UP              13
#define PS3_BUTTON_DOWN            14
#define PS3_BUTTON_LEFT            15
#define PS3_BUTTON_RIGHT           16
#define PS3_BUTTON_CLICKL          11
#define PS3_BUTTON_CLICKR          12
#define PS3_BUTTON_X               0
#define PS3_BUTTON_O               1
#define PS3_BUTTON_SQUARE          3
#define PS3_BUTTON_TRIANGLE        2
#define PS3_BUTTON_L1              4
#define PS3_BUTTON_R1              5
#define PS3_BUTTON_L2              6
#define PS3_BUTTON_R2              7
#define PS3_BUTTON_START           9
#define PS3_BUTTON_SELECT          8
#define PS3_BUTTON_VOLUP           16
#define PS3_BUTTON_VOLDOWN         17



enum 
  {
    FMTYPE_2_OP_FM,
    FMTYPE_2_OP_AM
  };


#define SYNTH_PICOSYNTH    0
#define SYNTH_OPL2         1
#define SYNTH_PICODRUM     2
#define SYNTH_PBSYNTH      3
#define SYNTH_CURSYNTH     4
#define SYNTH_OPEN303      5
#define SYNTH_TWYTCHSYNTH  6
#define SYNTH_MIDIOUT      7
#define SYNTH_MDADRUM      8
#define SYNTH_SIDSYNTH     9
#define SYNTH_LGPTSAMPLER  10




enum 
  {
    PICO_WAVETABLE_SINE,
    PICO_WAVETABLE_SAW,
    PICO_WAVETABLE_PULSE,
    PICO_WAVETABLE_TRGL,

    PICO_WAVETABLE_SMSINE,
    PICO_WAVETABLE_SMSAW,
    PICO_WAVETABLE_SMPULSE,
    PICO_WAVETABLE_SMTRGL,
    
    PICO_WAVETABLE_NOISE,
    PICO_WAVETABLE_LFSRNOISE,
    
    //PICO_WAVETABLE_ONE,
    PICO_WAVETABLE_SIZE
  };

enum 
  {
    PICO_DBOPL_SINE,
    PICO_DBOPL_HSIN,
    PICO_DBOPL_ABSIN,
    PICO_DBOPL_QSIN,
    PICO_DBOPL_EPSIN,
    PICO_DBOPL_ASIN,
    PICO_DBOPL_SQR,
    PICO_DBOPL_DSQR,
    PICO_DBOPL_SIZE
  };


enum 
  {
    PICO_PBSYNTH_SQUARE,
    PICO_PBSYNTH_TRIANGLE,
    PICO_PBSYNTH_SAW,
    PICO_PBSYNTH_SIZE
  };

enum 
  {
    PICO_SIDSYNTH_SQUARE,
    PICO_SIDSYNTH_TRIANGE,
    PICO_SIDSYNTH_SAW,
    PICO_SIDSYNTH_NOISE,
    PICO_SIDSYNTH_SIZE
  };


enum 
  {
    PICO_CURSYNTH_SIN,
    PICO_CURSYNTH_TRIANGLE,
    PICO_CURSYNTH_SQUARE,
    PICO_CURSYNTH_DOWNSAW,
    PICO_CURSYNTH_UPSAW,
    PICO_CURSYNTH_THREESTEP,
    PICO_CURSYNTH_FOURSTEP,
    PICO_CURSYNTH_HEIGHTSTEP,
    PICO_CURSYNTH_THREEPYRAMID,
    PICO_CURSYNTH_FIVEPYRAMID,
    PICO_CURSYNTH_NINEPYRAMID,
    PICO_CURSYNTH_WHITENOISE,
    PICO_CURSYNTH_SIZE
  };

enum 
  {
    PICO_TWYTCHSYNTH_SIN,
    PICO_TWYTCHSYNTH_TRIANGLE,
    PICO_TWYTCHSYNTH_SQUARE,
    PICO_TWYTCHSYNTH_DOWNSAW,
    PICO_TWYTCHSYNTH_UPSAW,
    PICO_TWYTCHSYNTH_THREESTEP,
    PICO_TWYTCHSYNTH_FOURSTEP,
    PICO_TWYTCHSYNTH_HEIGHTSTEP,
    PICO_TWYTCHSYNTH_THREEPYRAMID,
    PICO_TWYTCHSYNTH_FIVEPYRAMID,
    PICO_TWYTCHSYNTH_NINEPYRAMID,
    PICO_TWYTCHSYNTH_SIZE
  };

enum
  {
    PBSYNTH_FILTER_ALGO_PBLP,
    PBSYNTH_FILTER_ALGO_SIZE
  };


enum
  {
    PBSYNTH_FILTER_TYPE_LP12,
    PBSYNTH_FILTER_TYPE_LP24,
    PBSYNTH_FILTER_TYPE_SIZE
  };

enum
  {
    FILTER_TYPE_LP,
    FILTER_TYPE_BP,
    FILTER_TYPE_HP,
    FILTER_TYPE_SIZE
  };

enum
  {
    CURSYNTH_FILTER_TYPE_LP,
    CURSYNTH_FILTER_TYPE_HP,
    CURSYNTH_FILTER_TYPE_SIZE
  };


enum
  {
    TWYTCHSYNTH_FILTER_TYPE_LP,
    TWYTCHSYNTH_FILTER_TYPE_HP,
    TWYTCHSYNTH_FILTER_TYPE_BP,
    TWYTCHSYNTH_FILTER_TYPE_LS,
    TWYTCHSYNTH_FILTER_TYPE_HS,
    TWYTCHSYNTH_FILTER_TYPE_BS,
    TWYTCHSYNTH_FILTER_TYPE_AP,
    TWYTCHSYNTH_FILTER_TYPE_NT,
    TWYTCHSYNTH_FILTER_TYPE_SIZE
  };


enum
  {
    OPEN303_FILTER_TYPE_FLAT,
    OPEN303_FILTER_TYPE_LP_6,
    OPEN303_FILTER_TYPE_LP_12,
    OPEN303_FILTER_TYPE_LP_18,
    OPEN303_FILTER_TYPE_LP_24,

    OPEN303_FILTER_TYPE_HP_6,
    OPEN303_FILTER_TYPE_HP_12,
    OPEN303_FILTER_TYPE_HP_18,
    OPEN303_FILTER_TYPE_HP_24,

    OPEN303_FILTER_TYPE_BP_12_12,
    OPEN303_FILTER_TYPE_BP_6_18, 
    OPEN303_FILTER_TYPE_BP_18_6, 
    OPEN303_FILTER_TYPE_BP_6_12, 
    OPEN303_FILTER_TYPE_BP_12_6, 
    OPEN303_FILTER_TYPE_BP_6_6,  

    OPEN303_FILTER_TYPE_TB_303,  
    OPEN303_FILTER_TYPE_SIZE
  };



enum
  {
    FILTER_ALGO_NOFILTER,
    FILTER_ALGO_BIQUAD,
    FILTER_ALGO_AMSYNTH,
    FILTER_ALGO_SIZE
  };

enum
  {
    LFO_TYPE_PITCHLFO,
    LFO_TYPE_PITCHBEND,
    LFO_TYPE_SIZE
  };


enum
  {
    FM_TYPE_AM,
    FM_TYPE_FM,
    FM_TYPE_SIZE
  };

#define FONTSIZE         8

// Screen size is
//  LINE00    to LINE63
//  COLLUMN00 to COLLUMN29

#define COLLUMN00           0*5*SCREEN_MULT
#define COLLUMN01           1*5*SCREEN_MULT
#define COLLUMN02           2*5*SCREEN_MULT
#define COLLUMN03           3*5*SCREEN_MULT
#define COLLUMN04           4*5*SCREEN_MULT
#define COLLUMN05           5*5*SCREEN_MULT
#define COLLUMN06           6*5*SCREEN_MULT
#define COLLUMN07           7*5*SCREEN_MULT
#define COLLUMN08           8*5*SCREEN_MULT
#define COLLUMN09           9*5*SCREEN_MULT

#define COLLUMN10          10*5*SCREEN_MULT
#define COLLUMN11          11*5*SCREEN_MULT
#define COLLUMN12          12*5*SCREEN_MULT
#define COLLUMN13          13*5*SCREEN_MULT
#define COLLUMN14          14*5*SCREEN_MULT
#define COLLUMN15          15*5*SCREEN_MULT
#define COLLUMN16          16*5*SCREEN_MULT
#define COLLUMN17          17*5*SCREEN_MULT
#define COLLUMN18          18*5*SCREEN_MULT
#define COLLUMN19          19*5*SCREEN_MULT

#define COLLUMN20          20*5*SCREEN_MULT
#define COLLUMN21          21*5*SCREEN_MULT
#define COLLUMN22          22*5*SCREEN_MULT
#define COLLUMN23          23*5*SCREEN_MULT
#define COLLUMN24          24*5*SCREEN_MULT
#define COLLUMN25          25*5*SCREEN_MULT
#define COLLUMN26          26*5*SCREEN_MULT
#define COLLUMN27          27*5*SCREEN_MULT
#define COLLUMN28          28*5*SCREEN_MULT
#define COLLUMN29          29*5*SCREEN_MULT

#define COLLUMN30          30*5*SCREEN_MULT
#define COLLUMN31          31*5*SCREEN_MULT
#define COLLUMN32          32*5*SCREEN_MULT
#define COLLUMN33          33*5*SCREEN_MULT
#define COLLUMN34          34*5*SCREEN_MULT
#define COLLUMN35          35*5*SCREEN_MULT
#define COLLUMN36          36*5*SCREEN_MULT
#define COLLUMN37          37*5*SCREEN_MULT
#define COLLUMN38          38*5*SCREEN_MULT
#define COLLUMN39          39*5*SCREEN_MULT

#define COLLUMN40          40*5*SCREEN_MULT
#define COLLUMN41          41*5*SCREEN_MULT
#define COLLUMN42          42*5*SCREEN_MULT
#define COLLUMN43          43*5*SCREEN_MULT
#define COLLUMN44          44*5*SCREEN_MULT
#define COLLUMN45          45*5*SCREEN_MULT
#define COLLUMN46          46*5*SCREEN_MULT
#define COLLUMN47          47*5*SCREEN_MULT
#define COLLUMN48          48*5*SCREEN_MULT
#define COLLUMN49          49*5*SCREEN_MULT

#define COLLUMN50          50*5*SCREEN_MULT
#define COLLUMN51          51*5*SCREEN_MULT
#define COLLUMN52          52*5*SCREEN_MULT
#define COLLUMN53          53*5*SCREEN_MULT
#define COLLUMN54          54*5*SCREEN_MULT
#define COLLUMN55          55*5*SCREEN_MULT
#define COLLUMN56          56*5*SCREEN_MULT
#define COLLUMN57          57*5*SCREEN_MULT
#define COLLUMN58          58*5*SCREEN_MULT
#define COLLUMN59          59*5*SCREEN_MULT

#define COLLUMN60          60*5*SCREEN_MULT
#define COLLUMN61          61*5*SCREEN_MULT
#define COLLUMN62          62*5*SCREEN_MULT
#define COLLUMN63          63*5*SCREEN_MULT
#define COLLUMN64          64*5*SCREEN_MULT
#define COLLUMN65          65*5*SCREEN_MULT
#define COLLUMN66          66*5*SCREEN_MULT
#define COLLUMN67          67*5*SCREEN_MULT
#define COLLUMN68          68*5*SCREEN_MULT
#define COLLUMN69          69*5*SCREEN_MULT





#define LINE00        0*8*SCREEN_MULT
#define LINE01        1*8*SCREEN_MULT
#define LINE02        2*8*SCREEN_MULT
#define LINE03        3*8*SCREEN_MULT
#define LINE04        4*8*SCREEN_MULT
#define LINE05        5*8*SCREEN_MULT
#define LINE06        6*8*SCREEN_MULT
#define LINE07        7*8*SCREEN_MULT
#define LINE08        8*8*SCREEN_MULT
#define LINE09        9*8*SCREEN_MULT

#define LINE10       10*8*SCREEN_MULT
#define LINE11       11*8*SCREEN_MULT
#define LINE12       12*8*SCREEN_MULT
#define LINE13       13*8*SCREEN_MULT
#define LINE14       14*8*SCREEN_MULT
#define LINE15       15*8*SCREEN_MULT
#define LINE16       16*8*SCREEN_MULT
#define LINE17       17*8*SCREEN_MULT
#define LINE18       18*8*SCREEN_MULT
#define LINE19       19*8*SCREEN_MULT

#define LINE20       20*8*SCREEN_MULT
#define LINE21       21*8*SCREEN_MULT
#define LINE22       22*8*SCREEN_MULT
#define LINE23       23*8*SCREEN_MULT
#define LINE24       24*8*SCREEN_MULT
#define LINE25       25*8*SCREEN_MULT
#define LINE26       26*8*SCREEN_MULT
#define LINE27       27*8*SCREEN_MULT
#define LINE28       28*8*SCREEN_MULT
#define LINE29       29*8*SCREEN_MULT

#define LINE30       30*8*SCREEN_MULT
#define LINE31       31*8*SCREEN_MULT
#define LINE32       32*8*SCREEN_MULT
#define LINE33       33*8*SCREEN_MULT
#define LINE34       34*8*SCREEN_MULT
#define LINE35       35*8*SCREEN_MULT
#define LINE36       36*8*SCREEN_MULT
#define LINE37       37*8*SCREEN_MULT
#define LINE38       38*8*SCREEN_MULT
#define LINE39       39*8*SCREEN_MULT

#define LINE40       40*8*SCREEN_MULT
#define LINE41       41*8*SCREEN_MULT
#define LINE42       42*8*SCREEN_MULT
#define LINE43       43*8*SCREEN_MULT
#define LINE44       44*8*SCREEN_MULT
#define LINE45       45*8*SCREEN_MULT
#define LINE46       46*8*SCREEN_MULT
#define LINE47       47*8*SCREEN_MULT
#define LINE48       48*8*SCREEN_MULT
#define LINE49       49*8*SCREEN_MULT

#define LINE50       50*8*SCREEN_MULT
#define LINE51       51*8*SCREEN_MULT
#define LINE52       52*8*SCREEN_MULT
#define LINE53       53*8*SCREEN_MULT
#define LINE54       54*8*SCREEN_MULT
#define LINE55       55*8*SCREEN_MULT
#define LINE56       56*8*SCREEN_MULT
#define LINE57       57*8*SCREEN_MULT
#define LINE58       58*8*SCREEN_MULT
#define LINE59       59*8*SCREEN_MULT

#define LINE60       60*8*SCREEN_MULT
#define LINE61       61*8*SCREEN_MULT
#define LINE62       62*8*SCREEN_MULT
#define LINE63       63*8*SCREEN_MULT
#define LINE64       64*8*SCREEN_MULT


// PC_DESKTOP
// picosynth,picodrum,opl2,pbsynth
// cursynth,open303,twytch,mdadrum,sidsynth
// midiout
// lgpt
#if    defined(__VECTORFPU__) && defined(__RTMIDI__) && defined(__RAM512MIB__) && !defined(__PBSYNTHONLY__)
#define SYNTH_SIZE 11
#endif

// PC_DESKTOP without MIDIOUT
// picosynth,picodrum,opl2,pbsynth
// cursynth,open303,twytch,mdadrum,sidsynth
// lgpt
#if    defined(__VECTORFPU__) && !defined(__RTMIDI__) && defined(__RAM512MIB__)&& !defined(__PBSYNTHONLY__)
#define SYNTH_SIZE 10
#endif

// PC_DESKTOP without : ( SYTNH_MIDIOUT, SYNTH_LGPTSAMPLER)
// picosynth,picodrum,opl2,pbsynth
// cursynth,open303,twytch,mdadrum,sidsynth
#if    defined(__VECTORFPU__) && !defined(__RTMIDI__) && !defined(__RAM512MIB__) && !defined(__PBSYNTHONLY__)
#define SYNTH_SIZE 9
#endif

//PSVITA, LINUXPOCKETCHIP
// picosynth,picodrum,opl2,pbsynth,
// lgptsampler
#if    !defined(__VECTORFPU__) && !defined(__RTMIDI__) && defined(__RAM512MIB__) && !defined(__PBSYNTHONLY__)
#define SYNTH_SIZE 5
#endif

// OPENDINGUX PSP GP2X
// picosynth,picodrum,opl2,pbsynth
#if   !defined(__VECTORFPU__) && !defined(__RTMIDI__) && !defined(__RAM512MIB__) && !defined(__PBSYNTHONLY__)
#define SYNTH_SIZE 4
#endif

// LINUX
// pbsynth
#if   !defined(__VECTORFPU__) && !defined(__RTMIDI__) && !defined(__RAM512MIB__) && defined(__PBSYNTHONLY__)
#define SYNTH_SIZE 1
#endif



// Define this variable to be able to open the picoloop.log file,
// This variable are defined in the same file as main()
extern int opendebugprintf;
extern FILE * fdebugprintf;
#ifndef DEBUGPRINTF
#define DEBUGPRINTF 0
#else
#define DEBUGPRINTF 1
#endif
#ifndef DEBUGPRINTF_STDOUT
#define DEBUGPRINTF_STDOUT 0
#else
#define DEBUGPRINTF_STDOUT 1
#endif

/*
#if defined(PSVITA) && defined(DEBUGPRINTF)
#define DPRINTF(FMT, ARGS...) do { \
    char strdebug[512];							\
    sprintf(strdebug,"%.40s:%.8d [" FMT "]\n", __FUNCTION__, __LINE__, ## ARGS); \
    psp2shell_print(strdebug);						\
  } while (0)
#endif
*/
#if defined(PSVITA) && DEBUGPRINTF==1
#define DPRINTF(FMT, ARGS...) do { \
    char strdebug[512];							\
    sprintf(strdebug,"%.40s:%.8d [" FMT "]\n", __FUNCTION__, __LINE__, ## ARGS); \
    sceClibPrintf(strdebug);						\
  } while (0)
#endif

#if defined(PSVITA)
#define CLOSE_DPRINTF() do { } while(0)
#endif

#if !defined(PSVITA)
// BUG HERE DEBUGPRINTF always defined
//#define CLOSE_DPRINTF() do { fclose(fdebugprintf); } while(0)
#define CLOSE_DPRINTF() do { } while(0)
#endif


#if !defined(PSVITA) && DEBUGPRINTF==1 && DEBUGPRINTF_STDOUT==0
#define DPRINTF(FMT, ARGS...) do {					\
    if (DEBUGPRINTF)							\
      if (opendebugprintf==0)						\
	{ fdebugprintf=fopen("picoloop.log","w"); opendebugprintf=1; }	\
    if (DEBUGPRINTF)							\
      {									\
	fprintf(fdebugprintf, "%.40s:%.8d [" FMT "]\n", __FUNCTION__, __LINE__, ## ARGS); \
      }									\
  } while (0)
#endif

#if !defined(PSVITA) && DEBUGPRINTF==1 && DEBUGPRINTF_STDOUT==1
#define DPRINTF(FMT, ARGS...) do {					\
    if (DEBUGPRINTF)							\
      {									\
	printf("%.40s:%.8d [" FMT "]\n", __FUNCTION__, __LINE__, ## ARGS); \
      }									\
  } while (0)
#endif

//	fprintf(stderr, "%.40s:%.8d [" FMT "]\n", __FUNCTION__, __LINE__, ## ARGS); \

#if DEBUGPRINTF==0
#define DPRINTF(FMT, ARGS...) do { } while(0)
#endif



#endif



