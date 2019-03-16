/*
#############################################################################
#
#	$VER: cpuz80.c 2.0 (25.07.95)
#
#############################################################################
*/


/*
#############################################################################
#
#	Includes
#
#############################################################################
*/

#include <stdio.h>
#include <stdlib.h>

#include "dsm8.h"
#include "protos.h"


/*
#############################################################################
#
#	Prototypes
#
#############################################################################
*/

void addtmp(void);
void decodebyte(unsigned char);
void _C0(unsigned char);
void _C1(unsigned char);
void _C2(unsigned char);
void _C0F0(void);
void _C0F1(void);
void _C0F2(void);
void _C0F3(void);
void _C1F0(void);
void _C1F1(void);
void _C1F2(void);
void _C1F3(void);
void _C2F1(void);
void _C2F2(void);
void _C0F0H0(void);
void _C0F0H1(void);
void _C0F0H2(void);
void _C0F0H3(void);
void _C0F0H4(void);
void _C0F0H5(void);
void _C0F0H6(void);
void _C0F0H7(void);
void _C0F3H0(void);
void _C0F3H1(void);
void _C0F3H2(void);
void _C0F3H3(void);
void _C0F3H4(void);
void _C0F3H5(void);
void _C0F3H6(void);
void _C0F3H7(void);
void _C2F1H0(void);
void _C2F1H1(void);
void _C2F1H2(void);
void _C2F1H3(void);
void _C2F1H4(void);
void _C2F1H5(void);
void _C2F1H6(void);
void _C2F1H7(void);
void _C0F0H1K0(void);
void _C0F0H1K1(void);
void _C0F0H3K0(void);
void _C0F0H3K1(void);
void _C0F3H1K0(void);
void _C0F3H1K1(void);
void _C0F3H5K0(void);
void _C0F3H5K1(void);
void _C2F1H2K0(void);
void _C2F1H2K1(void);
void _C2F1H3K0(void);
void _C2F1H3K1(void);
void _C2F1H5K0(void);
void _C2F1H5K1(void);
void _Z80_EMPTY(void);


/*
#############################################################################
#
#	General look-up tables
#
#############################################################################
*/

char *__r[] = {
	"B","C","D","E","H","L","x","A"
};

char *__s[] = {
	"BC","DE","y","SP"
};

char *__q[] = {
	"BC","DE","y","AF"
};

char *__n[] = {
	"0","1","2","3","4","5","6","7"
};

char *__c[] = {
	"NZ","Z","NC","C","PO","PE","P","M"
};

char *__x[] = {
	"ADD  A,","ADC  A,","SUB  ","SBC  A,",
	"AND  ","XOR  ","OR   ","CP   "
};


/*
#############################################################################
#
#	Decoder function tables
#
#############################################################################
*/

void (*_C[])(unsigned char) = {_C0,_C1,_C2};

void (*_C0F[])(void) = {_C0F0,_C0F1,_C0F2,_C0F3};
void (*_C1F[])(void) = {_C1F0,_C1F1,_C1F2,_C1F3};
void (*_C2F[])(void) = {_Z80_EMPTY,_C2F1,_C2F2,_Z80_EMPTY};

void (*_C0F0H[])(void) = {_C0F0H0,_C0F0H1,_C0F0H2,_C0F0H3,
	_C0F0H4,_C0F0H5,_C0F0H6,_C0F0H7};
void (*_C0F3H[])(void) = {_C0F3H0,_C0F3H1,_C0F3H2,_C0F3H3,
	_C0F3H4,_C0F3H5,_C0F3H6,_C0F3H7};
void (*_C2F1H[])(void) = {_C2F1H0,_C2F1H1,_C2F1H2,_C2F1H3,
	_C2F1H4,_C2F1H5,_C2F1H6,_C2F1H7};

