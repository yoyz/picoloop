# Build container for the RG35XX SP (Anbernic H700 / KNULLI) picoloop port.
#
# Purpose: build and link picoloop for aarch64 against a glibc OLDER than the
# device's, so nothing has to be imported from the handheld - no headers, no
# .so files, no device glibc, no device attached.
#
#   device (KNULLI, Buildroot)  glibc 2.40   SDL2 2.32.8   SDL2_ttf 2.24.0
#   this image (ubuntu 24.04)   glibc 2.39   SDL2 2.30.0   SDL2_ttf 2.22.0
#
# Validated 2026-10-24 with podman 5.7.0: the image builds, picoloop compiles
# and links, the resulting binary needs at most GLIBC_2.38 (device: 2.40) with
# no rpath, and it runs when launched from the Ports menu on the handheld.
#
# Compiling against *older* headers/libraries is the safe direction: every
# symbol the binary references exists on the device. The reverse fails at
# startup, e.g.
#
#   ./picoloop: /lib64/libm.so.6: version `GLIBC_2.43' not found
#
# which is what happens when the host's own cross toolchain (glibc 2.43 on the
# development machine used for this port) is used directly.
#
# Do NOT move this base to a newer release without checking its glibc:
# Debian 13 / Ubuntu 25.10 already ship glibc >= 2.41 and would reintroduce
# the problem. Debian 12 (glibc 2.36) is the safest base if binaries are to be
# distributed to older KNULLI images.
#
# WHY THE -dev PACKAGES ARE UNPACKED BY HAND
# Ubuntu's arm64 libsdl2-dev hard-depends on libibus-1.0-dev -> libglib2.0-dev
# -> python3, so `apt-get install libsdl2-dev:arm64` tries to install and
# *configure* python3.12-minimal:arm64, whose postinst is an arm64 binary:
#
#   /var/lib/dpkg/info/python3.12-minimal.postinst: 51: /usr/bin/python3.12:
#   Exec format error
#
# That only works with qemu/binfmt emulation registered on the host. Since we
# only want the headers and the unversioned .so symlinks (never a canned
# build of anything), the runtime libraries are installed normally and the
# -dev .debs are simply unpacked with dpkg-deb -x, which runs no postinst.
#
# Build (podman works the same way):
#   docker build -t picoloop-sp -f doc/rg35xxsp/Dockerfile doc/rg35xxsp
#
# For a fully pinned build, pin the base image by digest:
#   FROM docker.io/library/ubuntu:24.04@sha256:<digest>
#
# The base is fully qualified (docker.io/...) so that podman, which does not
# expand short names by default, builds it as-is too.
#
# Use (this is the DEFAULT picoloop build for the SP - the makefile needs no
# overrides: SDL2/SDL2_ttf/alsa come from the image's arm64 multiarch packages,
# which the cross compiler finds on its default search paths):
#   docker run --rm -u "$(id -u):$(id -g)" -v "$PWD":/src -w /src picoloop-sp \
#       make -f Makefile.PatternPlayer_rg35xxsp -j"$(nproc)"
#
# Rootless podman: replace `-u "$(id -u):$(id -g)"` with `--userns=keep-id`
# (otherwise container uids map to subuids and the bind mount is not writable):
#   podman run --rm --userns=keep-id -v "$PWD":/src:z -w /src picoloop-sp \
#       make -f Makefile.PatternPlayer_rg35xxsp -j"$(nproc)"
#
# The device-matched alternative (SDL/glibc at the device's exact versions,
# built on the host by platform/rg35xxsp/build_sdk.sh) is opt-in:
#   make -f Makefile.PatternPlayer_rg35xxsp USE_DEVICE_LIBS=1 \
#        CC=aarch64-linux-gnu-g++-14 -j"$(nproc)"
#
# Verify the result (must be <= the device glibc, 2.40):
#   aarch64-linux-gnu-objdump -T picoloop_rg35xxsp \
#       | grep -o 'GLIBC_2\.[0-9]*' | sort -V -u | tail -1

FROM docker.io/library/ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

# arm64 packages come from ports.ubuntu.com, which only carries non-amd64
# architectures; the default archive stays amd64-only, hence the explicit
# Architectures: lines in both source lists.
RUN set -eux; \
    dpkg --add-architecture arm64; \
    sed -i 's/^Types: deb$/Types: deb\nArchitectures: amd64/' \
        /etc/apt/sources.list.d/ubuntu.sources; \
    printf '%s\n' \
        'Types: deb' \
        'URIs: http://ports.ubuntu.com/ubuntu-ports/' \
        'Suites: noble noble-updates noble-security' \
        'Components: main restricted universe multiverse' \
        'Architectures: arm64' \
        'Signed-By: /usr/share/keyrings/ubuntu-archive-keyring.gpg' \
        > /etc/apt/sources.list.d/ubuntu-ports.sources; \
    apt-get update; \
    apt-get install -y --no-install-recommends \
        make \
        g++-aarch64-linux-gnu \
        libsdl2-2.0-0:arm64 \
        libsdl2-ttf-2.0-0:arm64 \
        libasound2t64:arm64; \
    mkdir -p /tmp/dev; \
    cd /tmp/dev; \
    apt-get download libsdl2-dev:arm64 libsdl2-ttf-dev:arm64 libasound2-dev:arm64; \
    for deb in ./*.deb; do dpkg-deb -x "$deb" /; done; \
    cd /; \
    rm -rf /tmp/dev /var/lib/apt/lists/*

WORKDIR /src
