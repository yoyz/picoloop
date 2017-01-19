
                 ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
                 ³ The PC GAMES PROGRAMMERS ENCYCLOPEDIA 1.0a ³
                 ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ


ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³ Introduction ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

Well, here it is! This is the first edition of the PC Games Programmers
Encyclopedia. The PC-GPE as it currently stands is a collection of text
files, each covering a different aspect of programming games for the PC.
Some files were obtained from the net, others were grabbed off Usenet, quite
a few were written for the PC-GPE.

Every effort has been made to contact the original authors of all public
domain articles obtained via ftp. In some cases the original authors were
not able to be contacted. Seeing as these files were already available to the
public the liberty was taken to include them anyway. The files were not
modified in any way. There is a list at the end of this document showing
which files we couldn't contact the authors about. Please note that files
were *not* written exclusively for the PC-GPE unless stated otherwise.

The information in the PC-GPE is provided to you free of charge. The authors
of each article have included their own conditions of use, eg some ask that
you give them credit if you use their source code. As a general rule of
thumb, an e-mail or postcard to an author telling them you found their file
helpful probably wouldn't go astray.....

This first version of the PC-GPE is very hardware oriented. We hope to
include more actual game algorithms in future releases. If you would like to
see a particular topic included in the next PC-GPE release or if you think
you could contribute an article then by all means let us know (btw plugs for
personal projects in articles are accepted). The editor's e-mail address is
at the end of this file.

Some of the text files are pretty long, so the PCGPE uses a protected mode
file viewer (PCGPE.EXE) which may play up when run on 286 machines. If this
happens read the DPMIUSER.DOC file for help on fixing the problem.

ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³ PC-GPE Home Site ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

The Games Programmers Encyclopedia official home site is:

teeri.oulu.fi
/pub/msdos/programming/gpe

There are plans to develop GPE's for the mac and other architectures for
cross-platform game development. The teeri site will also hold PC-GPE
updates/bug fixes/etc.

Many thanks to Jouni Miettunen for all his help and for allowing us to use
teeri as the PC-GPE's home site. He's put a lot of work into it and it's a
great programming resource, particularly for people wanting to develop game
software.


ÚÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³ History ³
ÀÄÄÄÄÄÄÄÄÄÙ

The PC-GPE was conceived, designed and largely built by the same people who
keep the Usenet groups rec.games.programmer and comp.graphics.algorithms
alive. It was noticed that information required for even the most basic game
development was strewn out across the vast wastelands of the Internet and was
time consuming and annoying (if not down-right impossible) to obtain.

Most of us can't afford to go out and buy a book every time we want to look
up a particlar topic, so a bunch of us decided to grab the most commonly
sought-after free info and put it in one place.


ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³ The People Who Did All the Work ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

First a big thanks goes to everyone who wrote articles or allowed us to use
their existing articles.

Also thanks to the Demo groups Asphixia and VLA (more specifically Lithium
and Denthor) for letting us use the asm and vga trainers they wrote.

A number of people who didn't actually write articles contributed heaps
to the project right from the start with tips/comments/suggestions etc as
well as lots of info on where we could get stuff. Thanks go to Bri, Dizzy,
Claus Anderson, Nathan Clegg, Alex Curylo, Cameron Grant, Chris Matrakidis
and the many others who sent info. If it wasn't for them you probably
wouldn't be reading this now!

And finally thanks to Jouni Miettunen for setting up the PC-GPE directory on
the teeri site, letting us use it as the official home site and supplying
a heap of information.

The editor would also like to thank the scores of other people who e-mailed
him with suggestions, comments, requests etc...and continually hassled him to
hurry up and get the damn thing finished.


ÚÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³ Disclaimer ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÙ

It's a pity we live in a world where the following kind of crap is
neccessary. Oh well, here goes....

