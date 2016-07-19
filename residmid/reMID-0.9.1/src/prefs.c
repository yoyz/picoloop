
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <glib.h>

#include "sid_instr.h"
#include "midi.h"

pthread_mutex_t prefs_mutex=PTHREAD_MUTEX_INITIALIZER;

char *instr_file="instruments.conf";

int polyphony;
int new_polyphony=16;

int pal=1;

int pt_debug=0;

int use_gui=1;
int use_sid_volume=0;

char *midi_connect_args[255];
char *jack_connect_args[255];

int num_instrs;
sid_instrument_t **sid_instr=NULL;

char *opnames[NUM_OPCODES] = {
	[NOP]="nop",
	[STOP]="stop",
	[WAIT]="wait",
	[GOTO]="goto",
	[PRINT]="print",
	[V1FREQ]="v1_freq", [V1FREQPCT]="v1_freq_pct", [V1FREQHS]="v1_freq_hs",
	[V1PULSE]="v1_pulse",
	[V1CONTROL]="v1_control", [V1AD]="v1_ad", [V1SR]="v1_sr",
	[V2FREQ]="v2_freq", [V2FREQPCT]="v2_freq_pct", [V2FREQHS]="v2_freq_hs",
	[V2PULSE]="v2_pulse",
	[V2CONTROL]="v2_control", [V2AD]="v2_ad", [V2SR]="v2_sr",
	[V3FREQ]="v3_freq", [V3FREQPCT]="v3_freq_pct", [V3FREQHS]="v3_freq_hs",
	[V3PULSE]="v3_pulse",
	[V3CONTROL]="v3_control", [V3AD]="v3_ad", [V3SR]="v3_sr",
	[FILTER_CUTOFF]="filter_cutoff",
	[FR_VIC]="fr_vic",
	[FILTER_MODE]="filter_mode",
	[V1PULSEMOD]="v1_pulsemod", [V2PULSEMOD]="v2_pulsemod", [V3PULSEMOD]="v3_pulsemod",
	[V1GATE]="v1_gate", [V2GATE]="v2_gate", [V3GATE]="v3_gate",
	[INVALID]=NULL
};

// requires init_jack_audio() to take effect
void prefs_set_polyphony(int value) {
	if(value<1) value=1;
	else if(value>128) value=128;
	new_polyphony=value;
}

void prefs_add_midi_connect(char *port) {
	int i;
	for(i=0; midi_connect_args[i]; i++);
	midi_connect_args[i++]=port;
	midi_connect_args[i]=NULL;
}

void prefs_add_jack_connect(char *port) {
	int i;
	for(i=0; jack_connect_args[i]; i++);
	jack_connect_args[i++]=port;
	jack_connect_args[i]=NULL;
}

void prefs_init() {
	midi_connect_args[0]=NULL;
	jack_connect_args[0]=NULL;
}

int readn(char *str) {
	//int i;
	//while(isspace(*str)) str++;
	//if(!strncmp(str, "0x", 2)||!strncmp(str, "$", 2)) {
	//	i=strtol(str, (char **)NULL, 16);
	//} else {
	//	i=strtol(str, (char **)NULL, 10);
	//}
	return strtol(str, (char **)NULL, 0);
}

