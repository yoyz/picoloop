{$R-}
{$X+}
Program T_holic;

USES
   Crt;

CONST
   Vga : Word = $a000;

   Block : Array[1..40,1..40] of Byte = (

       (0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0),
       (0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0),
       (0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0),
       (0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0),
       (0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0),
       (0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0),
       (0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0),
       (0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0),
       (0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0),
       (0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0),
       (1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1),
       (1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1),
       (1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1),
       (1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
       (1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
       (1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
       (1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
       (1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
       (1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
       (1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
       (1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
       (1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
       (1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
       (1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
       (1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
       (1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
       (1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
       (1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1),
       (1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1),
       (1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1),
       (0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0),
       (0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0),
       (0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0),
       (0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0),
       (0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0),
       (0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0),
       (0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0),
       (0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0),
       (0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0),
       (0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0)
       );


VAR
   WholePal : Array[1..256,1..3] of Byte;
   CurX,CurY,CurCol : Word;
   right,down:Boolean;


{컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴}
Procedure SetMCGA;  { This procedure gets you into 320x200x256 mode. }
BEGIN
  asm
     mov        ax,0013h
     int        10h
  end;
END;


{컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴}
Procedure SetText;  { This procedure returns you to text mode.  }
BEGIN
  asm
     mov        ax,0003h
     int        10h
  end;
END;


{컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴}
procedure WaitRetrace; assembler;
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


{컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴}
Procedure GetPal(ColorNo : Byte; Var R,G,B : Byte);
  { This reads the values of the Red, Green and Blue values of a certain
    color and returns them to you. }
Begin
   Port[$3c7] := ColorNo;
   R := Port[$3c9];
   G := Port[$3c9];
   B := Port[$3c9];
End;


{컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴}
Procedure Pal(ColorNo : Byte; R,G,B : Byte);
  { This sets the Red, Green and Blue values of a certain color }
Begin
   Port[$3c8] := ColorNo;
   Port[$3c9] := R;
   Port[$3c9] := G;
   Port[$3c9] := B;
End;


{컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴}
Procedure FadeDown;
  { This procedure fades the screen out to black. }
VAR loop1,loop2:integer;
    Tmp : Array [1..3] of byte;
      { This is temporary storage for the values of a color }
BEGIN
  For loop1:=1 to 64 do BEGIN
    WaitRetrace;
    For loop2:=1 to 255 do BEGIN
      Getpal (loop2,Tmp[1],Tmp[2],Tmp[3]);
      If Tmp[1]>0 then dec (Tmp[1]);
      If Tmp[2]>0 then dec (Tmp[2]);
      If Tmp[3]>0 then dec (Tmp[3]);
        { If the Red, Green or Blue values of color loop2 are not yet zero,
          then, decrease them by one. }
      Pal (loop2,Tmp[1],Tmp[2],Tmp[3]);
        { Set the new, altered pallette color. }
    END;
  END;
END;



{컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴}
Procedure Putpixel (X,Y : Integer; Col : Byte);
  { This puts a pixel on the screen by writing directly to memory. }
BEGIN
  Mem [VGA:X+(Y*320)]:=Col;
END;


{컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴}
Procedure CunninglyManipulatePalette;
   { This moves up the pallette by one so that the color of the block
     being put down is always the same }
Var
   Tmp : Array[1..3] of byte;
  loop : Byte;
Begin
   Move(WholePal[210],Tmp[1],3);           { Save Last Colour             }
   Move(WholePal[1],WholePal[2],209*3);    { Move Rest Up one             }
   Move(Tmp,WholePal[1],3);                { Put Last Colour to First pos }
   For Loop := 1 to 210 do
      Pal(Loop,WholePal[Loop,1],WholePal[Loop,2],WholePal[Loop,3]);
End;



{컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴}
Procedure PreparePalette;
   { This sets up the palette to have pretty gradients in it for our use }
Var
   Loop : Byte;
Begin
   For loop := 1 to 30 do BEGIN
      Wholepal [loop,1]:=loop*2;
      Wholepal [loop,2]:=0;
      Wholepal [loop,3]:=0;
   END;

   For loop := 31 to 60 do BEGIN
      Wholepal [loop,1]:=0;
      Wholepal [loop,2]:=loop*2-30;
      Wholepal [loop,3]:=0;
   END;


   For loop := 61 to 90 do BEGIN
      Wholepal [loop,1]:=0;
      Wholepal [loop,2]:=0;
      Wholepal [loop,3]:=loop*2-30;
   END;

   For loop := 91 to 120 do BEGIN
      Wholepal [loop,1]:=loop*2-30;
      Wholepal [loop,2]:=loop*2-30;
      Wholepal [loop,3]:=loop*2-30;
   END;

   For loop := 121 to 150 do BEGIN
      Wholepal [loop,1]:=loop*2-30;
      Wholepal [loop,2]:=loop*2-30;
      Wholepal [loop,3]:=0;
   END;

   For loop := 151 to 180 do BEGIN
      Wholepal [loop,1]:=0;
      Wholepal [loop,2]:=loop*2-30;
      Wholepal [loop,3]:=loop*2-30;
   END;

   For loop := 181 to 210 do BEGIN
      Wholepal [loop,1]:=loop*2-30;
      Wholepal [loop,2]:=0;
      Wholepal [loop,3]:=loop*2-30;
   END;
End;


{컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴}
Procedure IngeniouslyMoveCurPos;
   { This moves the position of the block to put down around the screen }
Begin
   CurCol := (CurCol) mod 210 + 1;        { This Does Work                }
   if right then CurX := CurX + 4 else CurX := CurX - 3;
   if down then CurY := CurY + 3 else CurY := CurY - 2;

   If CurX > 250 then right:= FALSE;
   If CurY > 150 then down := FALSE;

   If CurX < 10 then right := TRUE;
   If CurY < 10 then down  := TRUE;

End;


{컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴}
Procedure DrawBlock;
   { This draws the block onto the VGA screen }
Var
   Xloop,Yloop : Integer;
Begin
   For XLoop := 1 to 40 do
      For Yloop := 1 to 40 do
         If block[Yloop,Xloop] = 1 then
            PutPixel(CurX+Xloop,CurY+Yloop,CurCol);
End;


{컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴}
Procedure StartSnakiepoo;
   { This is the proc where we set things up & set em in motion! ;-) }
Begin
   CurX := 100;
   CurY := 100;
   CurCol := 1;
   PreparePalette;
   Repeat
      DrawBlock;
      CunninglyManipulatePalette;
      IngeniouslyMoveCurPos;
   Until Keypressed;
   fadedown;
   Readkey;
End;

Begin
   ClrScr;
   Writeln ('Hi there!  This is a small little routine that Livewire');
   Writeln ('and Denthor of ASPHYXIA threw together during lunch break');
   Writeln ('at varsity. We first saw this routine in the T-Holic demo');
   Writeln ('by Extreme a few months back, and decided to write it as');
   Writeln ('a supliment to the ASPHYXIA VGA Demo Trainer Series on the');
   Writeln ('MailBox BBS here in Durban.                              ');
   Writeln;
   Writeln ('The routine consists of a wormy type thing bouncing around');
   Writeln ('the screen, and looks quite effective. The code is');
   Writeln ('documented, and the concept behind it is so easy everyone');
   Writeln ('should be able to understand it.                         ');
   Writeln;
   Writeln ('The Pal routines, setmcga, waitretrace etc. are taken');
   Writeln ('directly from the ASPHYXIA Trainer Series, and you should');
   Writeln ('read those to understand how they work.');
   Writeln;
   Writeln ('See the Trainer Series for how to get into contact with us.');
   Writeln; Writeln;
   Writeln ('Hit any key to continue ....                             ');
   Readkey;
   SetMCGA;
   StartSnakiepoo;
   SetText;
   Writeln ('All done. This was a sample routine written by ASPHYXIA.');
   Writeln ('Please read the ASPHYXIA Demo Trainer Series on the MailBox BBS,');
   Writeln ('written by Denthor. You may reach DENTHOR under the name of GRANT');
   Writeln ('SMITH on the MailBox BBS, or leave a message to ASPHYXIA on the');
   Writeln ('ASPHYXIA BBS. Get the numbers from Roblist, or write to :');
   Writeln ('             Grant Smith');
   Writeln ('             P.O. Box 270');
   Writeln ('             Kloof');
   Writeln ('             3640');
   Writeln ('We hope to hear from you soon!');
   Writeln; Writeln;
   Write   ('Hit any key to exit ...');
   Readkey;
End.