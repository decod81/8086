// gcc x86.c -lSDL -lOpenGL32
// https://www.felixcloutier.com/x86/aam
// https://en.wikipedia.org/wiki/X86_instruction_listings
// http://www.gabrielececchetti.it/Teaching/CalcolatoriElettronici/Docs/i8086_instruction_set.pdf
// https://c9x.me/x86/html/file_module_x86_id_218.html
// http://www.c-jump.com/CIS77/reference/Instructions_by_Opcode.html
// http://www.c-jump.com/CIS77/CPU/x86/index.html

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <time.h>
#include <sys/timeb.h>
#include <memory.h>
// #include <unistd.h>
#include <fcntl.h>

unsigned char mem[0x10FFF0];
int surface[320*200];

//#define DEBUG 1

#define GL_BGRA 0x80E1

#define AX 0
#define CX 1
#define DX 2
#define BX 3
#define SP 4
#define BP 5
#define SI 6
#define DI 7
#define ES 8
#define CS 9
#define SS 10
#define DS 11

#define AL 0
#define AH 1
#define CL 2
#define CH 3
#define DL 4
#define DH 5
#define BL 6
#define BH 7

int vgapal[] = {0x000000,0x0000AA,0x00AA00,0x00AAAA,0xAA0000,0xAA00AA,0xAA5500,0xAAAAAA,0x555555,0x5555FF,0x55FF55,0x55FFFF,0xFF5555,0xFF55FF,0xFFFF55,0xFFFFFF,0x000000,0x141414,0x202020,0x2C2C2C,0x383838,0x444444,0x505050,0x616161,0x717171,0x818181,0x919191,0xA1A1A1,0xB6B6B6,0xCACACA,0xE2E2E2,0xFFFFFF,0x0000FF,0x4000FF,0x7D00FF,0xBE00FF,0xFF00FF,0xFF00BE,0xFF007D,0xFF0040,0xFF0000,0xFF4000,0xFF7D00,0xFFBE00,0xFFFF00,0xBEFF00,0x7DFF00,0x40FF00,0x00FF00,0x00FF40,0x00FF7D,0x00FFBE,0x00FFFF,0x00BEFF,0x007DFF,0x0040FF,0x7D7DFF,0x9D7DFF,0xBE7DFF,0xDE7DFF,0xFF7DFF,0xFF7DDE,0xFF7DBE,0xFF7D9D,0xFF7D7D,0xFF9D7D,0xFFBE7D,0xFFDE7D,0xFFFF7D,0xDEFF7D,0xBEFF7D,0x9DFF7D,0x7DFF7D,0x7DFF9D,0x7DFFBE,0x7DFFDE,0x7DFFFF,0x7DDEFF,0x7DBEFF,0x7D9DFF,0xB6B6FF,0xC6B6FF,0xDAB6FF,0xEAB6FF,0xFFB6FF,0xFFB6EA,0xFFB6DA,0xFFB6C6,0xFFB6B6,0xFFC6B6,0xFFDAB6,0xFFEAB6,0xFFFFB6,0xEAFFB6,0xDAFFB6,0xC6FFB6,0xB6FFB6,0xB6FFC6,0xB6FFDA,0xB6FFEA,0xB6FFFF,0xB6EAFF,0xB6DAFF,0xB6C6FF,0x000071,0x1C0071,0x380071,0x550071,0x710071,0x710055,0x710038,0x71001C,0x710000,0x711C00,0x713800,0x715500,0x717100,0x557100,0x387100,0x1C7100,0x007100,0x00711C,0x007138,0x007155,0x007171,0x005571,0x003871,0x001C71,0x383871,0x443871,0x553871,0x613871,0x713871,0x713861,0x713855,0x713844,0x713838,0x714438,0x715538,0x716138,0x717138,0x617138,0x557138,0x447138,0x387138,0x387144,0x387155,0x387161,0x387171,0x386171,0x385571,0x384471,0x505071,0x595071,0x615071,0x695071,0x715071,0x715069,0x715061,0x715059,0x715050,0x715950,0x716150,0x716950,0x717150,0x697150,0x617150,0x597150,0x507150,0x507159,0x507161,0x507169,0x507171,0x506971,0x506171,0x505971,0x000040,0x100040,0x200040,0x300040,0x400040,0x400030,0x400020,0x400010,0x400000,0x401000,0x402000,0x403000,0x404000,0x304000,0x204000,0x104000,0x004000,0x004010,0x004020,0x004030,0x004040,0x003040,0x002040,0x001040,0x202040,0x282040,0x302040,0x382040,0x402040,0x402038,0x402030,0x402028,0x402020,0x402820,0x403020,0x403820,0x404020,0x384020,0x304020,0x284020,0x204020,0x204028,0x204030,0x204038,0x204040,0x203840,0x203040,0x202840,0x2C2C40,0x302C40,0x342C40,0x3C2C40,0x402C40,0x402C3C,0x402C34,0x402C30,0x402C2C,0x40302C,0x40342C,0x403C2C,0x40402C,0x3C402C,0x34402C,0x30402C,0x2C402C,0x2C4030,0x2C4034,0x2C403C,0x2C4040,0x2C3C40,0x2C3440,0x2C3040,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000};

