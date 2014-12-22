# README

Picoloop is a synth Stepsequencer  ( a nanoloop clone ).
A pattern of 16 notes is played repeatedly while these notes can be edited in various respects like volume, pitch, filter, lfo etc. 
All parameters are set step-wise, so you can make huge variation of sound on the same pattern.
There are four channels, playing simultaneously.
Each channel's patterns can be saved to file slots  and are then available for new combinations. 
Finally, saved patterns can be arranged in a song structure.

Besides the song editor and manual loading of patterns, there are 3 different ways to modify each channel's patterns so that they form longer structures:                                       

- 1/2, 1/4 or 1/8 speed                                                               
- Different pattern lengths


Picoloop is target for linux/SDL compatible system.
It use SDL     to render graphics.
It use RtAudio or SDL to render audio out.


# HOW IT WORKS

There is two pane, the 16 box and the menu which is above the 16 box.
On the 16 box, there is a moving cursor which display the current played step.
and another cursor which display the current selected step.
There is a text menu, which allow you to select channel and edit parameter.
All notes in the sequencer can be edited simultaneously.

</>             select menu1 entry
^/v             select channel 
B               return to sequencer  
A + </>/^/v,    edit all notes at once
SELECT          call menu2 or menu1 ( it cycle between the two menu )
        
        
menu1 : 
- A/R : 
    - Amp Enveloppe 
        - Attack/Release     Amp /  FM operator1   
        - Decay/Sustain      Amp /  FM operator1   
    - Filter Enveloppe           
        - Attack/Release Filter  /  FM Operator2       
        - Decay/Sustain  Filter  /  FM Operator2       
    -  Trig/Amplification
         - trig time/Volume                                                                    

- Note : 
    - Choose note with "C3" "C4" "D2"                                                          
    - Choose note with "dot"                                                                   
    
- OSC : 
    - Choose the two OSC of the synth, the drumsynth, the FM synth                             

- VCO : 
    - VCO mix in synth mode, OP1mult/OP2mult in FM mode                                        
    - OP1amp/OP2amp in FM mode                                                                 
    
- LFO : 
    - depth/speed in psynth and drumsynth mode 
    - pitchbenddepth/pitchbendspeed in psynth mode 

- FLTR : 
    - Cutoff/Resonance
    - algo/mode ( lp/bp/hp )


menu 2 :
- L/S :
    - Load/Save pattern live

- BANK :
    - give the ability to choose another bank for save/load pattern and song 

- PSH :
    - shift the pattern to the left/right, reduce the size of the pattern ( from 16 step to 1 step )

- MAC :
    - change the synth engine : synth, drumsynth, dbopl ( adlib fm )

- FX :
    - change the depth and the speed of the echo delay

- BPM :
    - change the bpm of the track and change the swing






# PACKAGE DEPENDENCY & DEVEL PACKAGE

     - libsdl*      => version 1.2 the 2.0 was not tested "ttf" and "gfx" needed
     - libasound2*  => alsa
     - libpthread*  => pthread
     - g++          => I use the g++ 4.7.2  
     - make         => U use the make 3.81


# LASTEST SOURCE

<tt> # git clone https://github.com/yoyz/audio.git </tt> </br>


# COMPILATION FOR DEBIAN

     # cd      picoloop
     # make -f Makefile.RtAudio_debian               clean
     # make -f Makefile.PatternPlayer_debian_RtAudio clean
     # make -f Makefile.RtAudio_debian
     # make -f Makefile.PatternPlayer_debian_RtAudio

# COMPILATION FOR OPENDINGUX

On debian you need the /opt/opendingux-toolchain/ directory.
you can fetch fetch it there : http://www.treewalker.org/opendingux/

     # cd      picoloop
     # make -j 64 -f Makefile.RtAudio_opendingux               clean
     # make -j 64 -f Makefile.PatternPlayer_opendingux_RtAudio clean

     # make -j 64 -f Makefile.RtAudio_opendingux
     # make -j 64 -f Makefile.PatternPlayer_opendingux_RtAudio

# COMPILATION FOR GP2X

On debian you need the /opt/open2x/ directory.
you can fetch it there : http://wiki.gp2x.org/articles/i/n/s/Installing_the_Open2x_toolchain.html

     # cd      picoloop
     # make -j 64 -f Makefile.RtAudio_opendingux               clean
     # make -j 64 -f Makefile.PatternPlayer_opendingux_RtAudio clean

     # make -j 64 -f Makefile.RtAudio_opendingux
     # make -j 64 -f Makefile.PatternPlayer_opendingux_RtAudio



# COMPILATION FOR WINDOWS WITH MINGW32

On debian, you need the mingw32 package.
Picoloop provide the header and binary for SDL 1.2 on windows.

     # cd         picoloop
     # make       -f Makefile.RtAudio_mingw32                              clean
     # make       -f Makefile.PatternPlayer_windows_mingw_RtAudio clean

     # make       -f Makefile.RtAudio_mingw32
     # make       -f Makefile.PatternPlayer_windows_mingw_RtAudio -j 64


# COMPILATION FOR PSP

you need to have the sdk, the toolchain and the external lib.
All can be fetched here : https://github.com/pspdev

     # cd      picoloop
     # source ~/local/pspdev/env_build
     # make -f Makefile.PatternPlayer_psp_SDL        clean
     # make -f Makefile.PatternPlayer_psp_SDL 


# LAUNCHING

<tt> # ./PatternPlayer				</tt> <br>



# KEYS

## Debian/Windows

    - ESC               : Select key : go back to the global menu/switch menu
    - ENTER             : Start  key : change sub menu
    - L-CTRL            : A      key : insert note/delete node/copy note
    - L-ALT             : B      key : change the value on the screen of the selected step
    - ESC+ENTER         : quit

Depending on the current menu selected : A/R, OSC, VCO, BPM, LS, etc.

    - L-ALT + up        : change value up
    - L-ALT + down      : change value down
    - L-ALT + left      : change value left
    - L-ALT + right     : change value right


## OpenDingux/PSP/GP2X

    - SELECT            : enter menu and move between menu
    - ENTER             : Start key/enter a menu
    - A key             : insert note/delete node/copy note
    - ESC+ENTER         : quit

Depending on the current menu selected : A/R, OSC, VCO, BPM, LS, etc.

    - B key + up        : change value up
    - B key + down      : change value down
    - B key + left      : change value left 
    - B key + right     : change value right

# USAGE

file named dataP<%d>T<%d>.pic store information about saved pattern.
This file will be created when you store file in the "L/S" menu.


# DEBUG

You need two file font.bmp and font.ttf which are located next to the source.








