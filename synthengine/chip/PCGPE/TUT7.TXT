                   ÕÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¸
                   ³         W E L C O M E         ³
                   ³  To the VGA Trainer Program   ³ ³
                   ³              By               ³ ³
                   ³      DENTHOR of ASPHYXIA      ³ ³ ³
                   ÔÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¾ ³ ³
                     ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ ³
                       ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

                           --==[ PART 7 ]==--



=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
ş Introduction

Hello! By popular request, this part is all about animation. I will be
going over three methods of doing animation on a PC, and will
concerntrate specifically on one, which will be demonstrated in the
attached sample code.

Although not often used in demo coding, animation is usually used in
games coding, which can be almost as rewarding ;-)

In this part I will also be a lot less stingy with assembler code :)
Included will be a fairly fast pure assembler putpixel, an asm screen
flip command, an asm icon placer, an asm partial-flip and one or two
others. I will be explaining how these work in detail, so this may also
be used as a bit of an asm-trainer too.

By the way, I apologise for this part taking so long to be released, but
I only finished my exams a few days ago, and they of course took
preference ;-). I have also noticed that the MailBox BBS is no longer
operational, so the trainer will be uploaded regularly to the BBS lists
shown at the end of this tutorial.

If you would like to contact me, or the team, there are many ways you
can do it : 1) Write a message to Grant Smith/Denthor/Asphyxia in private mail
                  on the ASPHYXIA BBS.
            2) Write a message in the Programming conference on the
                  For Your Eyes Only BBS (of which I am the Moderator )
                  This is preferred if you have a general programming query
                  or problem others would benefit from.
            4) Write to Denthor, Eze or Livewire on Connectix.
            5) Write to :  Grant Smith
                           P.O.Box 270 Kloof
                           3640
                           Natal
            6) Call me (Grant Smith) at (031) 73 2129 (leave a message if you
                  call during varsity)
            7) Write to mcphail@beastie.cs.und.ac.za on InterNet, and
                  mention the word Denthor near the top of the letter.

NB : If you are a representative of a company or BBS, and want ASPHYXIA
       to do you a demo, leave mail to me; we can discuss it.
NNB : If you have done/attempted a demo, SEND IT TO ME! We are feeling
        quite lonely and want to meet/help out/exchange code with other demo
        groups. What do you have to lose? Leave a message here and we can work
        out how to transfer it. We really want to hear from you!



=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
ş  The Principals of Animation

I am sure all of you have seen a computer game with animation at one or
other time. There are a few things that an animation sequence must do in
order to give an impression of realism. Firstly, it must move,
preferably using different frames to add to the realism (for example,
with a man walking you should have different frames with the arms an
legs in different positions). Secondly, it must not destroy the
background, but restore it after it has passed over it.

This sounds obvious enough, but can be very difficult to code when you
have no idea of how to go about achieving that.

In this trainer I will discuss various methods of meeting these two
objectives.



=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
ş  Frames and Object Control

It is quite obvious that for most animation to succeed, you must have
numerous frames of the object in various poses (such as a man with
several frames of him walking). When shown one after the other, these
give the impression of natural movement.

So, how do we store these frames? I hear you cry. Well, the obvious
method is to store them in arrays. After drawing a frame in Autodesk
Animator and saving it as a .CEL, we usually use the following code to
load it in :

TYPE icon = Array [1..50,1..50] of byte;

VAR tree : icon;

Procedure LoadCEL (FileName :  string; ScrPtr : pointer);
var
  Fil : file;
  Buf : array [1..1024] of byte;
  BlocksRead, Count : word;
begin
  assign (Fil, FileName);
  reset (Fil, 1);
  BlockRead (Fil, Buf, 800);    { Read and ignore the 800 byte header }
  Count := 0; BlocksRead := $FFFF;
  while (not eof (Fil)) and (BlocksRead <> 0) do begin
    BlockRead (Fil, mem [seg (ScrPtr^): ofs (ScrPtr^) + Count], 1024, BlocksRead);
    Count := Count + 1024;
  end;
  close (Fil);
end;

BEGIN
  Loadcel ('Tree.CEL',addr (tree));
END.

We now have the 50x50 picture of TREE.CEL in our array tree. We may access
this array in the usual manner (eg. col:=tree [25,30]). If the frame is
large, or if you have many frames, try using pointers (see previous
parts)

Now that we have the picture, how do we control the object? What if we
want multiple trees wandering around doing their own thing? The solution
is to have a record of information for each tree. A typical data
structure may look like the following :

TYPE Treeinfo = Record
                  x,y:word;       { Where the tree is }
                  speed:byte;     { How fast the tree is moving }
                  Direction:byte; { Where the tree is facing }
                  frame:byte      { Which animation frame the tree is
                                    currently involved in }
                  active:boolean; { Is the tree actually supposed to be
                                    shown/used? }
                END;

VAR Forest : Array [1..20] of Treeinfo;

You now have 20 trees, each with their own information, location etc.
These are accessed using the following means :
                  Forest [15].x:=100;
