# PICOLOOP 0.75 README #

Manual and tutorial for PC/PSP/Pocketchip can be fetch here : https://github.com/farvardin/picoloop-manual


## Overview ##

Picoloop is a synth and a stepsequencer (a clone of the famous nanoloop).
The step sequencer play 4 tracks simultaneously, by default each track are 16 steps long, you can set the track to be up to 128 steps.
Each track has a synthengine which can be a Virtual Analog, a Drum Synth, a FM synth.
Each of the step can be edited on the 4 tracks.
So for example, you can set a C4 note on step 0, nothing on step 1 and 2, then a D+5 on step 3 until the step 15.

A pattern of 16 steps is played repeatedly while these notes can be edited in various respects like volume, pitch, filter, lfo etc. 
All parameters are set step-wise, so you can make huge variation of sound on the same pattern.
Each channel's patterns can be saved to file slots and are then available for new combinations. 
Finally, saved patterns can be arranged in a song structure.

Besides the song editor and manual loading of patterns, there are 3 different ways to modify each channel's patterns so that they form longer structures:                                       

- 1/2, 1/4 or 1/8 speed                                                               
- Different pattern lengths by track


Picoloop is targeted for linux/SDL compatible system.
It uses SDL     to render graphics.
It uses RtAudio or SDL to render audio out.
It uses RtMidi  to send and receive midi message, today only clock


## How it works ##


There is two pane :
 - the 16 steps ;
 - the menu which is above the 16 steps ;
 
On the 16 boxes, there is a moving cursor which displays the current played step.
And another cursor which displays the current selected step.
There is a text menu, which allows you to select channel and edit parameter.
All notes in the sequencer can be edited simultaneously.

### The menu mode ###


The menu mode changes the selected textual menu at the bottom of the screen.
This allows you to choose what kind of parameter you want to change, for example, the NOTE, the OSC, the Filter...
In menu mode, when you press left/right, it changes the textual menu in the bottom of the screen

    </>             move backward/forward in the menu
    ^/v             select the track from 0 to track 3
    B               do nothing
    A + </>/^/v,    edit all steps at once
    A               enter the edit mode
    L               go to the next     16 steps on the same track
    R               go to the previous 16 steps on the same track
    SELECT          call menu2 or menu1 ( it cycle between the two menu )


### The edit mode ###

The edit mode changes the current step on the 16 steps display.
In edit mode, when you press left/right, it changes the current step in the sequencer.


    </>             select next step
    ^/v             select step+4, step-4
    B               enter a note in a step, it works as a cut/paste
    A + </>/^/v,    edit the current step
    A               do nothing
    L               go to the next     16 step on the same track
    R               go to the previous 16 step on the same track
    SELECT          call menu2 or menu1 ( it cycle between the two menu )
    START           cycle between the different parameter





#### menu1 ####

The menu 1 is the first menu. It allows to modify the parameter of the synthengine of the current track.
Each synth engine has different kind of parameter.


You can:

1. A/R  : the enveloppe of the synth
2. Note : the value of the note trigged
3. OSC  : the oscillator of the synth
4. VCO  : the vco parameter for example a osc1 and osc2 mixer
5. LFO  : the lfo parameter
6. FLTR : the filter parameter.



Here is the global overview of the most common parameters you will find by synth engine.

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
    - Choose the two OSC shape of the synth, the drumsynth, the FM synth                             

- VCO : 
    - VCO mix in synth mode, OP1mult/OP2mult in FM mode                                        
    - OP1amp/OP2amp in FM mode                                                                 
    
- LFO : 
    - depth/speed in psynth and drumsynth mode 
    - pitchbenddepth/pitchbendspeed in psynth mode 

- FLTR : 
    - Cutoff/Resonance
    - algo/mode ( lp/bp/hp )


#### menu 2 ####

The menu 2 is a more general menu. You can:

1. L/S   load and save a pattern  track or the 4 pattern track ;
2. BANK  change the current bank which allow you to have more pattern.
3. PSH   shift left or right the pattern of a track and add bunch of 16 step ;
4. MAC   change the current synth engine, synth, drumsynth, dbopl (adlib fm)
5. FX    apply fx on the current track delay
6. BPM   change the BPM and swing of the 4 track and the step diviser of the current track


