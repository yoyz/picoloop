
GUI=yes

ALSA_MIDI=yes
JACK_MIDI=yes

APP=reMID

CC=gcc

CFLAGS= -g \
	$(shell pkg-config jack --cflags) \
	$(shell pkg-config glib-2.0 --cflags)

ifeq ($(ALSA_MIDI), yes)
	CFLAGS+=-D ALSA_MIDI $(shell pkg-config alsa --cflags)
endif

ifeq ($(JACK_MIDI), yes)
	CFLAGS+=-D JACK_MIDI
endif

CPPFLAGS=-I/usr/include/resid

LDFLAGS= \
	$(shell pkg-config jack --libs) \
	$(shell pkg-config glib-2.0 --libs) \
	-lstdc++

ifeq ($(ALSA_MIDI), yes)
	LDFLAGS+=$(shell pkg-config alsa --libs)
endif

LIBS=-lresid

TARGETS=$(APP)

ifeq ($(GUI), yes)
	GUI_OBJ=gui.o
	CFLAGS+=-DGUI \
		$(shell pkg-config gtk+-2.0 --cflags) \
		$(shell pkg-config libglade-2.0 --cflags)
	LDFLAGS+= \
		$(shell pkg-config gmodule-export-2.0 --libs) \
		$(shell pkg-config libglade-2.0 --libs)
endif

OBJS=$(APP).o $(GUI_OBJ) jack_audio.o sid_chips.o midi.o
ifeq ($(ALSA_MIDI), yes)
	OBJS+=alsa_midi.o
endif
ifeq ($(JACK_MIDI), yes)
	OBJS+=jack_midi.o
endif
OBJS+=prefs.o

all: $(TARGETS)

$(APP): $(OBJS) $(LIBS)

clean:
	rm -f $(TARGETS) *.o


