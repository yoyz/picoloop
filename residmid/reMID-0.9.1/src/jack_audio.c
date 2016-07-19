
#include <jack/jack.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "jack_audio.h"
#include "midi.h"
#include "sid_chips.h"
#include "sid_instr.h"
#include "prefs.h"


jack_client_t *client=NULL;
jack_port_t *output_port_l;
jack_port_t *output_port_r;
char *port_names[2];

char *jclientname="reMID";

struct SID **sid_bank=NULL;

int process(jack_nframes_t nframes, void *arg) {
	int i,j;

	short x;
	static short high=0;
	static short low=0;

	pthread_mutex_lock(&prefs_mutex);

	read_midi(nframes);

	sample_t *outl=(sample_t *)jack_port_get_buffer(output_port_l, nframes);
	for(i=0; i<nframes; i++) outl[i]=0.0;
	sample_t *outr=(sample_t *)jack_port_get_buffer(output_port_r, nframes);
	for(i=0; i<nframes; i++) outr[i]=0.0;

	short *sid_buf=sid_process(sid_bank, (int)nframes);
	for(i=0; i<polyphony; i++) {
		int channel=midi_keys[i]->channel;
		if(channel==-1) continue;
		int program=midi_channels[channel].program;
		if(program==-1) continue;
		//int inst_num=midi_programs[channel];
		int inst_num=midi_programs[program];
		if(inst_num==-1) continue;
		sid_instrument_t *instr=sid_instr[inst_num];
		sample_t volume=((sample_t)midi_keys[i]->velocity)/128.0;
		for(j=0; j<nframes; j++) {
			x=sid_buf[(i*nframes)+j];
			sample_t a=((sample_t)x)/32768.0;

			if(!use_sid_volume) a*=volume;

			sample_t al=a*instr->vol_left;
			sample_t ar=a*instr->vol_right;

			if(instr->panning) { 
				// 0-127 -> 0.0-2.0
				sample_t note=((sample_t)midi_keys[i]->note)/64.0;
				al*=2.0-note;
				ar*=note;
			}

			outl[j]+=al;
			outr[j]+=ar;

			if(x>high) high=x;
			if(x<low) low=x;
		}
	}
	//printf("low: %d, high: %d\n", low, high);
	pthread_mutex_unlock(&prefs_mutex);
	return 0;      
}

int srate(jack_nframes_t nframes, void *arg) {
	printf("The sample rate is now %" PRIu32 "/sec\n", nframes);
	sid_set_srate(sid_bank, pal, nframes);
	return 0;
}

void jack_shutdown(void *arg) {
	printf("JACK client shutting down\n");
	if(client) jack_client_close(client);
	exit(0);
}

void jack_connect_ports() {
	int i;
	char src_port[255];
	for(i=0; jack_connect_args[i]; i++) {
		char *port=jack_connect_args[i];
		snprintf(src_port, 255, "%s:%s", "reMID", port_names[i%2]);
		printf("Connecting JACK audio port to %s\n", port);
		if(jack_connect(client, src_port, port)) {
			printf("Error connecting to JACK port %s\n", port);
		} else {
			printf("Connected to JACK port %s\n", port);
		}
	}
}

int init_jack_audio() {

	pthread_mutex_lock(&prefs_mutex);

	if(client) jack_client_close(client);
	if(sid_bank) sid_close(sid_bank);

	client=jack_client_new(jclientname);
	if (!client) {
		fprintf(stderr, "Couldn't open connection to jack server\n");
		pthread_mutex_unlock(&prefs_mutex);
		return 0;
	}

	polyphony=new_polyphony;

	init_midi(polyphony);
	prefs_read_instruments(instr_file);
	sid_bank=sid_init();
	
	jack_set_process_callback(client, process, 0);
	jack_set_sample_rate_callback(client, srate, 0);
	jack_on_shutdown(client, jack_shutdown, 0);

	port_names[0]="audio_out_left";
	output_port_l=jack_port_register(client, port_names[0], JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
	port_names[1]="audio_out_right";
	output_port_r=jack_port_register(client, port_names[1], JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

	if (jack_activate(client)) {
		fprintf(stderr, "Cannot activate client");
		pthread_mutex_unlock(&prefs_mutex);
		return 0;
	}

	jack_connect_ports();

	pthread_mutex_unlock(&prefs_mutex);

	printf("\nREADY.\n");


	//while(1) sleep(1);
	//jack_client_close(client);
	//exit (0);
}