void (*_C0F0H1K[])(void) = {_C0F0H1K0,_C0F0H1K1};
void (*_C0F0H3K[])(void) = {_C0F0H3K0,_C0F0H3K1};
void (*_C0F3H1K[])(void) = {_C0F3H1K0,_C0F3H1K1};
void (*_C0F3H5K[])(void) = {_C0F3H5K0,_C0F3H5K1};
void (*_C2F1H2K[])(void) = {_C2F1H2K0,_C2F1H2K1};
void (*_C2F1H3K[])(void) = {_C2F1H3K0,_C2F1H3K1};
void (*_C2F1H5K[])(void) = {_C2F1H5K0,_C2F1H5K1};


/*
#############################################################################
#
#	Globals for this module
#
#############################################################################
*/

unsigned char CLASS,INDEX,__F,__G,__H,__J,__K;
int ilen,DISP;
char DIS[40],DISFINAL[80],*DISFINptr,tmp[40];


/*
#############################################################################
#
#	Z80 decoder
#
#############################################################################
*/

void decodez80(void) {
  int x,y,addr;
  char *symbol;
  CLASS=INDEX=ilen=0;
  DISP=-1;
  DIS[0]='\0';
  if(opt_addr)
    printf("%04x  ",filepos&0xffff);
  if(opt_sym) {
    if(symbol=getsym(filepos&0xffff))
      printf(symprintfspec,symbol);
    else
      printf(symprintfspec,"");
    }
  x=getbyte();
  ilen++;
  decodebyte(x);
  DISFINptr=DISFINAL;
  for(x=0;x<strlen(DIS);x++) {
    switch(DIS[x]) {
      case '@':
        *DISFINptr++='+';
        break;
      case 'v':
        if(DIS[x+1]=='v') {
          x++;
          y=getbyte();
          addr=(getbyte()<<8)+y;
          ilen+=2;
          if(symbol=getsym(addr))
            strcpy(tmp,symbol);
          else
            sprintf(tmp,"%04x",addr);
          addtmp();
          }
        else {
          y=getbyte();
          ilen++;
          if((DIS[x-1]=='@')&&(opt_rel)) {
            if(symbol=getsym(filepos+(char)y)) {
              strcpy(tmp,symbol);
              DISFINptr--;
              }
            else
              sprintf(tmp,"%04x",filepos+(char)y);
            }
          else {
            if((DIS[x-1]=='@')&&(y>=128)) {
              sprintf(tmp,"%02x",256-y);
              *(DISFINptr-1)='-';
              }
            else sprintf(tmp,"%02x",y);
            }
          addtmp();
          }
        break;
      case 'y':
        switch(INDEX) {
          case 0:
            *DISFINptr++='H';
            *DISFINptr++='L';
            break;
          case 1:
            *DISFINptr++='I';
            *DISFINptr++='X';
            break;
          case 2:
            *DISFINptr++='I';
            *DISFINptr++='Y';
            break;
          }
        break;
      case 'x':
        switch(INDEX) {
          case 0:
            *DISFINptr++='(';
            *DISFINptr++='H';
            *DISFINptr++='L';
            *DISFINptr++=')';
            break;
          case 1:
            if(DISP<0) {
              DISP=getbyte();
              ilen++;
              }
            if(DISP>=128)
              sprintf(tmp,"(IX-%02x)",256-DISP);
            else
              sprintf(tmp,"(IX+%02x)",DISP);
            addtmp();
            break;
          case 2:
            if(DISP<0) {
              DISP=getbyte();
              ilen++;
              }
            if(DISP>=128)
              sprintf(tmp,"(IY-%02x)",256-DISP);
            else
              sprintf(tmp,"(IY+%02x)",DISP);
            addtmp();
            break;
          }
        break;
      default:
        *DISFINptr++=DIS[x];
      }
    }
  if(opt_hex) for(x=0;x<(5-ilen)*2;x++) putchar(' ');
  *DISFINptr='\0';
  printf("%s\n",DISFINAL);
  }


/*
#############################################################################
#
#	Add 'tmp' to DISFINAL
#
#############################################################################
*/

