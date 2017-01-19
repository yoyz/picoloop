
#ifndef JACK_AUDIO_H
#define JACK_AUDIO_H

#include <jack/jack.h>

typedef jack_default_audio_sample_t sample_t;

extern jack_client_t *client;
extern char *jclientname;

int init_jack_audio();

#endif