##### L/S menu

The top menu shows the tracks and patterns. 
- Save your current track by selecting an empty slow, then B+down.
- Save your current pattern (group of 4 tracks) by selecting a column, then A+down.
- Load a track with A+up (it will replace your current working track).
- Load a pattern with B+up (il will replace the 4 current working tracks).

You can clone a track by loading it into memory, then saving it to another track.

The menu below (song position) allows you to order your tracks. You can enter it with Start key. 
- Define Loop start position with A+up
- Define Loop end position with A+down
- Change the values with B+up and B+down.

##### PSH menu

- Increase the track size (16 steps block) with A+UP
- Decrease the track size (16 steps block) with A+DOWN
- Increase the track size (1 step) with A+RIGHT
- Decrease the track size (1 step) with A+LEFT
- Navigate into the new steps (move the position by -16 or +16) with the LEFT and RIGHT SHOULDER (or TAB and BACKSPACE on PC)



## What you need to know ##

Every pattern is stored in the bank/bankXXX/file so if you backup this directory you should be fine.


You can write pattern on Dingux and push it on your laptop, unfortunately, it could sound little different.
This is not the case for PSP which is able to sound the same.
You won't be able to push pattern from PC to PSP/Opendingux, it could work but it depend if the Machine is available on the PSP.



The combo Select+UP, Select+Down allow you to raise or decrease the volume.


If you plug on Windows/Linux a midi cable you can use it with midi sync in or midi sync out.
The MMC message ( "play", "stop" ) are received by picoloop, but it doesn't transmit this kind of message.
It understand the "stop" and "play" MMC message. 


The MDA Drum synth works with file patch ".ds" file.
So if you have a laptop with a harddrive which is not an ssd, you should work with a PSU and not the battery to
avoid disk shutdown. The automatic disk shutdown will block audio for 10ms and it became audible.
Will be fixed in a latter version.



### Package dependency, build from source ###


This section describes the dependency for building picoloop.
On most platform in 2016, you will need this kind of package.

     - libsdl*      => version 1.2 with the "ttf" and "gfx"
     - libasound2*  => alsa
     - libpthread*  => pthread for multiple thread
     - g++          => I use the g++ 4.7.2  
     - make         => I use the make 3.81
     - libjack-dev  => I use jack 2.0, and the jack dependency is needed today 
     


#### LASTEST SOURCE ####

You can always download the lastest source.
But, you should probably switch to a tag then.


<tt> # git clone https://github.com/yoyz/audio.git </tt> </br>
<tt> # git tag </tt> </br>
<tt> # git checkout picoloop-0.69 </tt> </br>

Tag should be safier.


#### COMPILATION FOR DEBIAN ####

     # cd      picoloop
     # make -f Makefile.RtAudio_debian               clean
     # make -f Makefile.RtMidi_debian                clean
     # make -f Makefile.PatternPlayer_debian_RtAudio clean
     # make -f Makefile.RtAudio_debian
     # make -f Makefile.RtMidi_debian
     # make -f Makefile.PatternPlayer_debian_RtAudio

#### COMPILATION FOR POCKETCHIP ####

You have to build it on the pocketchip.

     # cd      picoloop
     # make -f Makefile.RtAudio_pocketchip               clean
     # make -f Makefile.RtMidi_pocketchip                clean
     # make -f Makefile.PatternPlayer_pocketchip_RtAudio clean
     # make -f Makefile.RtAudio_pocketchip
     # make -f Makefile.RtMidi_pocketchip
     # make -f Makefile.PatternPlayer_pocketchip_RtAudio


#### COMPILATION FOR RASPBERRYPI1 ####

You have to build it directly on the raspberry.
If you have a raspberry 2 or 3, you should go to the "debian" section.
This build type is a light version like the opendingux one.

     # cd      picoloop
     # make -f Makefile.RtAudio_raspi1                  clean
     # make -f Makefile.PatternPlayer_raspi1_RtAudio    clean
     # make -f Makefile.RtAudio_raspi1                  
     # make -f Makefile.PatternPlayer_raspi1_RtAudio    


#### COMPILATION FOR OPENDINGUX ####