void addtmp(void) {
  *DISFINptr='\0';
  strcat(DISFINAL,tmp);
  while(*DISFINptr++)
    /*nothing*/;
  DISFINptr--;
  }


/*
#############################################################################
#
#	The guts of the decoder (level 0)
#
#############################################################################
*/

void decodebyte(unsigned char byte) {
  __F=(byte&0xc0)>>6;
  __G=(byte&0x38)>>3;
  __H=(byte&0x07);
  __J=(__G&0x06)>>1;
  __K=(__G&0x01);
  (*_C[CLASS])(byte);
  }


/*
#############################################################################
#
#	The guts of the decoder (level 1)
#
#############################################################################
*/

void _C0(unsigned char byte) {
  switch(byte) {
    case 0x76:
      strcpy(DIS,"HALT");
      return;
    case 0xCB:
      CLASS=1;
      if(INDEX)
        DISP=getbyte();
      decodebyte(getbyte());
      return;
    case 0xED:
      CLASS=2;
      decodebyte(getbyte());
      return;
    case 0xDD:
      INDEX=1;
      decodebyte(getbyte());
      return;
    case 0xFD:
      INDEX=2;
      decodebyte(getbyte());
      return;
    }
  (*_C0F[__F])();
  }

void _C1(unsigned char byte) {
  (*_C1F[__F])();
  }

void _C2(unsigned char byte) {
  (*_C2F[__F])();
  }


/*
#############################################################################
#
#	The guts of the decoder (level 2)
#
#############################################################################
*/

void _C0F0(void) {
  (*_C0F0H[__H])();
  }

void _C0F1(void) {
  strcpy(DIS,"LD   ");
  strcat(DIS,__r[__G]);
  strcat(DIS,",");
  strcat(DIS,__r[__H]);
  }

void _C0F2(void) {
  strcpy(DIS,__x[__G]);
  strcat(DIS,__r[__H]);
  }

void _C0F3(void) {
  (*_C0F3H[__H])();
  }

void _C1F0(void) {
  switch(__G) {
    case 0: strcpy(DIS,"RLC  ");break;
    case 1: strcpy(DIS,"RRC  ");break;
    case 2: strcpy(DIS,"RL   ");break;
    case 3: strcpy(DIS,"RR   ");break;
    case 4: strcpy(DIS,"SLA  ");break;
    case 5: strcpy(DIS,"SRA  ");break;
    case 6: strcpy(DIS,"");break;
    case 7: strcpy(DIS,"SRL  ");break;
    }
  strcat(DIS,__r[__H]);
  }

void _C1F1(void) {
  strcpy(DIS,"BIT  ");
  strcat(DIS,__n[__G]);
  strcat(DIS,",");
  strcat(DIS,__r[__H]);
  }

void _C1F2(void) {
  strcpy(DIS,"RES  ");
  strcat(DIS,__n[__G]);
  strcat(DIS,",");
  strcat(DIS,__r[__H]);
  }

void _C1F3(void) {
  strcpy(DIS,"SET  ");
  strcat(DIS,__n[__G]);
  strcat(DIS,",");
  strcat(DIS,__r[__H]);
  }

void _C2F1(void) {
  (*_C2F1H[__H])();
  }

void _C2F2(void) {
  switch(__H) {
    case 0: strcpy(DIS,"LD");break;
    case 1: strcpy(DIS,"CP");break;
    case 2: strcpy(DIS,"IN");break;
    case 3: strcpy(DIS,"OT");break;
    }
  switch(__G) {
    case 4: strcat(DIS,"I");break;
    case 5: strcat(DIS,"D");break;
    case 6: strcat(DIS,"IR");break;
    case 7: strcat(DIS,"DR");break;
    }
  }


/*
#############################################################################
#
#	The guts of the decoder (level 3)
#
#############################################################################
*/

