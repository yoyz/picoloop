
#include <stdio.h>

#include <gtk/gtk.h>
#include <glib.h>
#include <glade/glade.h>

#include "jack_audio.h"
#include "midi.h"
#include "sid_chips.h"
#include "sid_instr.h"
#include "gui.h"
#include "prefs.h"

GtkWidget *main_window;
//GtkSpinButton *spinbutton1;
//GtkLabel *label_polyphony;
GtkLabel *label_instr_file;
GtkSpinButton *poly_spinbutton;
//char label_polyphony_str[255];

GtkFileChooser *instr_chooser_dialog;
GtkTable *chan_table;
GtkComboBox *chan_lists[16];
//GtkProgressBar *progbar;
GtkProgressBar *usage_bars[16];
double velocities[16];

void reload_instruments(void);

void update_progs() {
	int i;
	pthread_mutex_lock(&prefs_mutex);
	for(i=0; i<16; i++) {
		velocities[i]=((double)midi_channels[i].last_velocity)/127.0;
		midi_channels[i].last_velocity=0;
	}
	pthread_mutex_unlock(&prefs_mutex);

	for(i=0; i<16; i++) {
		gtk_progress_bar_set_fraction(usage_bars[i], velocities[i]);
	}
}

int gui_update() {
	int i;
	int chan_progs[16];

	// get data
	pthread_mutex_lock(&prefs_mutex);
	for(i=0; i<16; i++) {
		if(midi_channels[i].in_use) chan_progs[i]=midi_channels[i].program+1;
		else chan_progs[i]=0;
	}
	pthread_mutex_unlock(&prefs_mutex);

	// update gui
	for(i=0; i<16; i++) gtk_combo_box_set_active(chan_lists[i], chan_progs[i]);

	update_progs();

	return TRUE;
}

//void on_file_cancel() {
	//gtk_widget_hide((GtkWidget *)instr_chooser_dialog);
//	gtk_widget_destroy((GtkWidget *)instr_chooser_dialog);
//}

//void on_file_open() {
	//gtk_widget_hide((GtkWidget *)instr_chooser_dialog);
//	instr_file=(char *)gtk_file_chooser_get_filename(instr_chooser_dialog);
//	gtk_widget_destroy((GtkWidget *)instr_chooser_dialog);
//	reload_instruments();
//}

void on_file_open_selected() {
	//printf("file/open selected\n");

	instr_chooser_dialog=(GtkFileChooser *)gtk_file_chooser_dialog_new(
		"Open instruments", (GtkWindow *)main_window,
		//GTK_FILE_CHOOSER_ACTION_OPEN, "Open");
		GTK_FILE_CHOOSER_ACTION_OPEN,
		GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
		GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
		NULL);
	//gtk_widget_show_all((GtkWidget *)instr_chooser_dialog);

	if(gtk_dialog_run(GTK_DIALOG(instr_chooser_dialog))==GTK_RESPONSE_ACCEPT) {
		instr_file=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(instr_chooser_dialog));
		reload_instruments();
	}
	gtk_widget_destroy((GtkWidget *)instr_chooser_dialog);
}

gboolean on_button_audition_pressed(GtkWidget *widget, GdkEventButton *event, gpointer user_data) {
	//printf("audition channel %d\n", user_data+1);
	pthread_mutex_lock(&prefs_mutex);
	note_on((int)user_data , 60, 64);
	//note_on((int)user_data , 36, 127);
	pthread_mutex_unlock(&prefs_mutex);
	return TRUE;
}

gboolean on_button_audition_released(GtkWidget *widget, GdkEventButton *event, gpointer user_data) {
	//printf("audition channel %d released\n", user_data+1);
	pthread_mutex_lock(&prefs_mutex);
	note_off((int)user_data, 60);
	//note_off((int)user_data, 36);
	pthread_mutex_unlock(&prefs_mutex);
	return TRUE;
}

void on_poly_update_clicked() {
	int newpoly=gtk_spin_button_get_value(poly_spinbutton);
	pthread_mutex_lock(&prefs_mutex);
	prefs_set_polyphony(newpoly);
	pthread_mutex_unlock(&prefs_mutex);
	init_jack_audio();
	gtk_spin_button_set_value(poly_spinbutton, (guint)polyphony);
}

void clear_chan_lists() {
	int i;
	for(i=0; i<16; i++) gtk_object_destroy((GtkObject *)chan_lists[i]);
}

