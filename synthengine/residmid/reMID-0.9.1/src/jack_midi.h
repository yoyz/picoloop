
#include <jack/jack.h>

int jack_init_seq(void);
void jack_midi_connect(char *port);
void jack_read_midi(jack_nframes_t nframes);