void _C0F0H0(void) {
  if(__G>3) {
    strcpy(DIS,"JR   ");
    strcat(DIS,__c[__G-4]);
    strcat(DIS,",@v");
    }
  else {
    switch(__G) {
      case 0: strcpy(DIS,"NOP");break;
      case 1: strcpy(DIS,"EX   AF,AF'");break;
      case 2: strcpy(DIS,"DJNZ @v");break;
      case 3: strcpy(DIS,"JR   @v");break;
      }
    }
  }

void _C0F0H1(void) {
  (*_C0F0H1K[__K])();
  }

void _C0F0H2(void) {
  strcpy(DIS,"LD   ");
  switch(__G) {
    case 0: strcat(DIS,"(BC),A");break;
    case 1: strcat(DIS,"A,(BC)");break;
    case 2: strcat(DIS,"(DE),A");break;
    case 3: strcat(DIS,"A,(DE)");break;
    case 4: strcat(DIS,"(vv),y");break;
    case 5: strcat(DIS,"y,(vv)");break;
    case 6: strcat(DIS,"(vv),A");break;
    case 7: strcat(DIS,"A,(vv)");break;
    }
  }

void _C0F0H3(void) {
  (*_C0F0H3K[__K])();
  }

void _C0F0H4(void) {
  strcpy(DIS,"INC  ");
  strcat(DIS,__r[__G]);
  }

void _C0F0H5(void) {
  strcpy(DIS,"DEC  ");
  strcat(DIS,__r[__G]);
  }

void _C0F0H6(void) {
  strcpy(DIS,"LD   ");
  strcat(DIS,__r[__G]);
  strcat(DIS,",v");
  }

void _C0F0H7(void) {
  switch(__G) {
    case 0: strcpy(DIS,"RLCA");break;
    case 1: strcpy(DIS,"RRCA");break;
    case 2: strcpy(DIS,"RLA");break;
    case 3: strcpy(DIS,"RRA");break;
    case 4: strcpy(DIS,"DAA");break;
    case 5: strcpy(DIS,"CPL");break;
    case 6: strcpy(DIS,"SCF");break;
    case 7: strcpy(DIS,"CCF");break;
    }
  }

void _C0F3H0(void) {
  strcpy(DIS,"RET  ");
  strcat(DIS,__c[__G]);
  }

void _C0F3H1(void) {
  (*_C0F3H1K[__K])();
  }

void _C0F3H2(void) {
  strcpy(DIS,"JP   ");
  strcat(DIS,__c[__G]);
  strcat(DIS,",vv");
  }

void _C0F3H3(void) {
  switch(__G) {
    case 0: strcpy(DIS,"JP   vv");break;
    case 1: strcpy(DIS,"");break;
    case 2: strcpy(DIS,"OUT  (v),A");break;
    case 3: strcpy(DIS,"IN   A,(v)");break;
    case 4: strcpy(DIS,"EX   (SP),y");break;
    case 5: strcpy(DIS,"EX   DE,HL");break;
    case 6: strcpy(DIS,"DI");break;
    case 7: strcpy(DIS,"EI");break;
    }
  }

void _C0F3H4(void) {
  strcpy(DIS,"CALL ");
  strcat(DIS,__c[__G]);
  strcat(DIS,",vv");
  }

void _C0F3H5(void) {
  (*_C0F3H5K[__K])();
  }

void _C0F3H6(void) {
  strcpy(DIS,__x[__G]);
  strcat(DIS,"v");
  }

void _C0F3H7(void) {
  strcpy(DIS,"RST  ");
  switch(__G) {
    case 0: strcat(DIS,"00");break;
    case 1: strcat(DIS,"08");break;
    case 2: strcat(DIS,"10");break;
    case 3: strcat(DIS,"18");break;
    case 4: strcat(DIS,"20");break;
    case 5: strcat(DIS,"28");break;
    case 6: strcat(DIS,"30");break;
    case 7: strcat(DIS,"38");break;
    }
  }

