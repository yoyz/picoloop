#!/bin/bash
make       -f Makefile.RtAudio_mingw32                              clean
make       -f Makefile.PatternPlayer_windows_mingw_RtAudio clean

make       -f Makefile.RtAudio_mingw32                              
make       -f Makefile.PatternPlayer_windows_mingw_RtAudio -j 2


