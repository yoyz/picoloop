
#include <jack/jack.h>

struct midi_key_state {
	int last_used;
	int needs_clearing;

	int channel;
	int note_state_changed;
	int note_on;
	int note;
	int velocity;

	//int aftertouch_state_changed;
	//int aftertouch;
};
extern struct midi_key_state **midi_keys;

struct midi_channel_state {
	int in_use;
	int program;
	int sustain;
	int pitchbend;
	int vibrato;
	int vibrato_changed;
	int chanpress;
	int chanpress_changed;
	int last_velocity;
};
extern struct midi_channel_state midi_channels[];

extern int midi_programs[];

extern double note_frqs[];

int init_midi(int polyphony);
void read_midi(jack_nframes_t nframes);
void note_on(int channel, int note, int velocity);
void note_off(int channel, int note);
void silence_all(void);