This would set the 15th tree's x coordinate to 100.



=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
ş  Restoring the Overwritten Background

I will discuss three methods of doing this. These are NOT NECESSARILY
THE ONLY OR BEST WAYS TO DO THIS! You must experiment and decide which
is the best for your particular type of program.

METHOD 1 :

Step 1 : Create two virtual pages, Vaddr and Vaddr2.
Step 2 : Draw the background to Vaddr2.
Step 3 : Flip Vaddr2 to Vaddr.
Step 4 : Draw all the foreground objects onto Vaddr.
Step 5 : Flip Vaddr to VGA.
Step 6 : Repeat from 3 continuously.

In ascii, it looks like follows ...

    +---------+           +---------+           +---------+
    |         |           |         |           |         |
    |  VGA    | <=======  |  VADDR  |  <======  |  VADDR2 |
    |         |           | (bckgnd)|           | (bckgnd)|
    |         |           |+(icons) |           |         |
    +---------+           +---------+           +---------+

The advantages of this approach is that it is straightforward, continual
reading of the background is not needed, there is no flicker and it is
simple to implement.  The disadvantages are that two 64000 byte virtual
screens are needed, and the procedure is not very fast because of the
slow speed of flipping.


METHOD 2 :

Step 1 : Draw background to VGA.
Step 2 : Grab portion of background that icon will be placed on.
Step 3 : Place icon.
Step 4 : Replace portion of background from Step 2 over icon.
Step 5 : Repeat from step 2 continuously.

In terms of ascii ...

      +---------+
      |      +--|------- + Background restored (3)
      |      * -|------> * Background saved to memory (1)
      |      ^  |
      |      +--|------- # Icon placed (2)
      +---------+

The advantages of this method is that very little extra memory is
needed. The disadvantages are that writing to VGA is slower then writing
to memory, and there may be large amounts of flicker.


METHOD 3 :

Step 1 : Set up one virtual screen, VADDR.
Step 2 : Draw background to VADDR.
Step 3 : Flip VADDR to VGA.
Step 4 : Draw icon to VGA.
Step 5 : Transfer background portion from VADDR to VGA.
Step 6 : Repeat from step 4 continuously.

In ascii ...

     +---------+           +---------+
     |         |           |         |
     |   VGA   |           |  VADDR  |
     |         |           | (bckgnd)|
     | Icon>* <|-----------|--+      |
     +---------+           +---------+

The advantages are that writing from the virtual screen is quicker then
from VGA, and there is less flicker then in Method 2. Disadvantages are
that you are using a 64000 byte virtual screen, and flickering occurs
with large numbers of objects.

In the attached sample program, a mixture of Method 3 and Method 1 is
used. It is faster then Method 1, and has no flicker, unlike Method 3.
What I do is I use VADDR2 for background, but only restore the
background that has been changed to VADDR, before flipping to VGA.

In the sample program, you will see that I restore the entire background
of each of the icons, and then place all the icons. This is because if I
replace the background then place the icon on each object individually,
if two objects are overlapping, one is partially overwritten.

The following sections are explanations of how the various assembler
routines work. This will probably be fairly boring for you if you
already know assembler, but should help beginners and dabblers alike.


=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
ş  The ASM Putpixel

To begin with, I will explain a few of the ASM variables and functions :

<NOTE THAT THIS IS AN EXTREMELY SIMPLISTIC VIEW OF ASSEMBLY LANGUAGE!
There are numerous books to advance your knowledge, and the Norton
Guides assembler guide  may be invaluable for people beginning to code
in assembler. I haven't given you the pretty pictures you are supposed
to have to help you understand it easier, I have merely laid it out like
a programming language with it's own special procedures. >

There are 4 register variables : AX,BX,CX,DX. These are words (double
bytes) with a range from 0 to 65535. You may access the high and low
bytes of these by replacing the X with a "H" for high or "L" for low.
For example, AL has a range from 0-255.

You also have two pointers : ES:DI and DS:SI. The part on the left is
the segment to which you are pointing (eg $a000), and the right hand
part is the offset, which is how far into the segment you are pointing.
Turbo Pascal places a variable over 16k into the base of a segment, ie.
DI or SI will be zero at the start of the variable.

If you wish to be pointing to pixel number 3000 on the VGA screen (see
previous parts for the layout of the VGA screen), ES would be equal to
$a000 and DI would be equal to 3000.  You can quite as easily make ES or
DS be equal to the offset of a virtual screen.