unsigned char scancodes[] = {0x00,0x1B,0x00,0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x48,0x50,0x4D,0x4B,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x00,0x00,0x00,0x39,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1E,0x30,0x2E,0x20,0x12,0x21,0x22,0x23,0x17,0x24,0x25,0x26,0x32,0x31,0x18,0x19,0x10,0x13,0x1F,0x14,0x16,0x2F,0x11,0x2D,0x15,0x2C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

int main() {
  unsigned char xb, yb, zb;
  unsigned short xw, yw, zw;
  unsigned int xd, yd, zd;
  unsigned char regs8[256];
  unsigned short *regs16, ip;
  unsigned char CF, AF, ZF, SF, TF, IF, DF, OF; // carry, adjust, zero, sign, trap, interrupt enable, direction, overflow
  unsigned char opcode;
  int texture, palette[256];
  int cyclecount;
  SDL_Surface *sdl_screen;
  int a, b, c;
 
  sdl_screen = SDL_SetVideoMode(320*3, 200*3, 32, SDL_OPENGL);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glEnable(GL_TEXTURE_2D);  

  for(a=0; a<256; a++)
    palette[a] = vgapal[a];
  
  regs16 = (unsigned short *)(regs8);

  int program = open("vga.com", O_RDONLY);
  read(program, mem + 16*0x01ED + 0x0100, 0xFF00);
  close(program);

  regs16[AX] = 0;
  regs16[BX] = 0;
  regs16[CX] = 0x00FF;
  regs16[DX] = 0x01ED;

  regs16[SI] = 0x0100;
  regs16[DI] = 0xFFFE;
  regs16[BP] = 0x091C;
  regs16[SP] = 0xFFFE;

  regs16[DS] = 0x01ED;
  regs16[CS] = 0x01ED;
  regs16[ES] = 0x01ED;
  regs16[SS] = 0x01ED;
  ip = 0x0100;
  IF = 1;

  for(int addr;;) {
    addr = 16*regs16[CS] + ip;
    switch(mem[addr]) {
//////// //////// //////// //////// //////// //////// //////// ////////

      case 0x01: // add mem16, reg16
        switch(mem[addr+1]) {
          case 0xC8: // add ax, cx
#ifdef DEBUG
            printf("%.4x  add ax, cx\n", ip);
            fflush(stdout);
#endif
            regs16[AX] += regs16[CX];
            if(regs16[AX]==0) ZF = 1; else ZF = 0;
            if(regs16[AX]<0x8000) SF = 0; else SF = 1;
            ip += 2;
            break;
        }
        break;

//////// //////// //////// //////// //////// //////// //////// ////////

      case 0x04: // add al, imm8
#ifdef DEBUG
        printf("%.4x  add al, %.2x\n", ip, mem[addr+1]);
        fflush(stdout);
#endif
        regs8[AL] += mem[addr+1];
        if(regs8[AL]==0) ZF = 1; else ZF = 0;
        if(regs8[AL]<0x80) SF = 0; else SF = 1;
        ip += 2;
        break;

//////// //////// //////// //////// //////// //////// //////// ////////

      case 0x07: // pop es
#ifdef DEBUG
        printf("%.4x  pop es\n", ip);
        fflush(stdout);
#endif
        regs16[ES] = *(unsigned short*)&mem[16*regs16[SS] + regs16[SP] + 2];
        regs16[SP] += 2;
        ip += 1;
        break;

//////// //////// //////// //////// //////// //////// //////// ////////

      case 0x29: // sub mem16, reg16
        switch(mem[addr+1]) {
          case 0xE8: // sub ax, bp
#ifdef DEBUG
            printf("%.4x  sub ax, bp\n", ip);
            fflush(stdout);
#endif DEBUG
            regs16[AX] -= regs16[BP];
            ip += 2;
            break;
          default:
            printf("%.4x  !!!! unimplemented opcode !!!!\n", ip);
            fflush(stdout);
            exit(0);
        }
        break;

//////// //////// //////// //////// //////// //////// //////// ////////

      case 0x2B: // sub reg16, mem16
        switch(mem[addr+1]) {
          case 0x14: // sub dx, [ds:si]
#ifdef DEBUG
            printf("%.4x  sub dx, [ds:si]\n", ip);
            fflush(stdout);
#endif
            regs16[DX] = regs16[DX] - (mem[16*regs16[DS]+regs16[SI]+1]<<16) - mem[16*regs16[DS]+regs16[SI]];
            if(regs16[DX]==0) ZF = 1; else ZF = 0;
            if(regs16[DX]<0x8000) SF = 0; else SF = 1;
            ip += 2;
            break;

         //////// //////// //////// //////// //////// //////// ////////

          case 0x04: // sub ax, [ds:si]
#ifdef DEBUG
            printf("%.4x  sub ax, [ds:si]\n", ip);
            fflush(stdout);
#endif
            regs16[AX] = regs16[AX] - (mem[16*regs16[DS]+regs16[SI]+1]<<16) - mem[16*regs16[DS]+regs16[SI]];
            if(regs16[AX]==0) ZF = 1; else ZF = 0;
            if(regs16[AX]<0x8000) SF = 0; else SF = 1;
            ip += 2;
            break;           
          default:
            printf("%.4x  !!!! unimplemented opcode !!!!\n", ip);
            fflush(stdout);
            exit(0);
        }
        break;

//////// //////// //////// //////// //////// //////// //////// ////////

      case 0x45: // inc bp
#ifdef DEBUG
        printf("%.4x  inc bp\n", ip);
        fflush(stdout);
#endif
        regs16[BP] = regs16[BP] + 1;
        if(regs16[BP]==0) ZF = 1; else ZF = 0;
        if(regs16[BP]<0x8000) SF = 0; else SF = 1;
        ip = ip + 1;
        break;

//////// //////// //////// //////// //////// //////// //////// ////////

      case 0x47: // inc di
#ifdef DEBUG
	printf("%.4x  inc di\n", ip);
        fflush(stdout);
#endif
        regs16[DI] = regs16[DI] + 1;
        if(regs16[DI]==0) ZF = 1; else ZF = 0;
        if(regs16[DI]<0x8000) SF = 0; else SF = 1;
        ip = ip + 1;
        break;

//////// //////// //////// //////// //////// //////// //////// ////////

      case 0x52: // push dx
#ifdef DEBUG
        printf("%.4x  push dx\n", ip);
        fflush(stdout);
#endif
        a = 16*regs16[SS] + regs16[SP];
        mem[a+1]= regs8[DH];
        mem[a+0]= regs8[DL];
	regs16[SP] -= 2;
        ip += 1;
        break;

//////// //////// //////// //////// //////// //////// //////// ////////

      case 0x75: // jnz
#ifdef DEBUG
        printf("%.4x  jnz %.2d (%.4x)\n", ip, (char)mem[addr+1]+2, ip + (char)mem[addr+1]+2);
        fflush(stdout);
#endif
        ip = ip + 2;
        if(ZF==0)
          ip = ip + (char)mem[addr+1];
        break;

//////// //////// //////// //////// //////// //////// //////// ////////

      case 0x78: // js rel8
#ifdef DEBUG
        printf("%.4x  js %.2d\n", ip, (char)mem[addr+1]-2);
        fflush(stdout);
#endif
        ip = ip + 2;
        if(SF==1)
          ip = ip + (char)mem[addr+1];
        break;

//////// //////// //////// //////// //////// //////// //////// ////////

      case 0x89: // mov mem16, reg16
        switch(mem[addr+1]) {
          case 0xF8:
#ifdef DEBUG
            printf("%.4x  mov ax, di\n", ip);
            fflush(stdout);
#endif
            regs16[AX] = regs16[DI];
            ip += 2;
            break;
        }
        break;

//////// //////// //////// //////// //////// //////// //////// ////////

      case 0x91: // xchg ax, cx
#ifdef DEBUG
        printf("%.4x  xchg ax, cx\n", ip);
        fflush(stdout);
#endif
        regs16[AX] ^= regs16[CX];
        regs16[CX] ^= regs16[AX];
        regs16[AX] ^= regs16[CX];
        ip += 1;
        break;

//////// //////// //////// //////// //////// //////// //////// ////////

      case 0x92: // xchg ax, dx
#ifdef DEBUG
        printf("%.4x  xchg ax, dx\n", ip);
        fflush(stdout);
#endif
        regs16[AX] ^= regs16[DX];
        regs16[DX] ^= regs16[AX];
        regs16[AX] ^= regs16[DX];
        ip += 1;
        break;

//////// //////// //////// //////// //////// //////// //////// ////////

      case 0x93: // xchg ax, bx
#ifdef DEBUG
        printf("%.4x  xchg ax, bx\n", ip);
        fflush(stdout);
#endif
        regs16[AX] ^= regs16[BX];
        regs16[BX] ^= regs16[AX];
        regs16[AX] ^= regs16[BX];
        ip += 1;
        break;

//////// //////// //////// //////// //////// //////// //////// ////////

      case 0x99: // cwd
#ifdef DEBUG
        printf("%.4x  cwd\n", ip);
        fflush(stdout);
#endif
        if(regs16[AX]<0x8000)
          regs16[DX] = 0x0000;
        else
          regs16[DX] = 0xFFFF;
        ip += 1;
        break;

//////// //////// //////// //////// //////// //////// //////// ////////

      case 0xAA: // stosb
#ifdef DEBUG
        printf("%.4x  stosb\n", ip);
        fflush(stdout);
#endif
        mem[16*regs16[ES] + regs16[DI]] = regs8[AL];
        if(DF==0)
          regs16[DI] = regs16[DI] + 1;
        else
          regs16[DI] = regs16[DI] - 1;
        ip = ip + 1;
        break;

//////// //////// //////// //////// //////// //////// //////// ////////

      case 0xB8: // mov ax, imm
#ifdef DEBUG
        printf("%.4x  mov ax, $%.4x\n", ip, regs16[AX]);
        fflush(stdout);
#endif
        regs16[AX] = *(unsigned short*)&mem[addr+1];
        ip += 3;
        break;
      case 0xBA: // mov dx, imm16
#ifdef DEBUG        
	printf("%.4x  mov dx, $%.4x\n", ip, regs16[DX]);
        fflush(stdout);
#endif
        regs16[DX] = *(unsigned short*)&mem[addr+1];
        ip += 3;
        break;

//////// //////// //////// //////// //////// //////// //////// ////////

      case 0xBB: // mov bx, imm16
#ifdef DEBUG
        printf("%.4x  mov bx, $%.4x\n", ip, regs16[BX]);
        fflush(stdout);
#endif
        regs16[BX] = *(unsigned short*)&mem[addr+1];
        ip += 3;
        break;

//////// //////// //////// //////// //////// //////// //////// ////////

      case 0xC1: // rol/shl/sal
        switch(mem[addr+1]) {

          case 0xE2: // shl dx, imm8
#ifdef DEBUG
            printf("%.4x  shl dx, $%.2x\n", ip, mem[addr+2]);
            fflush(stdout);
#endif
            regs16[DX] = regs16[DX] << mem[addr+2];
            ip += 3;
            break;

         //////// //////// //////// //////// //////// //////// ////////

          case 0xE8: // shr ax, imm8
#ifdef DEBUG
            printf("%.4x  shr ax, $%.2x\n", ip, mem[addr+2]);
            fflush(stdout);
#endif
            regs16[AX] = regs16[AX] >> mem[addr+2];
            ip += 3;
            break;           

         //////// //////// //////// //////// //////// //////// ////////

          default:
            printf("%.4x  !!!! unimplemented opcode !!!!\n", ip);
            fflush(stdout);
            exit(0);
        }
        break;

//////// //////// //////// //////// //////// //////// //////// ////////

      case 0xC7: // mov mem16, imm16
#ifdef DEBUG        
        printf("%.4x  mov [ds:$%.4x], $%.4x\n", ip, regs16[DS], (mem[addr+3] << 8) + mem[addr+2]);
        fflush(stdout);
#endif
        mem[16*regs16[DS] + regs16[SI]] = (mem[addr+3] << 8) + mem[addr+2];
        ip += 4;
        break;

//////// //////// //////// //////// //////// //////// //////// ////////

      case 0xCD: // int imm8
#ifdef DEBUG
        printf("%.4x  int $%.2x\n", ip, mem[addr+1]);
        fflush(stdout);
#endif
	// DOES NOTHING AT THE MOMENT
        ip += 2;
        break;

//////// //////// //////// //////// //////// //////// //////// ////////

      case 0xD4: // aam imm8
#ifdef DEBUG
        printf("%.4x  aam $%.2x\n", ip, mem[addr+1]);
        fflush(stdout);
#endif
        xb = regs16[AX];
        regs8[AH] = xb / mem[addr+1];
        regs8[AL] = xb % mem[addr+1];
        ip += 2;
        break;

//////// //////// //////// //////// //////// //////// //////// ////////

      case 0xEB: // jmp
#ifdef DEBUG
        printf("%.4x  jmp %.2d (%.4x)\n", ip, (char)mem[addr+1]+2, ip + (char)mem[addr+1]+2);
        fflush(stdout);
	exit(0);
#endif
	ip = ip + 2;
        ip = ip + (char)mem[addr+1];
        break;        

//////// //////// //////// //////// //////// //////// //////// ////////

      case 0xF7: // div/idiv/imul/mul/neg/not/test
        switch(mem[addr+1]) {

          case 0xF3: // div bx
#ifdef DEBUG
            printf("%.4x  div bx\n", ip);
            fflush(stdout);
#endif
            xd = (regs16[DX] << 16) + regs16[AX];
            regs16[AX] = xd / regs16[BX];
            regs16[DX] = xd % regs16[BX];
            ip += 2;
            break;

         //////// //////// //////// //////// //////// //////// ////////

          case 0xD0: // not ax
#ifdef DEBUG
            printf("%.4x  not ax\n", ip);
            fflush(stdout);
#endif
            regs16[AX] ^= 0xFFFF;
            ip += 2;
            break;

         //////// //////// //////// //////// //////// //////// ////////

          case 0xFB: // idiv bx
#ifdef DEBUG
            printf("%.4x  idiv bx\n", ip);
            fflush(stdout);
#endif
            a = (regs16[DX] << 16) + regs16[AX];
            b = (short)regs16[BX];
            regs16[AX] = a / b;
            regs16[DX] = a % b;
            ip += 2;
            break;       

         //////// //////// //////// //////// //////// //////// ////////

          default:
            printf("%.4x  !!!! unimplemented opcode !!!!\n", ip);
            fflush(stdout);
            exit(0);
        }
        break;

//////// //////// //////// //////// //////// //////// //////// ////////

      default:
        printf("%.4x  !!!! unimplemented opcode !!!!\n", ip);
        fflush(stdout);
        exit(0);
    }
    
#ifdef DEBUG
    printf("AX %.4x  BX %.4x  CX %.4x  DX %.4x\n", regs16[AX], regs16[BX], regs16[CX], regs16[DX]);
    printf("SP %.4x  BP %.4x  SI %.4x  DI %.4x\n", regs16[SP], regs16[BP], regs16[SI], regs16[DI]);
    printf("ES %.4x  CS %.4x  SS %.4x  DS %.4x\n", regs16[ES], regs16[CS], regs16[SS], regs16[DS]);
    printf("CF %.4x  AF %.4x  ZF %.4x  SF %.4x\n", CF, AF, ZF, SF);
    printf("\n");
    getch();
#endif

    if(!(++cyclecount%200000)) {
      for(int i=0; i<320; i++)
	for(int j=0; j<200; j++)
	  surface[i+j*320] = palette[mem[0xA0000+i+j*320]];
      glBindTexture(GL_TEXTURE_2D, texture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 320, 200, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface);
      glBegin(GL_QUADS);
      glTexCoord2f(0, 0);
      glVertex2f(-1, 1);
      glTexCoord2f(1, 0);
      glVertex2f(1, 1);
      glTexCoord2f(1, 1);
      glVertex2f(1, -1);
      glTexCoord2f(0, 1);
      glVertex2f(-1, -1);
      glEnd();
      SDL_GL_SwapBuffers();
    }
  }
  
  SDL_Quit();  
  return 0;
}