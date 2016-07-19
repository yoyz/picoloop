
#include <pthread.h>

extern pthread_mutex_t prefs_mutex;

extern char *instr_file;

extern int polyphony;
extern int new_polyphony;

extern int pal;

extern int use_gui;
extern int pt_debug;

extern char *midi_connect_args[];
extern char *jack_connect_args[];

// if 1 use the SID volume register
// if 0 set SID volume to 0xf and scale the output later
// SID volume is more authentic
// non-SID volume avoids the SID volume click bug
//  and allows all 128 velocity levels
extern int use_sid_volume;

extern int num_instrs;

void prefs_set_polyphony(int value);

void prefs_init();

void prefs_read_instruments(char *path);