void on_program_changed(GtkComboBox *widget, gpointer data) {
	int channel=(int)data;
	int program=(int)gtk_combo_box_get_active(widget);
	//printf("%d selected %d\n", channel, program);
	pthread_mutex_lock(&prefs_mutex);
	if(program==0) midi_channels[channel].in_use=0;
	else if(program>=1 && program <=128) {
		midi_channels[channel].in_use=1;
		midi_channels[channel].program=program-1;
	}
	pthread_mutex_unlock(&prefs_mutex);
}

void init_usage_bars() {
	int i;
	for(i=0; i<16; i++) {
		usage_bars[i]=(GtkProgressBar *)gtk_progress_bar_new();
		gtk_table_attach(chan_table, (GtkWidget *)usage_bars[i], 3, 4, i, i+1,
			GTK_FILL|GTK_EXPAND, GTK_FILL|GTK_EXPAND, 1, 1);
	}
}

void init_chan_lists() {
	int i,j;
	int prog_num, inst_num;
	char prg_str[255];
	sid_instrument_t *instr;

	for(i=0; i<16; i++) {
		chan_lists[i]=(GtkComboBox *)gtk_combo_box_new_text();
		gtk_combo_box_append_text(chan_lists[i], "-- Channel off --");
		j=0;
		for(prog_num=0; prog_num<128; prog_num++) {
			inst_num=midi_programs[prog_num];
			if(inst_num!=-1) {
				instr=sid_instr[inst_num];
				snprintf(prg_str, 255, "%d: %s", prog_num+1, instr->name);
			} else snprintf(prg_str, 255, "%d: ", prog_num+1);
			gtk_combo_box_append_text(chan_lists[i], prg_str);
		}
		gtk_table_attach(chan_table, (GtkWidget *)chan_lists[i], 1, 2, i, i+1,
			GTK_FILL|GTK_EXPAND, GTK_FILL|GTK_EXPAND, 1, 1);

		g_signal_connect(G_OBJECT(chan_lists[i]), "changed",
			G_CALLBACK(on_program_changed), (gpointer)i);
	}
	gtk_widget_show_all((GtkWidget *)chan_table);
}

void reload_instruments() {
	pthread_mutex_lock(&prefs_mutex);
	prefs_read_instruments(instr_file);
	pthread_mutex_unlock(&prefs_mutex);
	clear_chan_lists();
	init_chan_lists();
	gtk_label_set_text(label_instr_file, instr_file);
}

void on_reload_instruments_clicked(GtkButton *widget, gpointer data) {
	reload_instruments();
}

void on_button_silence_all_clicked(GtkButton *widget, gpointer data) {
	printf("Silencing all channels\n");
	silence_all();
}

void *gui_start(void *args) {
	GladeXML *xml;
	int i;

	int argc=((gui_args_t *)args)->argc;
	char **argv=((gui_args_t *)args)->argv;
	gtk_init(&argc, &argv);

	xml=glade_xml_new("reMID.glade", NULL, NULL);
	glade_xml_signal_autoconnect(xml);

	main_window=glade_xml_get_widget(xml, "main_window");
	//instr_chooser_dialog=(GtkFileChooser *)glade_xml_get_widget(xml, "instr_chooser_dialog");
	//progbar=(GtkProgressBar *)glade_xml_get_widget(xml, "progressbar1");
	label_instr_file=(GtkLabel *)glade_xml_get_widget(xml, "label_instr_file");
	gtk_label_set_text(label_instr_file, instr_file);

	GtkButton *aud_buttons[16];
	char button_str[255];
	for(i=0; i<16; i++) {
		snprintf(button_str, 255, "button_audition%d", i+1);
		aud_buttons[i]=(GtkButton *)glade_xml_get_widget(xml, button_str);
		g_signal_connect(G_OBJECT(aud_buttons[i]), "button-press-event",
			G_CALLBACK(on_button_audition_pressed), (gpointer)i);
		g_signal_connect(G_OBJECT(aud_buttons[i]), "button-release-event",
			G_CALLBACK(on_button_audition_released), (gpointer)i);
	}

	chan_table=(GtkTable *)glade_xml_get_widget(xml, "chan_table");
	init_chan_lists();
	init_usage_bars();

	poly_spinbutton=(GtkSpinButton *)glade_xml_get_widget(xml, "poly_spinbutton");
	gtk_spin_button_set_value(poly_spinbutton, (guint)polyphony);

	g_timeout_add(500, gui_update, NULL);

	g_object_unref(xml);
	gtk_widget_show_all(main_window);
	gtk_main();

	return NULL;
}