Each article appearing in the PC-GPE is bound by any disclaimer that appears
within it. The editor assumes absolutely no responsibility whatsoever for
any effect that this file viewer or any of the PC-GPE articles have on you,
your sanity, computer, spouse, children, pets or anything else related to
you or your existance. No warranty is provided nor implied with this
information. The accuracy of the information contained is subject to
conjecture. Use all information at your own risk. The file PC-GPE.EXE may
not be distributed without all the original unmodified PC-GPE articles. The
distribution rights of individual articles is at the discretion of the
authors.


ÚÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³ File List ³
ÀÄÄÄÄÄÄÄÄÄÄÄÙ

The following is a list of all the PCGPE 1.0 files:

File               Description
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

PCGPE    EXE   *   PC-GPE main exe file
DPMIUSER DOC   *   PC-GPE.EXE DPMI info file
RTM      EXE   *   PC-GPE.EXE DPMI support file
RTMRES   EXE   *   PC-GPE.EXE DPMI support file
DPMIINST EXE   *   PC-GPE.EXE DPMI support file
DPMILOAD EXE   *   PC-GPE.EXE DPMI support file
DPMI16BI OVL   *   PC-GPE.EXE DPMI support file

README   TXT   *   PC-GPE main info doc
FTPSITES TXT       List of FTP sites for game development programs/utils

ASMINTRO TXT   *   VLA's assembly tutorial intro file
ASM0     TXT   *   VLA's assembly tutorial
ASM1     TXT   *   VLA's assembly tutorial
ASM2     TXT   *   VLA's assembly tutorial
ASM3     TXT   *   VLA's assembly tutorial
ANSI     TXT   *   VLA's assembly tutorial support file

INTEL    DOC       List of op codes plus timing info up to 486
CPUTYPE  TXT   *   Testing CPU type
TIMER    ASM   *   Testing CPU speed

TUT1     TXT   *   Asphixia's VGA Primer - Mode 13h
TUT2     TXT   *   Asphixia's VGA Primer - Palette/Fading
TUT3     TXT   *   Asphixia's VGA Primer - Lines/Circles
TUT4     TXT   *   Asphixia's VGA Primer - Virtual Screens
TUT5     TXT   *   Asphixia's VGA Primer - Scrolling
TUT6     TXT   *   Asphixia's VGA Primer - Look-up Tables
TUT7     TXT   *   Asphixia's VGA Primer - Animation
TUT8     TXT   *   Asphixia's VGA Primer - 3D/Optimisation
TUT9     TXT   *   Asphixia's VGA Primer - 3D Solids
TUT10    TXT   *   Asphixia's VGA Primer - Chain 4 mode
COPPER   PAS   *   Asphixia's VGA Primer - Copper Effect
WORMIE   PAS   *   Asphixia's VGA Primer - Worm Effect
PALLETTE COL   *   Asphixia's VGA Primer support file
SOFTROCK FNT   *   Asphixia's VGA Primer support file

MODEX    TXT   *   Introduction to mode x
SCROLL   TXT   *   VGA scrolling
VGAREGS  TXT   *   VGA palette and register set
VGABIOS  TXT       VGA BIOS function call list

SVGINTRO TXT   *   SVGA - Intro to programming SVGA cards
VESASP12 TXT       SVGA - The VESA standard
ATI      TXT   *   SVGA - Programming the ATI chip set
CAT      TXT   *   SVGA - Programming the Chips & Technologies chip set
GENOA    TXT   *   SVGA - Programming the Genoa chip set
PARADISE TXT   *   SVGA - Programming the Paradise chip set
TRIDENT  TXT   *   SVGA - Programming the Trident chip set
TSENG    TXT   *   SVGA - Programming the Tseng chip set
VIDEO7   TXT   *   SVGA - Programming the Video7 chip set
XTENDED  TXT   *   SVGA - 640x400x256 with no bank switching

