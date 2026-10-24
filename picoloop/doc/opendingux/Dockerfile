# Build environment for the GCW Zero / OpenDingux target.
#
# The toolchain is the 2014-08-20 GCW Zero one (Buildroot 2014.05, GCC 4.9.1,
# mipsel-gcw0-linux-uclibc, SDL 1.2 + SDL_ttf + SDL_gfx in its sysroot). Its
# original download URL, http://www.gcw-zero.com/files/
# opendingux-gcw0-toolchain.2014-08-20.tar.bz2, is dead - the host now answers
# with its homepage - so the archived copy is used and pinned by SHA-256: a
# build either gets exactly those bytes or fails.
#
# The 2014 binaries are 32-bit i386 and dynamically linked, hence the i386
# loader/runtime (the historical reason for gcc-multilib in older builders).
#
# Build:  podman build -t picoloop-od -f doc/opendingux/Dockerfile doc/opendingux
# Use:    podman run --rm --userns=keep-id -v "$PWD":/src:z -w /src picoloop-od \
#             make -f Makefile.PatternPlayer_opendingux_SDL -j"$(nproc)"
FROM docker.io/library/ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN dpkg --add-architecture i386 \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
        bzip2 \
        ca-certificates \
        curl \
        file \
        libc6:i386 \
        libstdc++6:i386 \
        libgcc-s1:i386 \
        make \
    && rm -rf /var/lib/apt/lists/*

ARG TOOLCHAIN_SHA256=3632c85f48879108d4349570ded60d87d7a324850b81d683d031e4ee112baaa0

RUN set -eu; \
    url="https://web.archive.org/web/20160310002829id_/http://www.gcw-zero.com/files/opendingux-gcw0-toolchain.2014-08-20.tar.bz2"; \
    curl -fsSL --retry 3 -o /tmp/toolchain.tar.bz2 "$url"; \
    echo "${TOOLCHAIN_SHA256}  /tmp/toolchain.tar.bz2" | sha256sum -c -; \
    mkdir -p /opt/opendingux-toolchain; \
    tar xjf /tmp/toolchain.tar.bz2 -C /opt/opendingux-toolchain --strip-components=1; \
    rm -f /tmp/toolchain.tar.bz2; \
    ln -s /opt/opendingux-toolchain /opt/gcw0-toolchain

# /opt/opendingux-toolchain is what the opendingux_* makefiles expect;
# /opt/gcw0-toolchain is the alias Makefile.PatternPlayer_gcwzero_RtAudio wants.
ENV PATH=/opt/opendingux-toolchain/usr/bin:/opt/opendingux-toolchain/usr/mipsel-gcw0-linux-uclibc/sysroot/usr/bin:$PATH
ENV CC=mipsel-gcw0-linux-uclibc-gcc

WORKDIR /src

CMD ["/bin/bash"]
