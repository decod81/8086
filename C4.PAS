{$g+}
uses crt;
var
  i, j, k, l, x, y, z : word;
  b : byte;
begin
  asm
    mov ax, 13h
    int 10h
  end;

  portw[$03c4] := $0604; {  SEQ 04h: index 0x02 selects memory plane, use 256 kB }
  portw[$03d4] := $C017; { CRTC 17h: sync enable, byte mode }
  portw[$03d4] := $0014; { CRTC 14h: disable stuff }
  portw[$03d4] := $5013; { CRTC 13h: scan line address difference 8*$50 = 640 }

  for x:=0 to 63 do begin
    port[$03c8] := x;
    port[$03c9] := x and 63;
    port[$03c9] := x and 63;
    port[$03c9] := x and 63;
    port[$03c8] := 64+x;
    port[$03c9] := x and 63;
    port[$03c9] := 0;
    port[$03c9] := 0;
    port[$03c8] := 128+x;
    port[$03c9] := 0;
    port[$03c9] := x and 63;
    port[$03c9] := 0;
    port[$03c8] := 192+x;
    port[$03c9] := 0;
    port[$03c9] := 0;
    port[$03c9] := x and 63;
  end;

  for x := 0 to 320-1 do begin
    port[$03c4] := 2;               { select plane }
    port[$03c5] := 1 shl (x and 3); { which plane }
    for y := 0 to 200-1 do
      mem[$A000:(x shr 2) + 160*y] := x and 63;
  end;
  for x := 320 to 640-1 do begin
    port[$03c4] := 2;
    port[$03c5] := 1 shl (x and 3);
    for y := 0 to 200-1 do
      mem[$A000:(x shr 2) + 160*y] := 64 + x and 63;
  end;
  for x := 0 to 320-1 do begin
    port[$03c4] := 2;
    port[$03c5] := 1 shl (x and 3);
    for y := 200 to 400-1 do
      mem[$A000:(x shr 2) + 160*y] := 128 + x and 63;
  end;
  for x := 320 to 640-1 do begin
    port[$03c4] := 2;
    port[$03c5] := 1 shl (x and 3);
    for y := 200 to 400-1 do
      mem[$A000:(x shr 2) + 160*y] := 192 + x and 63;
  end;

  x := 0;
  y := 0;
  for x := 0 to 320 do begin
    y := (10*x) shr 4;
    while (port[$3DA] and 8)=0 do; { need to wait for vblank }
    port[$03c0] := $33;
    port[$03c0] := x and 3 shl 1;
    j := y*160 + (x shr 2);
    portw[$3d4] := $0c + j and $ff00;
    portw[$3d4] := $0d + j shl 8;
    while (port[$3DA] and 8)<>0 do;
  end;

  repeat
  until keypressed;
  asm
    mov al, 03h
    int 10h
  end;
end.