On debian you need the /opt/opendingux-toolchain/ directory.
you can fetch fetch it there : http://www.treewalker.org/opendingux/

     # cd      picoloop
     # make -j 64 -f Makefile.RtAudio_opendingux               clean
     # make -j 64 -f Makefile.PatternPlayer_opendingux_RtAudio clean

     # make -j 64 -f Makefile.RtAudio_opendingux
     # make -j 64 -f Makefile.PatternPlayer_opendingux_RtAudio

#### COMPILATION FOR GP2X ####

On debian you need the /opt/open2x/ directory.
you can fetch it there : http://wiki.gp2x.org/articles/i/n/s/Installing_the_Open2x_toolchain.html
I don't have build it month ago, so at your own risk.

     # cd      picoloop
     # make -j 64 -f Makefile.RtAudio_opendingux               clean
     # make -j 64 -f Makefile.PatternPlayer_opendingux_RtAudio clean

     # make -j 64 -f Makefile.RtAudio_opendingux
     # make -j 64 -f Makefile.PatternPlayer_opendingux_RtAudio



#### COMPILATION FOR WINDOWS WITH MINGW32 ####

On debian, you need the mingw32 package.
Picoloop provide the header and binary for SDL 1.2 on windows.
So here it is a "cross build" you build on linux for windows.

     # cd         picoloop
     # make       -f Makefile.RtAudio_windows                              clean
     # make       -f Makefile.RtMidi_windows                               clean
     # make       -f Makefile.PatternPlayer_windows_mingw_RtAudio          clean
     # make       -f Makefile.RtAudio_windows                              
     # make       -f Makefile.RtMidi_windows                               
     # make       -f Makefile.PatternPlayer_windows_mingw_RtAudio -j 64


#### COMPILATION FOR PSP ####

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
    - TAB               : LEFT Shoulder
    - BACKSPACE         : RIGHT Shoulder

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


# DEBUG PSP


You need your psp gcc compiler, the library and psplinkusb.



1) Launch psplink on the psp this is an EBOOT.PBP.
2) as root, launch usbhostfs_pc ( you need to be root to access the psp by usb and you need to be in the somewhere/psplinkusb/usbhostfs_pc/ folder ).
You should see it is connected.

3) In another terminal launch pspsh.  Try to 'ls -l', to see if all is ok.
Copy the prx file in the usbhostfs_pc directory, you should see it with 'ls -l'

If you want to launch PatternPlayer.prx, launch it with pspsh : ./PatternPlayer.prx
In this case, you need the prx file ( not an elf or EBOOT.PBP ).



If you want to debug it with psp-gdb, you can do it by this command in two different window.

         
         host0:/> debug PatternPlayer.prx

In another window : 
	 
         $ psp-gdb ./PatternPlayer.elf
	 (gdb) target remote localhost:10001
         Remote debugging using localhost:10001

In this case you need a prx and an elf file.






Changelog
=========

V0.75abc: 
- a add the resid engine 
- b export DUMP_AUDIO=1 now export a wav file
- c add SDL2.0 compatibility and keep SDL1.2 as default because it works


V0.74acd:

- add pocketchip platform, which is a "kind of variant" of raspberry pi 1, thank to garvalf from chipmusic.org
- fix a PatternElement.cpp issue related to a value not initialized 
- shift track to the left and to the write in the song "orderlist"
- fix a bug with the note tuning which appear from 0.71 to 0.74


V0.73 :

- add a midi delta option in the [BPM] menu to move the sync signal in time, it is a kind of "midi delay" but doesn't work exactly the same way
- add a 'osc scale' which allow to change oscillator one or oscillator two pitch to make basic chord with two note
- rework a bit BPSynth on opendingoo to allow to remove some bad FixedPoint behaviour
- fix first buffer audio generation of pbsynth which lead to an audio glitch


V0.71 :

- improve realtime of mda drum synth, now quite useable, but need to be improved


V0.70 :

- add a readme.html in the repo
- fix some glitch on the screen from 320x240 to 640x480

V0.69 :

- add midi in  sync on linux/windows
- add midi out sync on linux/windows
- midi on windows add issue with latency
- windows can run MDADrumSynth, still with issue with the latency

