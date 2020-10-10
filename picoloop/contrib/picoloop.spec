# Disable production of debug package.
%global debug_package %{nil}

# Global variables for github repository
%global commit0 240d970c126d96472a91ccf3f2c3179ba2b15163
%global gittag0 master
%global shortcommit0 %(c=%{commit0}; echo ${c:0:7})

Name:    picoloop
Version: 0.77e.%{shortcommit0}
Release: 3%{?dist}
Summary: An audio sequencer

Group:   Applications/Multimedia
License: GPLv2+
URL:     https://github.com/yoyz/audio
Source0: https://github.com/yoyz/audio/archive/%{commit0}.tar.gz#/%{name}-%{shortcommit0}.tar.gz
Source1: picoloop-SYSTEMLINUX.cpp
Source2: https://github.com/farvardin/picoloop-manual/files/500912/picoloop_manual_pc.pdf

BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires: gcc gcc-c++
BuildRequires: alsa-lib-devel
BuildRequires: jack-audio-connection-kit-devel
BuildRequires: SDL2-devel
BuildRequires: SDL2_image-devel
BuildRequires: SDL2_gfx-devel
BuildRequires: SDL2_ttf-devel
BuildRequires: pulseaudio-libs-devel
BuildRequires: desktop-file-utils

Requires: SDL2_ttf

%description
Picoloop is a synth Stepsequencer (a nanoloop clone).
A pattern of 16 notes is played repeatedly while these notes can be edited in various respects like volume, pitch, filter, lfo etc.
All parameters are set step-wise, so you can make huge variation of sound on the same pattern.
There are four channels, playing simultaneously.

%prep
%setup -qn %{name}-%{commit0}

sed -i -e "s/\"font.ttf\"/\"\/usr\/share\/picoloop\/font\/font.ttf\"/g" picoloop/SDL_GUI.cpp
sed -i -e "s/\"font.bmp\"/\"\/usr\/share\/picoloop\/bmp\/font.bmp\"/g" picoloop/SDL_GUI.cpp

cp %{SOURCE1} picoloop/SYSTEMLINUX.cpp

%build

cd picoloop

#
# Build PulseAudio version
#

#sed -i -e "s/-D__LINUX_PULSE__/-D__LINUX_PULSE__ -D__UNIX_JACK__ -D__LINUX_ALSA__/g" Makefile.RtMidi_debian_sdl20
#sed -i -e "s/-D__LINUX_PULSE__/-D__LINUX_PULSE__ -D__UNIX_JACK__ -D__LINUX_ALSA__/g" Makefile.RtAudio_debian_sdl20
#sed -i -e "s/-D__LINUX_PULSE__/-D__LINUX_PULSE__ -D__UNIX_JACK__ -D__LINUX_ALSA__/g" Makefile.PatternPlayer_debian_RtAudio_sdl20

make -f Makefile.RtMidi_debian_sdl20 clean
make -f Makefile.RtAudio_debian_sdl20 clean
make -f Makefile.PatternPlayer_debian_RtAudio_sdl20 clean

mkdir -p debianrtaudio_sdl20/Machine/Lgptsampler

make -f Makefile.RtMidi_debian_sdl20  
make -f Makefile.RtAudio_debian_sdl20 
make -f Makefile.PatternPlayer_debian_RtAudio_sdl20

mv PatternPlayer_debian_RtAudio_sdl20 picoloop-pulse

#
# Build Alsa version
#

sed -i -e "s/-D__LINUX_PULSE__/-D__LINUX_ALSA__/g" Makefile.RtMidi_debian_sdl20
sed -i -e "s/-D__LINUX_PULSE__/-D__LINUX_ALSA__/g" Makefile.RtAudio_debian_sdl20
sed -i -e "s/-D__LINUX_PULSE__/-D__LINUX_ALSA__/g" Makefile.PatternPlayer_debian_RtAudio_sdl20

make -f Makefile.RtMidi_debian_sdl20 clean
make -f Makefile.RtAudio_debian_sdl20 clean
make -f Makefile.PatternPlayer_debian_RtAudio_sdl20 clean

mkdir -p debianrtaudio_sdl20/Machine/Lgptsampler

