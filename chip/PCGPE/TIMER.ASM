;--------------------------------------------------------------------------
; Here is a source for measuring relative speed of computer. This source
; will NOT function properly under multitaskers, as they use timer them-
; selves. At least DesqView, OS/2 and Windows mess up with timer.
;
; This source is copyright 1994 of Teemu Peltonen aka Quark/Remedy Prods.
; If you modify this source, please do not spread it! You may spread the
; unmodified source as much as you like, but please check that length of
; the actual source (not including this comment between vertical lines) is
; 91 lines!
;
; The way of measuring computer's speed is not the best possible, but it
; works. Here's a small kind of table of speeds:
;
; CPU           Output value
; 486DX/33MHz   024Ch
; 486SX/20MHz   03D4h
; 386DX/40MHz   05EDh
; 386DX/16MHz   0DBAh
; 286  /10MHz   1500h
; 086  /4.77MHz 6006h
;
; Notice! Values may differ from these about 2 up or down. You should
; run the tester for about five times and use the value that seems to
; appear most times. Disk caches and others can mess up test.
;
; Thanks to my BBS's users for testing this piece of software!
;
; If you wish to add your computer's speed here, please contact author.
; Feel free to contact me, even if you wouldn't want to add your speed..
; Contact addresses are:
; teemu.peltonen@stream.nullnet.fi in Internet
; Teemu Peltonen@2:222/100         in Fidonet
; Teemu Peltonen@68:100/100        in DGi-net
; Or call Bitstream BBS (+358-21-4383244) and leave a comment to operator
; (command C).
;--------------------------------------------------------------------------
stacki  segment para stack use16 'stack'
        dw      100h dup (?)
stacki  ends

data    segment para public use16 'data'
hexval  db      '0123456789ABCDEF'
stringi db      'Relative speed of computer is '
d       db      0
c       db      0
b       db      0
a       db      0
endi    db      'h.$'
data    ends

code    segment para public use16 'code'
        assume cs:code, ds:data, ss:stacki
startup:
        mov     ax, seg data
        mov     ds, ax

        cli

        mov     al, 34h
        out     43h, al         ;OUT can be used as immediate
                                ;if register value under 100h

        xor     al, al
        out     40h, al
        out     40h, al

        sti

        mov     cx, 1000h       ;action here,
here:                           ;must not take
        dec     cx              ;more than 1/18
        jnz     here            ;(0.0555) seconds to run

        cli

        mov     al, 4h
        out     43h, al


        in      al, 40h
        mov     dl, al
        in      al, 40h
        mov     dh, al

        sti

        neg     dx              ;Action took bx/1193180 seconds

        mov     cl, dh
        and     cl, 11110000b
        shr     cl, 4
        xor     bx, bx
        mov     bl, cl
        mov     ah, hexval[bx]
        mov     d, ah

        mov     cl, dh
        and     cl, 00001111b
        xor     bx, bx
        mov     bl, cl
        mov     ah, hexval[bx]
        mov     c, ah

        mov     cl, dl
        and     cl, 11110000b
        shr     cl, 4
        xor     bx, bx
        mov     bl, cl
        mov     ah, hexval[bx]
        mov     b, ah

        mov     cl, dl
        and     cl, 00001111b
        xor     bx, bx
        mov     bl, cl
        mov     ah, hexval[bx]
        mov     a, ah


        mov     ax, 0900h
        mov     dx, offset stringi
        int     21h

        mov     ax, 4c00h
        int     21h

code    ends
        end     startup