Here are a few functions that you will need to know :

      mov   destination,source       This moves the value in source to
                                     destination. eg  mov ax,50
      add   destination,source       This adds source to destination,
                                     the result being stored in destination
      mul   source                   This multiplies AX by source. If
                                     source is a byte, the source is
                                     multiplied by AL, the result being
                                     stored in AX. If source is a word,
                                     the source is multiplied by AX, the
                                     result being stored in DX:AX
      movsb                          This moves the byte that DS:SI is
                                     pointing to into ES:DI, and
                                     increments SI and DI.
      movsw                          Same as movsb except it moves a
                                     word instead of a byte.
      stosw                          This moves AX into ES:DI. stosb
                                     moves AL into ES:DI. DI is then
                                     incremented.
      push register                  This saves the value of register by
                                     pushing it onto the stack. The
                                     register may then be altered, but
                                     will be restored to it's original
                                     value when popped.
      pop register                   This restores the value of a pushed
                                     register. NOTE : Pushed values must
                                     be popped in the SAME ORDER but
                                     REVERSED.
      rep  command                   This repeats Command by as many
                                     times as the value in CX


SHL Destination,count      ;
and SHR Destination,count  ;
need a bit more explaining. As you know, computers think in ones and
zeroes. Each number may be represented in this base 2 operation. A byte
consists of 8 ones and zeroes (bits), and have a range from 0 to 255. A 
word consists of 16 ones and zeroes (bits), and has a range from 0 to 
65535. A double word consists of 32 bits.

The number 53 may be represented as follows :  00110101.  Ask someone who
looks clever to explain to you how to convert from binary to decimal and
vice-versa.

What happens if you shift everything to the left? Drop the leftmost
number and add a zero to the right? This is what happens :

                00110101     =  53
                 <-----
                01101010     =  106

As you can see, the value has doubled! In the same way, by shifting one
to the right, you halve the value! This is a VERY quick way of
multiplying or dividing by 2. (note that for dividing by shifting, we
get the trunc of the result ... ie.  15 shr 1 = 7)

In assembler the format is SHL destination,count    This shifts
destination by as many bits in count (1=*2, 2=*4, 3=*8, 4=*16 etc)
Note that a shift takes only 2 clock cycles, while a mul can take up to 133
clock cycles. Quite a difference, no? Only 286es or above may have count
being greater then one.

This is why to do the following to calculate the screen coordinates for
a putpixel is very slow :

           mov    ax,[Y]
           mov    bx,320
           mul    bx
           add    ax,[X]
           mov    di,ax

But alas! I hear you cry. 320 is not a value you may shift by, as you
may only shift by 2,4,8,16,32,64,128,256,512 etc.etc. The solution is
very cunning. Watch.

    mov     bx,[X]
    mov     dx,[Y]
    push    bx
    mov     bx, dx                  {; bx = dx = Y}
    mov     dh, dl                  {; dh = dl = Y}
    xor     dl, dl                  {; These 2 lines equal dx*256 }
    shl     bx, 1
    shl     bx, 1
    shl     bx, 1
    shl     bx, 1
    shl     bx, 1
    shl     bx, 1                   {; bx = bx * 64}
    add     dx, bx                  {; dx = dx + bx (ie y*320)}
    pop     bx                      {; get back our x}
    add     bx, dx                  {; finalise location}
    mov     di, bx

Let us have a look at this a bit closer shall we?
bx=dx=y        dx=dx*256  ;   bx=bx*64     ( Note, 256+64 = 320 )

dx+bx=Correct y value, just add X!

As you can see, in assembler, the shortest code is often not the
fastest.

The complete putpixel procedure is as follows :

Procedure Putpixel (X,Y : Integer; Col : Byte; where:word);
  { This puts a pixel on the screen by writing directly to memory. }
BEGIN
  Asm
    push    ds                      {; Make sure these two go out the }
    push    es                      {; same they went in }
    mov     ax,[where]
    mov     es,ax                   {; Point to segment of screen }
    mov     bx,[X]
    mov     dx,[Y]
    push    bx                      {; and this again for later}
    mov     bx, dx                  {; bx = dx}
    mov     dh, dl                  {; dx = dx * 256}
    xor     dl, dl
    shl     bx, 1
    shl     bx, 1
    shl     bx, 1
    shl     bx, 1
    shl     bx, 1
    shl     bx, 1                   {; bx = bx * 64}
    add     dx, bx                  {; dx = dx + bx (ie y*320)}
    pop     bx                      {; get back our x}
    add     bx, dx                  {; finalise location}
    mov     di, bx                  {; di = offset }
    {; es:di = where to go}
    xor     al,al
    mov     ah, [Col]
    mov     es:[di],ah              {; move the value in ah to screen
                                       point es:[di] }
    pop     es
    pop     ds
  End;
END;

Note that with DI and SI, when you use them :
      mov   di,50      Moves di to position 50
      mov   [di],50    Moves 50 into the place di is pointing to


=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
ş  The Flip Procedure

This is fairly straightforward. We get ES:DI to point to the start of
the destination screen, and DS:SI to point to the start of the source
screen, then do 32000 movsw (64000 bytes).

procedure flip(source,dest:Word);
  { This copies the entire screen at "source" to destination }
begin
  asm
    push    ds
    mov     ax, [Dest]
    mov     es, ax                  { ES = Segment of source }
    mov     ax, [Source]
    mov     ds, ax                  { DS = Segment of source }
    xor     si, si                  { SI = 0   Faster then mov si,0 }
    xor     di, di                  { DI = 0 }
    mov     cx, 32000
    rep     movsw                   { Repeat movsw 32000 times }
    pop     ds
  end;
