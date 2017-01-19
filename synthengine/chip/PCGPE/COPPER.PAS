{$X+}
Program Copper;
Uses Crt;



Type
   ColType = Record
                R,
                G,
                B : Byte;
             End;

   PalType = Array[0..255] of ColType;

   BarType = Record
                Col : Array[1..20] of ColType;
                Pos : Array[1..20] of Byte;
                 UP : Array[1..20] of Boolean;
             End;



Var
   Pal1 : PalType;
   Bars : Array[1..40] Of BarType;
   NumBars, NumLines : Byte;


Procedure Pal(Col, R, G, B : Byte);
Begin
   Asm
      mov   dx, 3c8h
      mov   al, [Col]
      out   dx, al
      inc   dx
      mov   al, [R]
      out   dx, al
      mov   al, [G]
      out   dx, al
      mov   al, [B]
      out   dx, al
   End;
End;

Procedure GetPal(Col : Byte; Var R, G, B : Byte);
Var
   Rt,Gt,Bt : Byte;
Begin
   Asm
      mov   dx, 3c7h
      mov   al, [Col]
      out   dx, al
      inc   dx
      inc   dx
      in    al, dx
      mov   [Rt],al
      in    al, dx
      mov   [Gt],al
      in    al, dx
      mov   [Bt],al
   End;
   R := Rt;
   G := Gt;
   B := Bt;
End;



Procedure WaitRetrace; Assembler;
Asm
    mov   dx,3DAh
@@1:
    in    al,dx
    and   al,08h
    jnz   @@1
@@2:
    in    al,dx
    and   al,08h
    jz    @@2
End;


Procedure SetPal(Var Palet : PalType); Assembler;
Asm
   call  WaitRetrace
   push  ds
   lds   si, Palet
   mov   dx, 3c8h
   mov   al, 0
   out   dx, al
   inc   dx
   mov   cx, 768
   rep   outsb
   pop   ds
End;


Procedure FadeOut(NoBars, BarSize : Byte);
Var
      F, L : Integer;
   PalFade : PalType;

Begin
   For F := 1 to NoBars do
      For L := 1 to BarSize do
      Begin
         If Bars[F].Col[L].R > 0 Then Dec(Bars[F].Col[L].R);
         If Bars[F].Col[L].G > 0 Then Dec(Bars[F].Col[L].G);
         If Bars[F].Col[L].B > 0 Then Dec(Bars[F].Col[L].B);
      End;
End;



Procedure SetMcga;
Begin
   Asm
      mov   ax, 0013h
      int   10h
   End;
End;

Procedure SetText;
Begin
   Asm
      mov   ax, 0003h
      int   10h
   End;
End;



Procedure DrawCopper(NoLines,  StartCol, YStart : Byte);
Var
   Loop : Word;
Begin
   For Loop := YStart to YStart + NoLines do
   Begin
      FillChar(Mem[$a000:Loop*320],320,StartCol+Loop-YStart);
   End;
End;


Procedure SetCopperPal(NoBars, BarSize, YStart, ColStart, Space : Byte);
Var
      Loop : Byte;
     Loop2 : Word;
      IncR : Byte;
       RGB : Byte;
   HalfBar : Byte;

Begin
   FillChar(Bars, SizeOf (Bars),0);
   HalfBar := BarSize Div 2;
   IncR := 63 Div HalfBar;
   RGB := 0;
   For Loop := 1 to NoBars do
   Begin
      For Loop2 := 1 to HalfBar do
      Begin
         If RGB = 0 Then
         Bars[Loop].Col[Loop2].R := Loop2 * IncR;
         If RGB = 1 Then
         Bars[Loop].Col[Loop2].G := Loop2 * IncR;
         If RGB = 2 Then
         Bars[Loop].Col[Loop2].B := Loop2 * IncR;

         Bars[Loop].Pos[Loop2] := YStart + (Loop-1) * (BarSize+Space) + Loop2 -1 + ColStart;
         Bars[Loop].UP[Loop2] := True
      End;

      For Loop2 := HalfBar + 1  to BarSize  do
      Begin
         If RGB = 0 Then
         Bars[Loop].Col[Loop2].R := (BarSize - Loop2) * IncR;
         If RGB = 1 Then
         Bars[Loop].Col[Loop2].G := (BarSize - Loop2) * IncR;
         If RGB = 2 Then
         Bars[Loop].Col[Loop2].B := (BarSize - Loop2) * IncR;

         Bars[Loop].Pos[Loop2] := YStart + (Loop-1) * (BarSize+Space) + Loop2 -1 + ColStart;
         Bars[Loop].UP[Loop2] := True
      End;

      RGB := (RGB + 1) Mod 3;
   End;

