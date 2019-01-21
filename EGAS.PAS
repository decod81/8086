{$g+}
uses crt;
var
  x, y, z : word;
  i, j, k : integer;
  a, b, c : byte;

procedure putpixel(x : integer; y : integer; c : byte);
begin
  portw[$03C4] := $02 + 15 shl 8;
  mem[$A000:(x-1) shr 3 + y*40] := 0;
  mem[$A000:x shr 3 + y*40] := 1 shl (7-(x and 7));
end;

begin
  asm
    mov ax, 0dh
    int 10h
  end;

  portw[$03C4] := $02 + 15 shl 8;
  for x := 0 to 8000 do
    mem[$A000:x] := 0;

  for x := 1 to 319 do begin
    putpixel(x, 100, 1);
    delay(20);
  end;

  for x := 0 to 39 do begin
    portw[$03C4] := $02 + (x and 15) shl 8;
    for y := 0 to 100 do
      mem[$A000:x + y*40] := 255;
  end;

  for a :=0 to 7 do begin
    port[$03c8] := a;
    port[$03c9] := 4*a;
    port[$03c9] := 4*a;
    port[$03c9] := 4*a;
  end;

  for a :=0 to 7 do begin
    port[$03c8] := 16+a;
    port[$03c9] := 4*(a+8);
    port[$03c9] := 4*(a+8);
    port[$03c9] := 4*(a+8);
  end;

  asm
    mov al, 03h
    int 10h
  end;
end.