end;

The cls procedure works in much the same way, only it moves the color
into AX then uses a rep stosw (see program for details)

The PAL command is almost exactly the same as it's Pascal equivalent
(see previous tutorials). Look in the sample code to see how it uses the
out and in commands.


=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
ş  In Closing

The assembler procedures presented to you in here are not at their best.
Most of these are procedures ASPHYXIA abandoned for better ones after
months of use. But, as you will soon see, they are all MUCH faster then
the original Pascal equivalents I originally gave you. In future, I
hope to give you more and more assembler procedures for your ever
growing collections. But, as you know, I am not always very prompt with
this series (I don't know if even one has been released within one week
of the previous one), so if you want to get any stuff done, try do it
yourself. What do you have to lose, aside from your temper and a few
rather inventive reboots ;-)

What should I do for the next trainer? A simple 3-d tutorial? You may
not like it, because I would go into minute detail of how it works :)
Leave me suggestions for future trainers by any of the means discussed
at the top of this trainer.

After the customary quote, I will place a listing of the BBSes I
currently know that regularly carry this Trainer Series. If your BBS
receives it regularly, no matter where in the country you are, get a
message to me and I'll add it to the list. Let's make it more convenient
for locals to grab a copy without calling long distance ;-)

            [  There they sit, the preschooler class encircling their
                  mentor, the substitute teacher.
               "Now class, today we will talk about what you want to be
                  when you grow up. Isn't that fun?" The teacher looks
                  around and spots the child, silent, apart from the others
                  and deep in thought. "Jonny, why don't you start?" she
                  encourages him.
               Jonny looks around, confused, his train of thought
                  disrupted. He collects himself, and stares at the teacher
                  with a steady eye. "I want to code demos," he says,
                  his words becoming stronger and more confidant as he
                  speaks. "I want to write something that will change
                  peoples perception of reality. I want them to walk
                  away from the computer dazed, unsure of their footing
                  and eyesight. I want to write something that will
                  reach out of the screen and grab them, making
                  heartbeats and breathing slow to almost a halt. I want
                  to write something that, when it is finished, they
                  are reluctant to leave, knowing that nothing they
                  experience that day will be quite as real, as
                  insightful, as good. I want to write demos."
               Silence. The class and the teacher stare at Jonny, stunned. It
                  is the teachers turn to be confused. Jonny blushes,
                  feeling that something more is required.  "Either that
                  or I want to be a fireman."
                                                                     ]
                                                       - Grant Smith
                                                            14:32
                                                               21/11/93

See you next time,
  - DENTHOR

These fine BBS's carry the ASPHYXIA DEMO TRAINER SERIES : (alphabetical)

ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍËÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍËÍÍÍÍÍËÍÍÍËÍÍÍÍËÍÍÍÍ»
ºBBS Name                  ºTelephone No.   ºOpen ºMsgºFileºPastº
ÌÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÎÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÎÍÍÍÍÍÎÍÍÍÎÍÍÍÍÎÍÍÍÍ¹
ºASPHYXIA BBS #1           º(031) 765-5312  ºALL  º * º *  º *  º
ºASPHYXIA BBS #2           º(031) 765-6293  ºALL  º * º *  º *  º
ºConnectix BBS             º(031) 266-9992  ºALL  º * º *  º *  º
ºFor Your Eyes Only BBS    º(031) 285-318   ºA/H  º * º *  º *  º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÊÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÊÍÍÍÍÍÊÍÍÍÊÍÍÍÍÊÍÍÍÍ¼

Open = Open at all times or only A/H
Msg  = Available in message base
File = Available in file base
Past = Previous Parts available


ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³ TUTPROG7.PAS ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

{$X+}
USES crt;

CONST VGA = $a000;

Type Toastinfo = Record                 { This is format of of each of our }
                 x,y:integer;              { records for the flying toasters }
                 speed,frame:integer;
                 active:boolean;
               END;

     icon = Array [1..30*48] of byte;  { This is the size of our pictures }

     Virtual = Array [1..64000] of byte;  { The size of our Virtual Screen }
     VirtPtr = ^Virtual;                  { Pointer to the virtual screen }