make -f Makefile.RtMidi_debian_sdl20  
make -f Makefile.RtAudio_debian_sdl20 
make -f Makefile.PatternPlayer_debian_RtAudio_sdl20

mv PatternPlayer_debian_RtAudio_sdl20 picoloop-alsa

#
# Build Jack version
#

sed -i -e "s/-D__LINUX_ALSA__/-D__UNIX_JACK__/g" Makefile.RtMidi_debian_sdl20
sed -i -e "s/-D__LINUX_ALSA__/-D__UNIX_JACK__/g" Makefile.RtAudio_debian_sdl20
sed -i -e "s/-D__LINUX_ALSA__/-D__UNIX_JACK__/g" Makefile.PatternPlayer_debian_RtAudio_sdl20

make -f Makefile.RtMidi_debian_sdl20 clean
make -f Makefile.RtAudio_debian_sdl20 clean
make -f Makefile.PatternPlayer_debian_RtAudio_sdl20 clean

mkdir -p debianrtaudio_sdl20/Machine/Lgptsampler

make -f Makefile.RtMidi_debian_sdl20  
make -f Makefile.RtAudio_debian_sdl20 
make -f Makefile.PatternPlayer_debian_RtAudio_sdl20

mv PatternPlayer_debian_RtAudio_sdl20 picoloop-jack

%install

install -m 755 -d %{buildroot}/%{_datadir}/applications/

cat > %{buildroot}/%{_datadir}/applications/picoloop-jack.desktop <<EOF
[Desktop Entry]
Version=1.0
Name=picoloop Jack
Comment=Audio tracker
Exec=picoloop-jack
Icon=picoloop
Terminal=false
Type=Application
Categories=Audio;
EOF

cat > %{buildroot}/%{_datadir}/applications/picoloop-alsa.desktop <<EOF
[Desktop Entry]
Version=1.0
Name=picoloop ALSA
Comment=Audio tracker
Exec=picoloop-alsa
Icon=picoloop
Terminal=false
Type=Application
Categories=Audio;
EOF

cat > %{buildroot}/%{_datadir}/applications/picoloop-pulse.desktop <<EOF
[Desktop Entry]
Version=1.0
Name=picoloop PulseAudio
Comment=Audio tracker
Exec=picoloop-pulse
Icon=picoloop
Terminal=false
Type=Application
Categories=Audio;
EOF

%__install -m 755 -d %{buildroot}/%{_bindir}/
%__install -m 755 picoloop/picoloop-pulse %{buildroot}%{_bindir}/
%__install -m 755 picoloop/picoloop-alsa  %{buildroot}%{_bindir}/
%__install -m 755 picoloop/picoloop-jack  %{buildroot}%{_bindir}/

%__install -m 755 -d %{buildroot}/%{_datadir}/%{name}/patch/MDADrum/
%__install -m 644 picoloop/patch/MDADrum/create_patchlist.sh %{buildroot}%{_datadir}/%{name}/patch/MDADrum/
%__install -m 644 picoloop/patch/MDADrum/patchlist.cfg %{buildroot}%{_datadir}/%{name}/patch/MDADrum/

