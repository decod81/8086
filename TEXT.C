#include <stdio.h>

main() {
  unsigned char far *txt = (unsigned char far*)0xB8000000L;
  unsigned char x, y;
  unsigned short i = 0, j;

  asm {
    // mov ax, 0x03
    // int 0x10

    mov ax, 0x1112
    xor bl, bl
    int 0x10
  }
  for(i=0; i<50*80*2; i++)
    txt[i] = 0;
  i = 0;
  for(y=0; y<4; y++)
    for(x=0; x<80; x++) {
      txt[2*x+0+160*y] = i;
      txt[2*x+1+160*y] = 15;
      i = i + 1;
    }
  while(!kbhit());
}