CONST frame1 : icon = (
0,0,0,0,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,5,5,5,5,
7,7,7,7,0,0,0,0,0,0,0,5,5,5,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,
5,7,7,7,7,7,7,7,8,8,7,7,7,7,7,7,0,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,
0,0,0,0,0,5,5,7,7,7,7,7,8,8,7,8,8,7,8,7,8,7,7,7,5,8,8,8,8,5,5,5,5,5,5,5,5,5,5,5,
5,0,0,0,0,0,0,0,0,0,0,0,5,7,7,7,7,7,7,8,7,7,7,8,7,7,7,7,7,7,0,0,0,0,0,0,8,5,5,5,
5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,5,7,7,8,8,7,7,8,7,7,8,7,7,7,7,7,0,0,0,0,0,
0,0,0,0,0,0,5,5,5,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,5,7,8,8,8,7,7,8,7,7,8,7,7,7,
7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,5,7,8,8,8,7,7,
8,8,8,8,8,8,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,9,9,9,
9,5,7,8,8,8,8,8,7,7,8,8,7,7,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,9,9,9,9,5,7,7,8,8,8,8,7,7,8,8,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
1,1,0,0,0,1,1,1,1,1,1,1,9,9,9,5,7,8,8,7,7,8,8,7,8,8,8,7,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,5,7,8,8,7,7,7,7,8,8,7,7,7,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,7,8,8,8,8,8,8,8,7,
7,7,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,7,
7,7,7,7,7,7,7,7,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,
1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,2,
2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,
1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,4,
4,6,6,6,6,6,6,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,1,1,1,1,1,1,1,1,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,3,3,1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,
9,9,9,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,9,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
);
      frame2 : icon = (
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,9,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,
1,1,0,0,0,1,1,1,1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,5,
5,5,5,5,5,5,5,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,
1,1,1,2,2,2,2,2,5,5,5,5,5,5,5,5,5,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,5,5,5,5,5,5,5,5,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,5,1,1,1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,2,2,5,5,5,5,5,5,5,5,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,5,5,1,1,1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,5,5,5,5,
5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,1,1,1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,2,
2,2,2,2,2,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,5,1,1,1,1,1,1,0,0,0,1,1,1,
1,1,1,2,2,2,2,2,2,2,2,2,2,2,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,5,1,7,1,4,
4,6,6,6,6,6,6,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,
0,0,0,5,5,1,1,1,1,1,1,1,1,1,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,5,5,5,5,5,5,5,0,0,
0,0,0,0,0,0,0,0,0,0,0,5,5,1,1,1,1,1,1,1,1,1,3,3,1,1,1,1,9,9,9,9,9,9,9,9,9,9,5,5,
5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,5,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,9,9,9,
9,9,9,9,9,9,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,9,9,9,9,9,9,9,9,9,9,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,
1,7,7,1,7,1,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,5,5,1,7,7,7,1,1,5,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,5,5,5,5,5,5,5,5,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,1,1,5,5,5,5,0,0,0,0,0,0,0,0,0,0,5,5,5,5,5,5,
5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,0,
0,0,5,5,5,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
);
      frame3 : icon = (
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,9,9,9,
9,9,9,9,9,9,9,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,9,9,9,9,9,9,9,9,9,5,5,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,7,1,1,1,1,1,
1,1,0,0,0,1,1,1,1,1,1,1,9,9,9,9,9,9,9,5,5,5,5,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,
0,7,1,1,7,7,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,5,5,5,1,7,7,7,7,5,5,5,5,5,5,
5,0,0,0,0,0,0,0,7,1,7,7,7,1,1,1,1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,5,5,1,1,1,7,7,
1,1,7,5,5,5,5,5,5,5,0,0,0,0,0,0,1,1,7,1,1,7,1,1,1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,2,
2,1,7,7,7,1,7,7,7,7,7,5,5,5,5,5,5,5,5,0,0,0,0,0,1,7,7,7,7,1,1,1,1,1,0,0,0,1,1,1,
1,1,1,2,2,2,2,2,2,1,7,7,7,7,7,7,7,1,1,5,5,5,5,5,5,5,5,5,0,0,0,0,7,7,1,7,1,7,1,1,
1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,2,1,1,1,1,1,1,2,2,5,5,5,5,5,5,5,5,5,5,5,0,0,0,
7,7,7,7,7,1,1,1,1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,5,5,5,5,5,5,
5,5,5,5,5,0,0,0,7,7,0,0,7,7,1,1,1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,5,5,0,0,5,5,0,5,5,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,2,
2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,
1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,4,
4,6,6,6,6,6,6,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,1,1,1,1,1,1,1,1,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,3,3,1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,
9,9,9,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,9,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
);


VAR Virscr : VirtPtr;                      { Our first Virtual screen }
    VirScr2 : VirtPtr;                     { Our second Virtual screen }
    Vaddr  : word;                      { The segment of our virtual screen}
    Vaddr2 : Word;                      { The segment of our 2nd virt. screen}
    ourpal : Array [0..255,1..3] of byte; { A virtual pallette }
    toaster : Array [1..10] of toastinfo; { The toaster info }

{ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ}
Procedure SetMCGA;  { This procedure gets you into 320x200x256 mode. }
BEGIN
  asm
     mov        ax,0013h
     int        10h
  end;
END;


{ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ}
Procedure SetText;  { This procedure returns you to text mode.  }
BEGIN
  asm
     mov        ax,0003h
     int        10h
  end;
END;

{ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ}
Procedure Cls (Col : Byte; Where:word);
   { This clears the screen to the specified color }
BEGIN
     asm
        push    es
        mov     cx, 32000;
        mov     es,[where]
        xor     di,di
        mov     al,[col]
        mov     ah,al
        rep     stosw
        pop     es
     End;
