uses crt, dos;

procedure SBIRQ; interrupt;
var
  b : byte;
begin
  b := port[$22e];  { ACK SB }
  port[$20] := $20; { ACK PIC }
  write('*');
end;

procedure writeDSP(value : byte);
begin
  while port[$22c] And $80 <> 0 do;
  port[$22c] := value;
end;

var
  f : file;
  snd : pointer;
  numread : word;
  size : word;
  offset : word;

begin
  clrscr;

  setintvec($0f, addr(SBIRQ));
  port[$21] := port[$21] and 127; { enable IRQ 7 }

  size := 1024*32;

  getmem(snd, size);
  assign(f, 'kingsv.wav');
  reset(f, 1);
  blockread(f, snd^, size, numread);
  close(f);

  writeDSP($40); { sample rate }
  writeDSP(256 - 1000000 div 4000);

  offset := 16*seg(snd^) + ofs(snd^);
  port[$0A] := 15;              { DMA channels 0-3 off }
  port[$0C] := 0;               { clear MSB/LSB flip flop }
  { port[$0B] := $49;             { single cycle DMA }
  port[$0B] := $59;             { looping DMA }
  port[$02] := lo(offset);
  port[$02] := hi(offset);
  port[$83] := (seg(snd^) + ofs(snd^) shr 4) shr 12; { high 4 bits of address }
  port[$03] := lo(size-1);
  port[$03] := hi(size-1);
  port[$0A] := 1;               { DMA channel 1 on }

  (*
  writeDSP($14);                { 8-bit single cycle DMA }
  writeDSP(lo(size)-1);
  writeDSP(hi(size)-1);
  *)

  writeDSP($48);                { 8-bit auto-init DMA }
  writeDSP(lo((size shr 2)-1)); { 4 ints per buffer }
  writeDSP(hi((size shr 2)-1));
  writeDSP($1c);                { start DMA }

  repeat
  until keypressed;
  port[$22c] := $da;
  port[$20] := $20;
  port[$21] := 128 + port[$21] and 127; { disable IRQ 7 }
  freemem(snd, 1024*32);
end.