%__install -m 755 -d %{buildroot}/%{_datadir}/%{name}/patch/MDADrum/misc/
%__install -m 755 -d %{buildroot}/%{_datadir}/%{name}/patch/MDADrum/misc_claps/
%__install -m 755 -d %{buildroot}/%{_datadir}/%{name}/patch/MDADrum/misc_synth/
%__install -m 755 -d %{buildroot}/%{_datadir}/%{name}/patch/MDADrum/misc_electro/
%__install -m 755 -d %{buildroot}/%{_datadir}/%{name}/patch/MDADrum/misc_fx/
%__install -m 755 -d %{buildroot}/%{_datadir}/%{name}/patch/MDADrum/misc_bass/
%__install -m 755 -d %{buildroot}/%{_datadir}/%{name}/patch/MDADrum/misc_perc/
%__install -m 755 -d %{buildroot}/%{_datadir}/%{name}/patch/MDADrum/misc_hats/
%__install -m 755 -d %{buildroot}/%{_datadir}/%{name}/patch/MDADrum/Acoustic/
%__install -m 755 -d %{buildroot}/%{_datadir}/%{name}/patch/MDADrum/Electro/
%__install -m 755 -d %{buildroot}/%{_datadir}/%{name}/patch/MDADrum/Latin/
%__install -m 755 -d %{buildroot}/%{_datadir}/%{name}/patch/MDADrum/tr808/
%__install -m 755 -d %{buildroot}/%{_datadir}/%{name}/patch/MDADrum/cr78/
%__install -m 755 -d %{buildroot}/%{_datadir}/%{name}/patch/MDADrum/Farfisa/
%__install -m 755 -d %{buildroot}/%{_datadir}/%{name}/patch/MDADrum/Linn/
%__install -m 755 -d %{buildroot}/%{_datadir}/%{name}/patch/MDADrum/tr909/
%__install -m 755 -d %{buildroot}/%{_datadir}/%{name}/patch/MDADrum/cr8000/
%__install -m 755 -d %{buildroot}/%{_datadir}/%{name}/patch/MDADrum/Ferraro/
%__install -m 755 -d %{buildroot}/%{_datadir}/%{name}/patch/MDADrum/magnetboy/
%__install -m 755 -d %{buildroot}/%{_datadir}/%{name}/patch/MDADrum/R_B/
%__install -m 755 -d %{buildroot}/%{_datadir}/%{name}/patch/MDADrum/instrmnt/
%__install -m 755 -d %{buildroot}/%{_datadir}/%{name}/patch/MDADrum/tr606/
%__install -m 755 -d %{buildroot}/%{_datadir}/%{name}/patch/MDADrum/Effects/
%__install -m 755 -d %{buildroot}/%{_datadir}/%{name}/patch/MDADrum/JergenSohn/
%__install -m 755 -d %{buildroot}/%{_datadir}/%{name}/patch/MDADrum/tr77/