END;


{ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ}
Procedure Putpixel (X,Y : Integer; Col : Byte; where:word);
  { This puts a pixel on the screen by writing directly to memory. }
BEGIN
  Asm
    push    ds
    push    es
    mov     ax,[where]
    mov     es,ax
    mov     bx,[X]
    mov     dx,[Y]
    push    bx                      {; and this again for later}
    mov     bx, dx                  {; bx = dx}
    mov     dh, dl                  {; dx = dx * 256}
    xor     dl, dl
    shl     bx, 1
    shl     bx, 1
    shl     bx, 1
    shl     bx, 1
    shl     bx, 1
    shl     bx, 1                   {; bx = bx * 64}
    add     dx, bx                  {; dx = dx + bx (ie y*320)}
    pop     bx                      {; get back our x}
    add     bx, dx                  {; finalise location}
    mov     di, bx
    {; es:di = where to go}
    xor     al,al
    mov     ah, [Col]
    mov     es:[di],ah
    pop     es
    pop     ds
  End;
END;


{ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ}
procedure WaitRetrace; assembler;
  {  This waits for a vertical retrace to reduce snow on the screen }
label
  l1, l2;
asm
    mov dx,3DAh
l1:
    in al,dx
    and al,08h
    jnz l1
l2:
    in al,dx
    and al,08h
    jz  l2
end;


{ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ}
Procedure Pal(Col,R,G,B : Byte);
  { This sets the Red, Green and Blue values of a certain color }
Begin
   asm
      mov    dx,3c8h
      mov    al,[col]
      out    dx,al
      inc    dx
      mov    al,[r]
      out    dx,al
      mov    al,[g]
      out    dx,al
      mov    al,[b]
      out    dx,al
   end;
End;

{ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ}
Procedure GetPal(Col : Byte; Var R,G,B : Byte);
  { This gets the Red, Green and Blue values of a certain color }
Var
   rr,gg,bb : Byte;
Begin
   asm
      mov    dx,3c7h
      mov    al,col
      out    dx,al

      add    dx,2

      in     al,dx
      mov    [rr],al
      in     al,dx
      mov    [gg],al
      in     al,dx
      mov    [bb],al
   end;
   r := rr;
   g := gg;
   b := bb;
end;

{ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ}
Procedure SetUpVirtual;
   { This sets up the memory needed for the virtual screen }
BEGIN
  GetMem (VirScr,64000);
  vaddr := seg (virscr^);
  GetMem (VirScr2,64000);
  vaddr2 := seg (virscr2^);
END;


{ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ}
Procedure ShutDown;
   { This frees the memory used by the virtual screen }
BEGIN
  FreeMem (VirScr,64000);
  FreeMem (VirScr2,64000);
END;


{ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ}
procedure flip(source,dest:Word);
  { This copies the entire screen at "source" to destination }
begin
  asm
    push    ds
    mov     ax, [Dest]
    mov     es, ax
    mov     ax, [Source]
    mov     ds, ax
    xor     si, si
    xor     di, di
    mov     cx, 32000
    rep     movsw
    pop     ds
  end;
end;


{ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ}
Procedure putico(X,Y:Word;VAR sprt : icon;Where:Word); ASSEMBLER;
  { This puts an icon, EXCEPT it's color 0 (black) pixels, onto the screen
    "where", at position X,Y }
label
  _Redraw, _DrawLoop, _Exit, _LineLoop, _NextLine, _Store, _NoPaint;

asm
    push  ds
    push  es
    lds   si,Sprt
    mov   ax,X     { ax = x }
    mov   bx,Y     { bx = y }
_Redraw:
    push    ax
    mov     ax,[where]
    mov     es,ax

    mov     ax, bx                  {; ax = bx  x = y}
    mov     bh, bl                  {; y = y * 256  bx = bx * 256}
    xor     bl, bl
    shl     ax, 1
    shl     ax, 1
    shl     ax, 1
    shl     ax, 1
    shl     ax, 1
    shl     ax, 1                   {; y = y * 64   ax = ax * 64}
    add     bx, ax                  {; y = (y*256) + (Y*64)  bx = bx + ax (ie y*320)}

    pop     ax                      {; get back our x}


    add     ax, bx                  {; finalise location}
    mov     di, ax

    mov   dl,30    { dl = height of sprite }
    xor   ch,ch
    mov   cl,48     { cx = width of sprite }
    cld
    push  ax
    mov   ax,cx
_DrawLoop:
    push  di            { store y adr. for later }
    mov   cx,ax          { store width }
_LineLoop:
    mov   bl,byte ptr [si]
    or    bl,bl
    jnz   _Store
_NoPaint:
    inc    si
    inc    di
    loop   _LineLoop
    jmp    _NextLine
_Store:
    movsb
    loop  _LineLoop
_NextLine:
    pop   di
    dec   dl
    jz    _Exit
    add   di,320        { di = next line of sprite }
    jmp   _DrawLoop
_Exit:
    pop   ax
    pop   es
    pop   ds
end;





{ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ}
Procedure Funny_line(a,b,c,d:integer;where:word);
  { This procedure draws a line from a,b to c,d on screen "where". After
    each pixel it plots, it increments a color counter for the next pixel.
    you may easily alter this to be a normal line procedure, and it will
    be quite a bit faster than the origional one I gave you. This is
    because I replaced all the reals with integers. }

  function sgn(a:real):integer;
  begin
       if a>0 then sgn:=+1;
       if a<0 then sgn:=-1;
       if a=0 then sgn:=0;
  end;
var i,s,d1x,d1y,d2x,d2y,u,v,m,n:integer;
    count:integer;
begin
     count:=50;
     u:= c - a;
     v:= d - b;
     d1x:= SGN(u);
     d1y:= SGN(v);
     d2x:= SGN(u);
     d2y:= 0;
     m:= ABS(u);
     n := ABS(v);
     IF NOT (M>N) then
     BEGIN
          d2x := 0 ;
          d2y := SGN(v);
          m := ABS(v);
          n := ABS(u);
     END;
     s := m shr 1;
     FOR i := 0 TO m DO
     BEGIN
          putpixel(a,b,count,where);
          inc (count);
          if count=101 then count:=50;
          s := s + n;
          IF not (s<m) THEN
          BEGIN
               s := s - m;
               a:= a + d1x;
               b := b + d1y;
          END
          ELSE
          BEGIN
               a := a + d2x;
               b := b + d2y;
          END;
     end;
END;



{ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ}
Procedure SetUpScreen;
  { This procedure sets up the static background to be used in the program }

CONST circ : Array [1..5,1..5] of byte =
        ((0,10,10,10,0),
         (10,13,12,11,10),
         (10,12,12,11,10),
         (10,11,11,11,10),
         (0,10,10,10,0));

VAR x,y:integer;
    loop1,loop2,loop3:integer;

BEGIN
  pal (1,22,22,22);
  pal (2,45,45,45);
  pal (3,59,59,59);
  pal (4,63,63,27);
  pal (5,39,63,3);
  pal (6,51,39,3);
  pal (7,3,27,3);
  pal (8,15,39,15);
  pal (9,35,35,35);
  pal (10, 0, 0,40);
  pal (11,10,10,50);
  pal (12,20,20,60);
  pal (13,30,30,63);

  For loop1:=50 to 100 do
    pal (loop1,0,0,loop1-36);

  For loop1:=0 to 255 do
     getpal (loop1,OurPal[loop1,1],OurPal[loop1,2],OurPal[loop1,3]);

  For loop1:=0 to 319 do
    Funny_line (0,199,loop1,0,vaddr);
  For loop1:=0 to 199 do
    Funny_line (0,199,319,loop1,vaddr);

  For loop1:=1 to 200 do BEGIN
    x:=random (315);
    y:=random (195);
    For loop2:=1 to 5 do
      For loop3:=1 to 5 do
        if circ [loop2,loop3]<>0 then
          putpixel (x+loop2,y+loop3,circ [loop2,loop3],vaddr);
  END;
  flip (vaddr,vga);  { Copy the entire screen at vaddr, our virtual screen }
                     { on which we have done all our graphics, onto the    }
                     { screen you see, VGA }
  flip (vaddr,vaddr2);
END;


{ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ}
Procedure rotatepal;
  { This procedure rotates the colors between 50 and 100 }
VAR temp : Array [1..3] of byte;
    loop1:integer;
BEGIN
  Move(OurPal[100],Temp,3);
  Move(OurPal[50],OurPal[51],50*3);
  Move(Temp,OurPal[50],3);
  For loop1:=50 to 100 do
    pal (loop1,OurPal[loop1,1],OurPal[loop1,2],OurPal[loop1,3]);
END;


{ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ}
Procedure ScreenTrans (x,y:word);
  { This is a small procedure to copy a 30x30 pixel block from coordinates
    x,y on the virtual screen to coordinates x,y on the true vga screen }
BEGIN
  asm
    push    ds
    push    es
    mov     ax,vaddr
    mov     es,ax
    mov     ax,vaddr2
    mov     ds,ax
    mov     bx,[X]
    mov     dx,[Y]
    push    bx                      {; and this again for later}
    mov     bx, dx                  {; bx = dx}
    mov     dh, dl                  {; dx = dx * 256}
    xor     dl, dl
    shl     bx, 1
    shl     bx, 1
    shl     bx, 1
    shl     bx, 1
    shl     bx, 1
    shl     bx, 1                   {; bx = bx * 64}
    add     dx, bx                  {; dx = dx + bx (ie y*320)}
    pop     bx                      {; get back our x}
    add     bx, dx                  {; finalise location}
    mov     di, bx                  {; es:di = where to go}
    mov     si, di
    mov     al,60
    mov     bx, 30         { Hight of block to copy }
@@1 :
    mov     cx, 24         { Width of block to copy divided by 2 }
    rep     movsw
    add     di,110h        { 320 - 48 = 272 .. or 110 in hex }
    add     si,110h
    dec     bx
    jnz     @@1

    pop     es
    pop     ds
  end;
  { I wrote this procedure late last night, so it may not be in it's
    most optimised state. Sorry :-)}
