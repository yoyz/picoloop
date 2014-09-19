# README

Picoloop is a Stepsequencer  ( a nanoloop clone ) which means that a pattern of 1/16 notes is played repeatedly while these notes can be edited in various respects like volume, pitch etc. 
All parameters are set step-wise.            
There are four channels, playing simultaneously. 
Each channel's patterns can be saved to file slots  and are then available for new combinations. 
Finally, saved patterns can be arranged in a song structure ( will be implemented soon )).

Besides the song editor and manual loading of patterns, there are 3 different ways to modify each channel's patterns so that they form longer structures:                                       

- 1/2, 1/4 or 1/8 speed                                                                        
- Different pattern lengths


Picoloop is target for linux compatible system.
It use SDL     to render graphics.
It use RtAudio to render audio out.


# HOW IT WORKS

In menu you can select channel and edit parameter.                                           
All notes in the sequencer can be edited simultaneously.                                       

</>             select menu1 entry                                                             
^/v             select channel                                                                 
B               return to sequencer                                                            
A + </>/^/v,    edit all notes at once                                                         
SELECT          call menu 2                                                                    
        
        
menu1 : 
- A/R : 
    - Amp Enveloppe 
        - Attack/Release Amp or FM operator1                                                   
        - Decay/Sustain Amp or FM operator1                                                    
    - Filter Enveloppe                                                                         
        - Attack/Release Filter or FM Operator2                                                
        - Decay/Sustain Filter or FM Operator2                                                 
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
    - depth/speed in synth and drum synth mode                                                 

- FLTR : 
    - Cutoff/Resonance
    - mode ( lp/bp/hp )


menu 2 :
- L/S :
    - Load/Save pattern live

- BANK :
    - will give the ability to choose another bank for save/load pattern and song ( not implemented )

- PSH :
    - shift the pattern to the left/right, reduce the size of the pattern ( from 16 step to 1 step )

- MAC :
    - change the synth engine : synth, drumsynth, fm

- FX :
    - change the depth and the speed of the echo delay

- BPM :
    - change the bpm of the track






# PACKAGE DEPENDENCY & DEVEL PACKAGE

     - libsdl*      => version 1.2 the 2.0 was not tested "ttf" and "gfx" needed
     - libasound2*  => alsa
     - libpthread*  => pthread
     - g++          => I use the g++ 4.7.2  
     - make         => U use the make 3.81


# LASTEST SOURCE

<tt> # git clone https://github.com/yoyz/audio.git </tt> </br>


# COMPILATION

     # cd      picoloop
     # make -f Makefile.RtAudio_debian               clean
     # make -f Makefile.PatternPlayer_debian_RtAudio clean
     # make -f Makefile.RtAudio_debian
     # make -f Makefile.PatternPlayer_debian_RtAudio

# LAUNCHING

<tt> # ./PatternPlayer				</tt> <br>



# KEYS

## Debian

    - ESC               : enter menu and move between menu
    - ENTER             : Start key/enter a menu
    - L-CTRL            : insert note/delete node/copy note
    - ESC+ENTER         : quit

Depending on the current menu selected : A/R, OSC, VCO, BPM, LS, etc.

    - L-ALT + up        : change value up
    - L-ALT + down      : change value down
    - L-ALT + left      : change value left
    - L-ALT + right     : change value right


## OpenDingux

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
The old "data.pic" which contain all patern was removed.


# DEBUG

You need two file font.bmp and font.ttf which are located next to the source.








