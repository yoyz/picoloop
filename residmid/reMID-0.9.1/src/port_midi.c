
#include <stdlib.h>
#include <stdio.h>
#include <portmidi.h>

#include "midi.h"

#define DRIVER_INFO NULL
#define INPUT_BUFFER_SIZE 1024

int midiport=9;
PmError pm_status;
PmStream *pm_stream;
PmEvent pm_buf[INPUT_BUFFER_SIZE];

//snd_seq_t *seq_handle=NULL;
//int in_port;
//int npfd;
//struct pollfd *pfd;

//char *clientname="reMID";
//char *portname="MIDI_In";

port_read_midi() {
	/*snd_seq_event_t *ev;
	int channel, param, value;

	if(!poll(pfd, npfd, 0)) return;

	do {
		snd_seq_event_input(seq_handle, &ev);
		switch(ev->type) {
			case SND_SEQ_EVENT_CONTROLLER:
				channel=ev->data.control.channel;
				if(!midi_channels[channel].in_use) break;
				param=ev->data.control.param;
				value=ev->data.control.value;
				if(param==64) {
					if(value>64) midi_channels[channel].sustain=1;
					else midi_channels[channel].sustain=0;
				} else if(param==1) {
					// modulation controlling vibrato: value=0-127
					midi_channels[channel].vibrato=value;
					//printf("%d\n", value);
					midi_channels[channel].vibrato_changed=1;
				}
				break;
			// case SND_SEQ_EVENT_KEYPRESS:
			case SND_SEQ_EVENT_CHANPRESS:
				channel=ev->data.control.channel;
				if(!midi_channels[channel].in_use) break;
				value=ev->data.control.value;
				midi_channels[channel].chanpress=value;
				midi_channels[channel].chanpress_changed=1;
				break;
			case SND_SEQ_EVENT_NOTEON:
				channel=ev->data.note.channel;
				if(!midi_channels[channel].in_use) break;
				note_on(channel, ev->data.note.note,
					ev->data.note.velocity);
				break;
			case SND_SEQ_EVENT_NOTEOFF:
				channel=ev->data.note.channel;
				if(!midi_channels[channel].in_use) break;
				note_off(channel, ev->data.note.note);
				break;
			case SND_SEQ_EVENT_PITCHBEND:
				// value=-8192 to +8191
				channel=ev->data.control.channel;
				if(!midi_channels[channel].in_use) break;
				value=ev->data.control.value;
				midi_channels[channel].pitchbend=value;
				break;
			case SND_SEQ_EVENT_PGMCHANGE:
				channel=ev->data.control.channel;
				if(midi_channels[channel].program==-1) break;
				value=ev->data.control.value;
				//printf("prg change %d\n", value);
				midi_channels[channel].program=value;
				break;
		}
	} while (snd_seq_event_input_pending(seq_handle, 0)>0);
	*/
}

port_midi_connect(int client, int port) {
	//snd_seq_connect_from(seq_handle, 0, client, port);
}

int port_init_seq() {
	//if(seq_handle!=NULL) return 1;

	pm_status=Pm_Initialize();
	if(pm_status!=pmNoError) {
		fprintf(stderr, "Error initialising PortMIDI\n");
		return 0;
	}

  int i;
  const PmDeviceInfo *pm_dev_info;
  int num_devices=Pm_CountDevices();
  printf("Available MIDI devices:\n");
  for(i=0; i<num_devices; i++) {
    pm_dev_info=Pm_GetDeviceInfo(i);
    if(pm_dev_info->input) printf("%d: %s\n", i, pm_dev_info->name);
  }
  printf("\n");

	pm_status=Pm_OpenInput(&pm_stream, midiport, DRIVER_INFO, INPUT_BUFFER_SIZE, NULL, NULL);
	if(pm_status!=pmNoError) {
		fprintf(stderr, "Error opening MIDI input device\n");
	}


/*
	if(snd_seq_open(&seq_handle, "default", SND_SEQ_OPEN_INPUT, 0)<0) {
		fprintf(stderr, "Error opening ALSA sequencer.\n");
		return 0;
	}

	snd_seq_set_client_name(seq_handle, clientname);

	in_port=snd_seq_create_simple_port(seq_handle, portname,
		SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,
		SND_SEQ_PORT_TYPE_APPLICATION);

	if(in_port<0) {
		fprintf(stderr, "Error creating sequencer port.\n");
		return 0;
	}

	npfd=snd_seq_poll_descriptors_count(seq_handle, POLLIN);
	pfd=(struct pollfd *)malloc(npfd*sizeof(struct pollfd));
	snd_seq_poll_descriptors(seq_handle, pfd, npfd, POLLIN);
*/

	return 1;
}

