# README

Picoloop is a nanoloop clone which is target for linux compatible system.
It use SDL     to render graphics.
It use RtAudio to render audio out.


# LASTEST SOURCE

<tt> # git clone https://github.com/yoyz/audio.git </tt> </br>


# COMPILATION

<tt> # cd picoloop 				</tt> <br>
<tt> # make -f Makefile.RtAudio clean 		</tt> <br>
<tt> # make -f Makefile.PatternPlayer clean 	</tt> <br>

<tt> # make -f Makefile.RtAudio  		</tt> <br>
<tt> # make -f Makefile.PatternPlayer		</tt> <br>

# LAUNCHING

<tt> # ./PatternPlayer				</tt> <br>



# KEYS

## Debian

    - ESC   		: enter menu and move between menu
    - ENTER 		: Start key/enter a menu
    - L-CTRL		: insert note/delete node/copy note
    - ESC+ENTER		: quit

Depending on the current menu selected : A/R, OSC, VCO, BPM, LS, etc.

    - L-ALT+up		: change value up
    - L-ALT+down	: change value down
    - L-ALT+left	: change value left
    - L-ALT+right	: change value right


## OpenDingux

    - SELECT   	: enter menu and move between menu
    - ENTER    	: Start key/enter a menu
    - A key		: insert note/delete node/copy note
    - ESC+ENTER	: quit

Depending on the current menu selected : A/R, OSC, VCO, BPM, LS, etc.

    - B key+up	: change value up
    - B key+down	: change value down
    - B key+left	: change value left 
    - B key+right	: change value right

# USAGE

file named dataP<%d>T<%d>.pic store information about saved pattern.
This file will be created when you store file in the "L/S" menu.
The old "data.pic" which contain all patern was removed.


# DEBUG

You need two file font.bmp and font.ttf which are located next to the source.


