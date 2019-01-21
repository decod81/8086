{$g+}
uses crt;
var
  x, y, z : word;
  i, j, k : integer;
begin
  asm
    mov ax, 0dh
    int 10h
  end;

  k := 640 div 8;
  portw[$03d4] := $13 + k shl 8;

  for z := 0 to 3 do begin
    portw[$03C4] := $0F02;
    for x := 0 to k-1 do
      for y := 0 to 400-1 do
        mem[$A000:x + y*k shl 1] := 255;
  end;

  for z := 0 to 3 do begin
    portw[$03C4] := $02 + (1 shl z) shl 8; { color plane }
    for x := 1 to k-2 do
      for y := 8 to 400-9 do
        mem[$A000:x + y*k shl 1] := random(256);
  end;

  x := 0;
  y := 0;
  i := 1;
  j := 1;
  repeat
    inc(y, j);
    if (y = 0) or (y = 200) then
      j := -j;

    inc(x, i);
    if (x = 0) or (x = 160) then
      i := -i;

    while (port[$3DA] and 8)=0 do;

    port[$03c0] := $33;
    port[$03c0] := x and 3 shl 1;
    z := y*k shl 1 + x shr 2;
    portw[$3d4] := $0c + z and $ff00;
    portw[$3d4] := $0d + z shl 8;

    while (port[$3DA] and 8)<>0 do;

  until keypressed;

  asm
    mov al, 03h
    int 10h
  end;
end.