void prefs_read_instruments(char *path) {
	GKeyFile *inst_config;
	int i,j,k;
	char *value;
	GError *err;

	inst_config=g_key_file_new();
	i=g_key_file_load_from_file(inst_config, path,
		G_KEY_FILE_KEEP_COMMENTS, NULL);
	if(!i) {
		printf("Error reading instrument config from %s\n", path);
		return;
	}

	// config groups
	gchar **groups=g_key_file_get_groups(inst_config, NULL);
	
	// instruments
	num_instrs=0;
	for(i=0; groups[i]; i++) {
		if(!strcmp(groups[i], "channels")) continue;
		if(!strcmp(groups[i], "programs")) continue;
		num_instrs++;
	}
	if(sid_instr!=NULL) {
		//printf("freeing memory\n");
		for(i=0; sid_instr[i]; i++) {
			sid_command_t *cmd=sid_instr[i]->sid_command_list;
			sid_command_t *next;
			while(cmd) {
				next=cmd->next;
				free(cmd);
				cmd=next;
			}

			//if(cmd) {
			//	sid_command_t *next;
			//	do {
			//		next=cmd->next;
			//		free(cmd);
			//	} while(next);
			//}

			free(sid_instr[i]);
		}
		free(sid_instr);
	}
	if(num_instrs) sid_instr=malloc(sizeof(sid_instrument_t *)*(num_instrs+1));
	else {
		printf("Error: no instruments found in configuration file\n");
		exit(1);
	}

	j=0;
	for(i=0; groups[i]; i++) {
		if(!strcmp(groups[i], "channels")) continue;
		if(!strcmp(groups[i], "programs")) continue;

		printf("loading instrument: %s\n", groups[i]);
		sid_instr[j]=calloc(1, sizeof(sid_instrument_t));

		sid_instr[j]->name=groups[i];

		// defaults
		sid_instr[j]->description="";
		sid_instr[j]->vol_left=1.0;
		sid_instr[j]->vol_right=1.0;
		sid_instr[j]->program_speed=50.0;
		//sid_instr[j]->sid_commands[0].opcode=STOP;
		sid_instr[j]->sid_command_list=NULL;
		sid_instr[j]->type=NORMAL;

		// parameters from file
		err=NULL;
		value=g_key_file_get_value(inst_config, groups[i], "description", &err);
		if(value) sid_instr[j]->description=value;

		err=NULL;
		value=g_key_file_get_value(inst_config, groups[i], "type", &err);
		if(value && !strcmp("percussion", value)) {
			sid_instr[j]->type=PERCUSSION_MAP;
			for(k=0; k<128; k++) {
				char nr[255];
				snprintf(nr, 255, "%d", k);
				err=NULL;
				value=g_key_file_get_value(inst_config, groups[i], nr, &err);
				if(value) sid_instr[j]->percussion_mapn[k]=value;
				else sid_instr[j]->percussion_mapn[k]=NULL;
			}
		}

		// voice 1
		err=NULL;
		value=g_key_file_get_value(inst_config, groups[i], "v1_freq", &err);
		if(value) sid_instr[j]->v1_freq=readn(value);

		err=NULL;
		value=g_key_file_get_value(inst_config, groups[i], "v1_pulse", &err);
		if(value) sid_instr[j]->v1_pulse=readn(value);

		err=NULL;
		value=g_key_file_get_value(inst_config, groups[i], "v1_control", &err);
		if(value) sid_instr[j]->v1_control=readn(value);

		err=NULL;
		value=g_key_file_get_value(inst_config, groups[i], "v1_ad", &err);
		if(value) sid_instr[j]->v1_ad=readn(value);

		err=NULL;
		value=g_key_file_get_value(inst_config, groups[i], "v1_sr", &err);
		if(value) sid_instr[j]->v1_sr=readn(value);

		// voice 2
		err=NULL;
		value=g_key_file_get_value(inst_config, groups[i], "v2_freq", &err);
		if(value) sid_instr[j]->v2_freq=readn(value);

		err=NULL;
		value=g_key_file_get_value(inst_config, groups[i], "v2_pulse", &err);
		if(value) sid_instr[j]->v2_pulse=readn(value);

		err=NULL;
		value=g_key_file_get_value(inst_config, groups[i], "v2_control", &err);
		if(value) sid_instr[j]->v2_control=readn(value);

		err=NULL;
		value=g_key_file_get_value(inst_config, groups[i], "v2_ad", &err);
		if(value) sid_instr[j]->v2_ad=readn(value);

		err=NULL;
		value=g_key_file_get_value(inst_config, groups[i], "v2_sr", &err);
		if(value) sid_instr[j]->v2_sr=readn(value);

		// voice 3
		err=NULL;
		value=g_key_file_get_value(inst_config, groups[i], "v3_freq", &err);
		if(value) sid_instr[j]->v3_freq=readn(value);

		err=NULL;
		value=g_key_file_get_value(inst_config, groups[i], "v3_pulse", &err);
		if(value) sid_instr[j]->v3_pulse=readn(value);

		err=NULL;
		value=g_key_file_get_value(inst_config, groups[i], "v3_control", &err);
		if(value) sid_instr[j]->v3_control=readn(value);

		err=NULL;
		value=g_key_file_get_value(inst_config, groups[i], "v3_ad", &err);
		if(value) sid_instr[j]->v3_ad=readn(value);

		err=NULL;
		value=g_key_file_get_value(inst_config, groups[i], "v3_sr", &err);
		if(value) sid_instr[j]->v3_sr=readn(value);

		// fc_lo/hi
		err=NULL;
		value=g_key_file_get_value(inst_config, groups[i], "filter_cutoff", &err);
		if(value) sid_instr[j]->filter_cutoff=readn(value);

		// fr_vic
		err=NULL;
		value=g_key_file_get_value(inst_config, groups[i], "fr_vic", &err);
		if(value) sid_instr[j]->fr_vic=readn(value);

		// filter_mode
		err=NULL;
		value=g_key_file_get_value(inst_config, groups[i], "filter_mode", &err);
		if(value) sid_instr[j]->filter_mode=readn(value);

		// program speed
		err=NULL;
		value=g_key_file_get_value(inst_config, groups[i], "program_speed", &err);
		if(value) sid_instr[j]->program_speed=strtod(value, NULL);

		// balance
		err=NULL;
		value=g_key_file_get_value(inst_config, groups[i], "vol_left", &err);
		if(value) sid_instr[j]->vol_left=strtod(value, NULL);

		err=NULL;
		value=g_key_file_get_value(inst_config, groups[i], "vol_right", &err);
		if(value) sid_instr[j]->vol_right=strtod(value, NULL);

		err=NULL;
		value=g_key_file_get_value(inst_config, groups[i], "panning", &err);
		if(value) sid_instr[j]->panning=readn(value);

		// program table
		err=NULL;
		gchar **keys=g_key_file_get_keys(inst_config, groups[i], NULL, &err);
		int k;
		sid_command_t **cmd_ptr=&(sid_instr[j]->sid_command_list);
		sid_command_t *cmd;
		for(k=0; keys[k]; k++) {
			if(strncmp(keys[k], ".", 1)) continue;

			err=NULL;
			value=g_key_file_get_value(inst_config, groups[i], keys[k], &err);
			if(!value) continue;

			const char *delims=",";
			char *line=strdup(value);
			char *token;
			while(token=strsep(&line, delims)) {
				cmd=calloc(1, sizeof(sid_command_t));
				cmd->next=NULL;
				cmd->opcode=NOP;

				*cmd_ptr=cmd;
				cmd_ptr=&(cmd->next);

				int line_no=readn(keys[k]+1);
				cmd->line_number=line_no;

				while(isspace(*token)) token++;
				int l;
				for(l=0; opnames[l]; l++) {
					int oplen;
					for(oplen=0; token[oplen]; oplen++) {
						if(isspace(token[oplen])) break;
					}
					if(!strncmp(opnames[l], token, oplen)
						&& (strlen(opnames[l])==oplen)) break;
				}
				cmd->opcode=l;
				switch(l) {
					case PRINT:
						cmd->data_ptr=token+strlen(opnames[l]);
						break;
					case INVALID:
						printf("unknown command in instrument table: %s\n", token);
						break;
					default:
						cmd->data1=readn(token+strlen(opnames[l]));
						break;
				}
			}
			free(line);
		}
		j++;
	}
	sid_instr[j]=NULL;

	// programs
	err=NULL;
	gchar **programs=g_key_file_get_keys(inst_config, "programs", NULL, &err);
	if(programs) {
		for(i=0; i<128; i++) {
			midi_programs[i]=-1;
			char prg_str[255];
			snprintf(prg_str, 255, "%d", i+1);
			err=NULL;
			value=g_key_file_get_value(inst_config, "programs", prg_str, &err);
			if(value) {
				for(j=0; sid_instr[j]; j++) {
					if(!strcmp(sid_instr[j]->name, value)) {
						midi_programs[i]=j;
						//printf("midi program %d: %d\n", i, j);
						break;
					}
				}
			}
		}
	}
	printf("%d instruments loaded from %s\n", num_instrs, path);
	//for(i=0; i<128; i++) printf("midi program: %d instrument: %d\n", i, midi_programs[i]);

	// channels
	gchar **channels=g_key_file_get_keys(inst_config, "channels", NULL, &err);
	if(channels) {
		for(i=0; i<16; i++) {
			midi_channels[i].in_use=0;
			midi_channels[i].program=-1;
			char chan_str[255];
			snprintf(chan_str, 255, "%d", i+1);
			err=NULL;
			value=g_key_file_get_value(inst_config, "channels", chan_str, &err);
			if(value) {
				int ivalue=atoi(value)-1;
				if(ivalue<=127) {
					midi_channels[i].program=ivalue;
					midi_channels[i].in_use=1;
					//printf("channel %d: %d\n", i, ivalue-1);
				}
			}
		}
	}

	// set up any percussion map indexes
	for(i=0; sid_instr[i]; i++) {
		sid_instrument_t *instr=sid_instr[i];
		if(!instr->type==PERCUSSION_MAP) continue;
		for(j=0; j<128; j++) {
			instr->percussion_map[j]=-1;
			if(instr->percussion_mapn[j]==NULL) continue;
			char *name=instr->percussion_mapn[j];
			for(k=0; sid_instr[k]; k++) {
				if(!strcmp(name, sid_instr[k]->name)) instr->percussion_map[j]=k;
			}
		}
	}
}