END;


{ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ}
Procedure NewToaster;
  { This adds a new toaster to the screen }
VAR loop1:integer;
BEGIN
  loop1:=0;
  repeat
    inc (loop1);
    if not (toaster[loop1].active) then BEGIN
      toaster[loop1].x:=random (200)+70;
      toaster[loop1].y:=0;
      toaster[loop1].active:=true;
      toaster[loop1].frame:=1;
      toaster[loop1].speed:=Random (3)+1;
      loop1:=10;
    END;
  until loop1=10;
END;


{ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ}
Procedure Fly;
  { This is the procedure where we move and put the toasters }
VAR loop1,loop2:integer;
    ch:char;
BEGIN
  For loop1:=1 to 10 do
    toaster[loop1].active:=FALSE;
  ch:=#0;
  NewToaster;
  Repeat
    if keypressed then BEGIN
      ch:=readkey;
      if ch='+' then NewToaster;      { If '+' is pressed, add a toaster }
      if ch='-' then BEGIN            { if '-' is pressed, remove a toaster }
        loop1:=0;
        repeat
          inc (loop1);
          if toaster[loop1].active then BEGIN
            screentrans (toaster[loop1].x,toaster[loop1].y);
            toaster [loop1].active:=FALSE;
            loop1:=10;
          END;
        until loop1=10;
      END;
    END;
    for loop1:=1 to 10 do
      if toaster[loop1].active then BEGIN
        screentrans (toaster[loop1].x,toaster[loop1].y);
          { Restore the backgrond the toaster was over }
        dec (toaster[loop1].x,toaster[loop1].speed);
        inc (toaster[loop1].y,toaster[loop1].speed);
          { Move the toaster }
        if (toaster[loop1].x<1) or (toaster[loop1].y>170) then BEGIN
          toaster[loop1].active:=FALSE;
          NewToaster;
        END;
          { When toaster reaches the edge of the screen, render it inactive
            and bring a new one into existance. }
      END;
    for loop1:=1 to 10 do
      if toaster[loop1].active then BEGIN
        CASE toaster [loop1].frame of
           1   : putico (toaster[loop1].x,toaster[loop1].y,frame1,vaddr);
           3   : putico (toaster[loop1].x,toaster[loop1].y,frame2,vaddr);
           2,4 : putico (toaster[loop1].x,toaster[loop1].y,frame3,vaddr);
        END;
        toaster[loop1].frame:=toaster[loop1].frame+1;
        if toaster [loop1].frame=5 then toaster[loop1].frame:=1;
          { Draw all the toasters on the VGA screen }
      END;
    waitretrace;
    flip (vaddr,vga);
    rotatepal;
  Until ch=#27;
END;


BEGIN
  Randomize;       { Make sure that the RANDOM funcion really is random }
  SetupVirtual;    { Set up virtual page, VADDR }
  ClrScr;
  writeln ('Hello! This program will demonstrate the principals of animation.');
  writeln ('The program will firstly generate an arb background screen to a');
  writeln ('virtual page, then flip it to the VGA. A toaster will then start');
  writeln ('to move across the screen. Note that the background will be restored');
  writeln ('after the toaster has passed over it. You may add or remove toasters');
  writeln ('by hitting "+" or "-" respectively. Note that the more frames you');
  writeln ('use, usually the better the routine looks. Because of space');
  writeln ('restrictions, we only had room for three frames.');
  writeln;
  writeln ('The toasters were drawn by Fubar (Pieter Buys) in Autodesk Animator.');
  writeln ('I wrote a small little program to convert them into CONSTANTS. See');
  writeln ('the main text to find out how to load up AA CEL files directly.');
  writeln;
  writeln;
  Write ('  Hit any key to contine ...');
  Readkey;
  SetMCGA;
  SetupScreen;     { Draw the background screen to VADDR, then flip it to
                     the VGA screen }
  Fly;             { Make the toasters fly around the screen }
  SetText;
  ShutDown;        { Free the memory taken up by virtual page }
  Writeln ('All done. This concludes the seventh sample program in the ASPHYXIA');
  Writeln ('Training series. You may reach DENTHOR under the names of GRANT');
  Writeln ('SMITH/DENTHOR/ASPHYXIA on the ASPHYXIA BBS. I am also an avid');
  Writeln ('Connectix BBS user, which is unfortunatly offline for the moment.');
  Writeln ('For discussion purposes, I am also the moderator of the Programming');
  Writeln ('newsgroup on the For Your Eyes Only BBS.');
  Writeln ('The numbers are available in the main text. You may also write to me at:');
  Writeln ('             Grant Smith');
  Writeln ('             P.O. Box 270');
  Writeln ('             Kloof');
  Writeln ('             3640');
  Writeln ('I hope to hear from you soon!');
  Writeln; Writeln;
  Write   ('Hit any key to exit ...');
  Readkey;
END.