3DROTATE DOC   *   VLA's three dimensional rotations for computer graphics
3DSHADE  DOC   *   VLA's three dimensional shading in computer graphics
PERSPECT TXT   *   Perspective transforms
BRES     TXT   *   Bresenham's line and circle algorithms
CONIC    CC    *   A bresenham-like general conic sections algorithm
BSP      TXT   *   A Simple Explanation of BSP Trees
TEXTURE  TXT   *   Texture mapping
FDTM     TXT   *   Real-time free direction texture mapping

STARS    TXT   *   VLA's programming star fields
FIRE     TXT   *   Programming fire effects

PCX      TXT       PCX graphics file format
BMP      TXT       BMP graphics file format
GIF      TXT       GIF graphics file format
IFF      DOC       IFF/LBM graphics file format
FLI      FOR       FLI/FLC graphics file format

SPEAKER  TXT   *   Programming the PC speaker (inc 8-bit sample playback)
GAMEBLST TXT   *   Programming the GameBlaster sound card
ADLIB    TXT       Programming the Adlib sound card
SBDSP    TXT   *   Programming the SoundBlaster sound card (DSP)
SBPRO    TXT   *   Programming the SoundBlaster Pro sound card
GUSFAQ   TXT   *   The GUS sound card's Frequently Asked Questions
GUS      TXT   *   Programming the GUS sound card

MODFORM  TXT   *   The MOD sound file format
VOC      TXT       The VOC sound file format
WAV      TXT   *   The WAV sound file format
CMF      TXT   *   The CMF sound file format
MIDI     TXT   *   The MID sound file format
UT       TXT       The UltraTracker sound file format

SURROUND TXT       Generating surround sound

MOUSE    TXT   *   Programming the mouse, general info
GMOUSE   DOC       Mouse driver function call list
KEYBOARD TXT   *   Programming for the PC keyboard
JOYSTICK TXT   *   Programming for the PC joystick
GAMEPAD  TXT   *   Programming for the Gravis GamePad and Analog Pro

LIMEMS41 DOC       EMS (Expanded Memory Specification)
XMS30    TXT       XMS (Extended Memory Specification)
DMA_VLA  TXT   *   Intro to DMA

PIT      TXT   *   Programming the Intel 8253 Programmable Interval Timer
DOOM     TXT   *   DOOM techniques


An asterix (*) indicates files which were either written for the PC-GPE or
included with permission from the author.


ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³ Final Words from the Editor ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

Greetz
ÄÄÄÄÄÄ

   Zob: Whaddaya mean you can't come out drinking with us for 6 months? What
        the hell is "glandular fever" anyway?

Wookie: Whaddaya mean I can't play ModemDOOM on a 2400?

  Fink: Live fast, die young, have a good lookin' corpse!

MainFrame, bri_acid, wReam, Nocturnus, MArtist, RetroSpec, Matrix, Syntax,
Andrez, Gideon and the rest of the #coders gang : try and get some sleep
                                                  some time guys!

  Eyre: You/me babe, how 'bout it?

  Aggi: Remember, reality is mass hallucination resulting from alcohol
        deficiency!

Fetish: You know the routine hon, pick a number and join the queue like
        the rest of 'em!


Why all my code is in Pascal
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

Ok, ok, I'm expecting to get lots of crap over this one. To put it simply
Pascal is close to psuedo code and I wanted the routines to be understood
by everyone, Pascal programmers, C programmers and *REAL* (to wit, asm)
programmers alike. Apart from that I'm running a 40Mg doublespaced hard
drive and I have to use the fastest compiler possible. That's a good enough
reason isn't it?....people?.....


Shameless Plug
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

There are two things in life I really can't stand,

1) My ex-girlfriend
2) Being unemployed, which I am now!

So if your company has any openings I'd really like to hear from you,
particularly if you develop game software.

I'm a 3rd year computer engineering student and my specialties lie in
computer graphics and low-level PC hardware programming. I program in C++
(Dos and Windows), Pascal, 80x86 assembly, QBasic (heh heh) and Prolog.


Mark Feldman
Internet: u914097@student.canberra.edu.au
          myndale@cairo.anu.edu.au