End;




Procedure RotatePal(NoBars, BarSize, YStart, StartCol, NumLines : Byte;
                    Up : Boolean);

Var
   TPal : PalType;
   TCol : ColType;
   Loop,
   Loop2 : Byte;

Begin
   FillChar(TPal, 768, 0);
   For Loop := 1 to NoBars do
   Begin
      For Loop2 := 1 to BarSize do
      Begin
         TPal[Bars[Loop].Pos[Loop2]] := Bars[Loop].Col[Loop2];
         If Up Then
         Begin
            If Bars[Loop].Pos[Loop2] = StartCol Then
            Bars[Loop].UP[Loop2] := False;
            If Bars[Loop].Pos[Loop2] = NumLines Then
            Bars[Loop].UP[Loop2] := True;

            If Bars[Loop].UP[Loop2] Then
            Dec(Bars[Loop].Pos[Loop2])
            Else
            Inc(Bars[Loop].Pos[Loop2]);

         End;
      End;

   End;
   SetPal(TPal);

End;


Procedure SetUP(NumLines, NumBars, BarSize, YStart, ColStart, Space : Byte);
Begin
   SetMcga;
   DrawCopper(NumLines,ColStart,YStart);
   SetCopperPal(NumBars, BarSize, YStart, ColStart, Space);
End;


Procedure DoItAll;
Var
   NumLines,
   NumBars,
   BarSize,
   YStart,
   ColStart,
   Space : Byte;
   Loop : Byte;

Begin
   NumLines := 200;
   NumBars := 10;
   BarSize := 10;
   YStart := 0;
   ColStart := 1;
   Space := 5;
   SetUP(NumLines, NumBars, BarSize, YStart, ColStart, Space);
   Repeat
       RotatePal(NumBars, BarSize,YStart, ColStart, NumLines, True);
       If KeyPressed Then
       Begin
          For Loop := 0 to 63 do
          Begin
             RotatePal(NumBars, BarSize,YStart, ColStart, NumLines, True);
             FadeOut(NumBars, BarSize);
          End;
          Exit;
       End;
   Until False;
End;



Procedure Creds;
Var
      R, G, B : Byte;
   R1, G1, B1 : Byte;
         Loop : Byte;

Begin
   SetText;
   While KeyPressed do ReadKey;

   Asm
      mov   ah, 1
      mov   ch, 1
      mov   cl, 0
      int   10h
   End;

   GetPal(7,R,G,B);
   Pal(7,0,0,0);
   WriteLn('Copper Bars Trainer...');
   WriteLn;
   WriteLn('By EzE of Asphyxia.');
   WriteLn;
   WriteLn('Contact Us on ...');
   WriteLn;
   WriteLn;
   WriteLn('the Asphyxia BBS (031) - 7655312');
   WriteLn;
   WriteLn('Email :       eze@');
   WriteLn('         asphyxia@');
   WriteLn('          edwards@');
   WriteLn('           bailey@');
   WriteLn('          mcphail@');
   WriteLn('                  beastie.cs.und.ac.za');
   WriteLn;
   WriteLn('or  peter.edwards@datavert.co.za');
   WriteLn;
   WriteLn('Write me snail-mail at...');
   WriteLn('P.O. Box 2313');
   WriteLn('Hillcrest');
   WriteLn('Natal');
   WriteLn('3650');
   R1 := 0;
   G1 := 0;
   B1 := 0;
   For Loop := 0 to 63 do
   Begin
      WaitRetrace;
      WaitRetrace;
      Pal(7, R1, G1, B1);
      If R1 < R Then Inc(R1);
      If G1 < G Then Inc(G1);
      If B1 < B Then Inc(B1);
   End;
   Asm
      mov   ah, 1
      mov   ch, 1
      mov   cl, 0
      int   10h
   End;

End;


Procedure Fadecurs;
Var
   Loop : Byte;
   R, G, B : Byte;
Begin
   GetPal(7, R, G, B);
   For Loop := 0 to 63 do
   Begin
      WaitRetrace;
      WaitRetrace;
      Pal(7, R, G, B);
      If R > 0 Then Dec(R);
      If G > 0 Then Dec(G);
      If B > 0 Then Dec(B);
   End;
End;


Begin
   TextAttr := $07;
   While KeyPressed do ReadKey;
   FadeCurs;
   DoItAll;
   Creds;
End.