%__install -m 644 picoloop/patch/MDADrum/Acoustic/*     %{buildroot}%{_datadir}/%{name}/patch/MDADrum/Acoustic/
%__install -m 644 picoloop/patch/MDADrum/Electro/*      %{buildroot}%{_datadir}/%{name}/patch/MDADrum/Electro/
%__install -m 644 picoloop/patch/MDADrum/Latin/*        %{buildroot}%{_datadir}/%{name}/patch/MDADrum/Latin/
%__install -m 644 picoloop/patch/MDADrum/misc_claps/*   %{buildroot}%{_datadir}/%{name}/patch/MDADrum/misc_claps/
%__install -m 644 picoloop/patch/MDADrum/misc_synth/*   %{buildroot}%{_datadir}/%{name}/patch/MDADrum/misc_synth/
%__install -m 644 picoloop/patch/MDADrum/tr808/*        %{buildroot}%{_datadir}/%{name}/patch/MDADrum/tr808/
%__install -m 644 picoloop/patch/MDADrum/cr78/*         %{buildroot}%{_datadir}/%{name}/patch/MDADrum/cr78/
%__install -m 644 picoloop/patch/MDADrum/Farfisa/*      %{buildroot}%{_datadir}/%{name}/patch/MDADrum/Farfisa/
%__install -m 644 picoloop/patch/MDADrum/Linn/*         %{buildroot}%{_datadir}/%{name}/patch/MDADrum/Linn/
%__install -m 644 picoloop/patch/MDADrum/misc_electro/* %{buildroot}%{_datadir}/%{name}/patch/MDADrum/misc_electro/
%__install -m 644 picoloop/patch/MDADrum/tr909/*        %{buildroot}%{_datadir}/%{name}/patch/MDADrum/tr909/
%__install -m 644 picoloop/patch/MDADrum/cr8000/*       %{buildroot}%{_datadir}/%{name}/patch/MDADrum/cr8000/
%__install -m 644 picoloop/patch/MDADrum/Ferraro/*      %{buildroot}%{_datadir}/%{name}/patch/MDADrum/Ferraro/
%__install -m 644 picoloop/patch/MDADrum/magnetboy/*    %{buildroot}%{_datadir}/%{name}/patch/MDADrum/magnetboy/
%__install -m 644 picoloop/patch/MDADrum/misc_fx/*      %{buildroot}%{_datadir}/%{name}/patch/MDADrum/misc_fx/
%__install -m 644 picoloop/patch/MDADrum/R_B/*          %{buildroot}%{_datadir}/%{name}/patch/MDADrum/R_B/
%__install -m 644 picoloop/patch/MDADrum/instrmnt/*     %{buildroot}%{_datadir}/%{name}/patch/MDADrum/instrmnt/
%__install -m 644 picoloop/patch/MDADrum/misc/*         %{buildroot}%{_datadir}/%{name}/patch/MDADrum/misc/
%__install -m 644 picoloop/patch/MDADrum/misc_hats/*    %{buildroot}%{_datadir}/%{name}/patch/MDADrum/misc_hats/
%__install -m 644 picoloop/patch/MDADrum/tr606/*        %{buildroot}%{_datadir}/%{name}/patch/MDADrum/tr606/
%__install -m 644 picoloop/patch/MDADrum/Effects/*      %{buildroot}%{_datadir}/%{name}/patch/MDADrum/Effects/
%__install -m 644 picoloop/patch/MDADrum/JergenSohn/*   %{buildroot}%{_datadir}/%{name}/patch/MDADrum/JergenSohn/
%__install -m 644 picoloop/patch/MDADrum/misc_bass/*    %{buildroot}%{_datadir}/%{name}/patch/MDADrum/misc_bass/
%__install -m 644 picoloop/patch/MDADrum/misc_perc/*    %{buildroot}%{_datadir}/%{name}/patch/MDADrum/misc_perc/
%__install -m 644 picoloop/patch/MDADrum/tr77/*         %{buildroot}%{_datadir}/%{name}/patch/MDADrum/tr77/

%__install -m 755 -d %{buildroot}/%{_datadir}/%{name}/documentation/filter/
%__install -m 755 -d %{buildroot}/%{_datadir}/%{name}/documentation/gp2x/

%__install -m 644 picoloop/documentation/filter/* %{buildroot}%{_datadir}/%{name}/documentation/filter/
%__install -m 644 picoloop/documentation/gp2x/*   %{buildroot}%{_datadir}/%{name}/documentation/gp2x/
%__install -m 644 %{SOURCE2}                      %{buildroot}%{_datadir}/%{name}/documentation/

%__install -m 755 -d %{buildroot}/%{_datadir}/%{name}/font/
%__install -m 644 picoloop/font.ttf %{buildroot}%{_datadir}/%{name}/font/
%__install -m 755 -d %{buildroot}/%{_datadir}/%{name}/bmp/
%__install -m 644 picoloop/font.bmp %{buildroot}%{_datadir}/%{name}/bmp/

%__install -m 755 -d %{buildroot}/%{_datadir}/icons/
%__install -m 644 picoloop/picoloop-logo.png %{buildroot}%{_datadir}/icons/picoloop.png

%post 
update-desktop-database -q
touch --no-create %{_datadir}/icons/%{name} >&/dev/null || :

%postun
update-desktop-database -q
if [ $1 -eq 0 ]; then
  touch --no-create %{_datadir}/icons/%{name} >&/dev/null || :
  gtk-update-icon-cache %{_datadir}/icons/%{name} >&/dev/null || :
fi

%posttrans 
/usr/bin/gtk-update-icon-cache %{_datadir}/icons/%{name} &>/dev/null || :

%files
%{_bindir}/*
%{_datadir}/*

%changelog
* Thu Dec 19 2019 Yann Collette <ycollette.nospam@free.fr> - 0.77e-3
- update to 0.77e-3. Update to last master. This version builds ...

* Sun Sep 29 2019 Yann Collette <ycollette.nospam@free.fr> - 0.77e-2
- update to 0.77e-2

* Mon Oct 15 2018 Yann Collette <ycollette.nospam@free.fr> - 0.70d-2
- update for Fedora 29

* Sun May 13 2018 Yann Collette <ycollette.nospam@free.fr> - 0.70d-1
- update to 0.70d-1

* Sat Jun 06 2015 Yann Collette <ycollette.nospam@free.fr> - 0.67-1
- Initial build