void _C2F1H0(void) {
  strcpy(DIS,"IN   ");
  strcat(DIS,__r[__G]);
  strcat(DIS,",(C)");
  }

void _C2F1H1(void) {
  strcpy(DIS,"OUT  (C),");
  strcat(DIS,__r[__G]);
  }

void _C2F1H2(void) {
  (*_C2F1H2K[__K])();
  }

void _C2F1H3(void) {
  (*_C2F1H3K[__K])();
  }

void _C2F1H4(void) {
  strcpy(DIS,"NEG");
  }

void _C2F1H5(void) {
  (*_C2F1H5K[__K])();
  }

void _C2F1H6(void) {
  switch(__G) {
    case 0: strcpy(DIS,"IM   0");break;
    case 1: strcpy(DIS,"");break;
    case 2: strcpy(DIS,"IM   1");break;
    case 3: strcpy(DIS,"IM   2");break;
    case 4: strcpy(DIS,"");break;
    case 5: strcpy(DIS,"");break;
    case 6: strcpy(DIS,"");break;
    case 7: strcpy(DIS,"");break;
    }
  }

void _C2F1H7(void) {
  switch(__G) {
    case 0: strcpy(DIS,"LD   I,A");break;
    case 1: strcpy(DIS,"LD   R,A");break;
    case 2: strcpy(DIS,"LD   A,I");break;
    case 3: strcpy(DIS,"LD   A,R");break;
    case 4: strcpy(DIS,"RRD");break;
    case 5: strcpy(DIS,"RLD");break;
    case 6: strcpy(DIS,"");break;
    case 7: strcpy(DIS,"");break;
    }
  }


/*
#############################################################################
#
#	The guts of the decoder (level 4)
#
#############################################################################
*/

void _C0F0H1K0(void) {
  strcpy(DIS,"LD   ");
  strcat(DIS,__s[__J]);
  strcat(DIS,",vv");
  }

void _C0F0H1K1(void) {
  strcpy(DIS,"ADD  y,");
  strcat(DIS,__s[__J]);
  }

void _C0F0H3K0(void) {
  strcpy(DIS,"INC  ");
  strcat(DIS,__s[__J]);
  }

void _C0F0H3K1(void) {
  strcpy(DIS,"DEC  ");
  strcat(DIS,__s[__J]);
  }

void _C0F3H1K0(void) {
  strcpy(DIS,"POP  ");
  strcat(DIS,__q[__J]);
  }

void _C0F3H1K1(void) {
  switch(__J) {
    case 0: strcpy(DIS,"RET");break;
    case 1: strcpy(DIS,"EXX");break;
    case 2: strcpy(DIS,"JP   (y)");break;
    case 3: strcpy(DIS,"LD   SP,y");break;
    }
  }

void _C0F3H5K0(void) {
  strcpy(DIS,"PUSH ");
  strcat(DIS,__q[__J]);
  }

void _C0F3H5K1(void) {
  strcpy(DIS,"CALL vv");
  }

void _C2F1H2K0(void) {
  strcpy(DIS,"SBC  HL,");
  strcat(DIS,__s[__J]);
  }

void _C2F1H2K1(void) {
  strcpy(DIS,"ADC  HL,");
  strcat(DIS,__s[__J]);
  }

void _C2F1H3K0(void) {
  strcpy(DIS,"LD   (vv),");
  strcat(DIS,__s[__J]);
  }

void _C2F1H3K1(void) {
  strcpy(DIS,"LD   ");
  strcat(DIS,__s[__J]);
  strcat(DIS,",(vv)");
  }

void _C2F1H5K0(void) {
  strcpy(DIS,"RETN");
  }

void _C2F1H5K1(void) {
  strcpy(DIS,"RETI");
  }


/*
#############################################################################
#
#	This replaces a couple of empty functions
#
#############################################################################
*/

void _Z80_EMPTY(void) {
  }


/*
#############################################################################
#
#	.end.
#
#############################################################################
*/
