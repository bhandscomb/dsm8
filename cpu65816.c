/*
#############################################################################
#
#	$VER: cpu65816.c 1.0 (28.07.95)
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

void am65816_abs(void);
void am65816_absidxind(void);
void am65816_absidxx(void);
void am65816_absidxy(void);
void am65816_absind(void);
void am65816_abslng(void);
void am65816_abslngidxx(void);
void am65816_acc(void);
void am65816_blk(void);
void am65816_dir(void);
void am65816_diridxind(void);
void am65816_diridxx(void);
void am65816_diridxy(void);
void am65816_dirind(void);
void am65816_dirindidx(void);
void am65816_dirindlng(void);
void am65816_dirindlngidx(void);
void am65816_imm8(void);
void am65816_imm816a(void);
void am65816_imm816xy(void);
void am65816_imp(void);
void am65816_pcrel(void);
void am65816_pcrellng(void);
void am65816_stkrel(void);
void am65816_stkrelindidx(void);


/*
#############################################################################
#
#	Instruction table
#
#############################################################################
*/

char instrtable65816[256][6] = {
  "BRK\0r","ORA\0k","COP\0r","ORA\0x","TSB\0j","ORA\0j","ASL\0j","ORA\0p",
  "PHP\0u","ORA\0s","ASL\0h","PHD\0u","TSB\0a","ORA\0a","ASL\0a","ORA\0f",
  "BPL\0v","ORA\0o","ORA\0n","ORA\0y","TRB\0j","ORA\0l","ASL\0l","ORA\0q",
  "CLC\0u","ORA\0d","INC\0h","TCS\0u","TRB\0a","ORA\0c","ASL\0c","ORA\0g",
  "JSR\0a","AND\0k","JSL\0f","AND\0x","BIT\0j","AND\0j","ROL\0j","AND\0p",
  "PLP\0u","AND\0s","ROL\0h","PLD\0u","BIT\0a","AND\0a","ROL\0a","AND\0f",
  "BMI\0v","AND\0o","AND\0n","AND\0y","BIT\0l","AND\0l","ROL\0l","AND\0q",
  "SEC\0u","AND\0d","DEC\0h","TSC\0u","BIT\0c","AND\0c","ROL\0c","AND\0g",
  "RTI\0u","EOR\0k","WDM\0u","EOR\0x","MVP\0i","EOR\0j","LSR\0j","EOR\0p",
  "PHA\0u","EOR\0s","LSR\0h","PHK\0u","JMP\0a","EOR\0a","LSR\0a","EOR\0f",
  "BVC\0v","EOR\0o","EOR\0n","EOR\0y","MVN\0i","EOR\0l","LSR\0l","EOR\0q",
  "CLI\0u","EOR\0d","PHY\0u","TCD\0u","JML\0f","EOR\0c","LSR\0c","EOR\0g",
  "RTS\0u","ADC\0k","PER\0w","ADC\0x","STZ\0j","ADC\0j","ROR\0j","ADC\0p",
  "PLA\0u","ADC\0s","ROR\0h","RTL\0u","JMP\0e","ADC\0a","ROR\0a","ADC\0f",
  "BVS\0v","ADC\0o","ADC\0n","ADC\0y","STZ\0l","ADC\0l","ROR\0l","ADC\0q",
  "SEI\0u","ADC\0d","PLY\0u","TDC\0u","JMP\0b","ADC\0c","ROR\0c","ADC\0g",
  "BRA\0v","STA\0k","BRL\0w","STA\0x","STY\0j","STA\0j","STX\0j","STA\0p",
  "DEY\0u","BIT\0s","TXA\0u","PHB\0u","STY\0a","STA\0a","STX\0a","STA\0f",
  "BCC\0v","STA\0o","STA\0n","STA\0y","STY\0l","STA\0l","STX\0m","STA\0q",
  "TYA\0u","STA\0d","TXS\0u","TXY\0u","STZ\0a","STA\0c","STZ\0c","STA\0g",
  "LDY\0t","LDA\0k","LDX\0t","LDA\0x","LDY\0j","LDA\0j","LDX\0j","LDA\0p",
  "TAY\0u","LDA\0s","TAS\0u","PLB\0u","LDY\0a","LDA\0a","LDX\0a","LDA\0f",
  "BCS\0v","LDA\0o","LDA\0n","LDA\0y","LDY\0l","LDA\0l","LDX\0m","LDA\0q",
  "CLV\0u","LDA\0d","TSX\0u","TYX\0u","LDY\0c","LDA\0c","LDX\0d","LDA\0g",
  "CPY\0t","CMP\0k","REP\0r","CMP\0x","CPY\0j","CMP\0j","DEC\0j","CMP\0p",
  "INY\0u","CMP\0s","DEX\0u","WAI\0u","CPY\0a","CMP\0a","DEC\0a","CMP\0f",
  "BNE\0v","CMP\0o","CMP\0n","CMP\0y","PEI\0n","CMP\0l","DEC\0l","CMP\0q",
  "CLD\0u","CMP\0d","PHX\0u","STP\0u","JML\0e","CMP\0c","DEC\0c","CMP\0g",
  "CPX\0t","SBC\0k","SEP\0r","SBC\0x","CPX\0j","SBC\0j","INC\0j","SBC\0p",
  "INX\0u","SBC\0s","NOP\0u","XBA\0u","CPX\0a","SBC\0a","INC\0a","SBC\0f",
  "BEQ\0v","SBC\0o","SBC\0n","SBC\0y","PEA\0a","SBC\0l","INC\0l","SBC\0q",
  "SED\0u","SBC\0d","PLX\0u","XCE\0u","JSR\0b","SBC\0c","INC\0c","SBC\0g"
  };


/*
#############################################################################
#
#	Addressing mode table
#
#############################################################################
*/

void (*fmt65816[])(void) = {
	am65816_abs,
	am65816_absidxind,
	am65816_absidxx,
	am65816_absidxy,
	am65816_absind,
	am65816_abslng,
	am65816_abslngidxx,
	am65816_acc,
	am65816_blk,
	am65816_dir,
	am65816_diridxind,
	am65816_diridxx,
	am65816_diridxy,
	am65816_dirind,
	am65816_dirindidx,
	am65816_dirindlng,
	am65816_dirindlngidx,
	am65816_imm8,
	am65816_imm816a,
	am65816_imm816xy,
	am65816_imp,
	am65816_pcrel,
	am65816_pcrellng,
	am65816_stkrel,
	am65816_stkrelindidx
	};


/*
#############################################################################
#
#	Globals for this module
#
#############################################################################
*/

int BITS_A=8,BITS_XY=8;


/*
#############################################################################
#
#	Addressing mode code
#
#############################################################################
*/

void am65816_abs(void) {
  char *symbol;
  if((symbol=getsym(dat[1]<<8|dat[0])))
    printf(" %s",symbol);
  if(!symbol)
    printf(" $%02x%02x",dat[1],dat[0]);
  }

void am65816_absidxind(void) {
  char *symbol;
  if((symbol=getsym(dat[1]<<8|dat[0])))
    printf(" (%s,X)",symbol);
  if(!symbol)
    printf(" ($%02x%02x,X)",dat[1],dat[0]);
  }

void am65816_absidxx(void) {
  char *symbol;
  if((symbol=getsym(dat[1]<<8|dat[0])))
    printf(" %s,X",symbol);
  if(!symbol)
    printf(" $%02x%02x,X",dat[1],dat[0]);
  }

void am65816_absidxy(void) {
  char *symbol;
  if((symbol=getsym(dat[1]<<8|dat[0])))
    printf(" %s,Y",symbol);
  if(!symbol)
    printf(" $%02x%02x,Y",dat[1],dat[0]);
  }

void am65816_absind(void) {
  char *symbol;
  if((symbol=getsym(dat[1]<<8|dat[0])))
    printf(" (%s)",symbol);
  if(!symbol)
    printf(" ($%02x%02x)",dat[1],dat[0]);
  }

void am65816_abslng(void) {
  char *symbol;
  if((symbol=getsym(dat[2]<<16|dat[1]<<8|dat[0])))
    printf(" %s",symbol);
  if(!symbol)
    printf(" $%02x%02x%02x",dat[2],dat[1],dat[0]);
  }

void am65816_abslngidxx(void) {
  char *symbol;
  if((symbol=getsym(dat[2]<<16|dat[1]<<8|dat[0])))
    printf(" %s,X",symbol);
  if(!symbol)
    printf(" $%02x%02x%02x,X",dat[2],dat[1],dat[0]);
  }

void am65816_acc(void) {
  printf(" A");
  }

void am65816_blk(void) {
  printf(" $%02x,$%02x",dat[1],dat[0]);
  }

void am65816_dir(void) {
  char *symbol;
  if((symbol=getsym(dat[0])))
    printf(" %s",symbol);
  if(!symbol)
    printf(" $%02x",dat[0]);
  }

void am65816_diridxind(void) {
  char *symbol;
  if((symbol=getsym(dat[0])))
    printf(" (%s,X)",symbol);
  if(!symbol)
    printf(" ($%02x,X)",dat[0]);
  }

void am65816_diridxx(void) {
  char *symbol;
  if((symbol=getsym(dat[0])))
    printf(" %s,X",symbol);
  if(!symbol)
    printf(" $%02x,X",dat[0]);
  }

void am65816_diridxy(void) {
  char *symbol;
  if((symbol=getsym(dat[0])))
    printf(" %s,Y",symbol);
  if(!symbol)
    printf(" $%02x,Y",dat[0]);
  }

void am65816_dirind(void) {
  char *symbol;
  if((symbol=getsym(dat[0])))
    printf(" (%s)",symbol);
  if(!symbol)
    printf(" ($%02x)",dat[0]);
  }

void am65816_dirindidx(void) {
  char *symbol;
  if((symbol=getsym(dat[0])))
    printf(" (%s),Y",symbol);
  if(!symbol)
    printf(" ($%02x),Y",dat[0]);
  }

void am65816_dirindlng(void) {
  char *symbol;
  if((symbol=getsym(dat[0])))
    printf(" [%s]",symbol);
  if(!symbol)
    printf(" [$%02x]",dat[0]);
  }

void am65816_dirindlngidx(void) {
  char *symbol;
  if((symbol=getsym(dat[0])))
    printf(" [%s],Y",symbol);
  if(!symbol)
    printf(" [$%02x],Y",dat[0]);
  }

void am65816_imm8(void) {
  printf(" #$%02x",dat[0]);
  }

void am65816_imm816a(void) {
  if(BITS_A==8)
    printf(" #$%02x",dat[0]);
  else
    printf(" #$%02x%02x",dat[1],dat[0]);
  }

void am65816_imm816xy(void) {
  if(BITS_XY==8)
    printf(" #$%02x",dat[0]);
  else
    printf(" #$%02x%02x",dat[1],dat[0]);
  }

void am65816_imp(void) {
  }

void am65816_pcrel(void) {
  char *symbol;
  if(opt_rel) {
    if((symbol=getsym(filepos+(char)dat[0])))
      printf(" %s",symbol);
    else
      printf(" %c$%06x",(dat[0]>=128)?'-':'+',filepos+(char)dat[0]);
    }
  else
    printf(" %c$%02x",(dat[0]>=128)?'-':'+',abs((char)dat[0]));
  }

void am65816_pcrellng(void) {
  char *symbol;
  short offset;
  if(opt_rel) {
    offset=dat[1]<<8|dat[0];
    if((symbol=getsym(filepos+offset)))
      printf(" %s",symbol);
    else
      printf(" %c$%06x",(dat[1]>=128)?'-':'+',filepos+offset);
    }
  else
    printf(" %c$%04x",(dat[1]>=128)?'-':'+',abs(offset));
  }

void am65816_stkrel(void) {
  printf(" $%02x,S",dat[0]);
  }

void am65816_stkrelindidx(void) {
  printf(" ($%02x,S),Y",dat[0]);
  }


/*
#############################################################################
#
#	65816 decoder
#
#############################################################################
*/

void decode65816(void) {
  unsigned char instr,imode;
  char *symbol;
  if(opt_addr)
    printf("%06x\t",filepos&0xffff);
  if(opt_sym) {
    if((symbol=getsym(filepos&0xffff)))
      printf(symprintfspec,symbol);
    else
      printf(symprintfspec,"");
    }
  instr=getbyte();
  imode=instrtable65816[instr][4];
  if(imode=='u') {
    if(opt_hex)
      printf("\t  ");
    printf("%s",instrtable65816[instr]);
    }
  else {
    dat[0]=getbyte();
    if (
      ((imode=='s')&&(BITS_A==16)) ||
      ((imode=='t')&&(BITS_XY==16)) ||
      ((imode>='a')&&(imode<='g')) ||
      (imode=='i') ||
      (imode=='w')
      )
      dat[1]=getbyte();
    if((imode=='f')||(imode=='g')) {
      dat[2]=getbyte();
      if(opt_hex)
        printf("  ");
      }
    else
      if(opt_hex)
        printf("\t  ");
    printf("%s",instrtable65816[instr]);
    (*fmt65816[imode-'a'])();
    }
  putchar('\n');
  if(instr==0xc2) {
    if(dat[0]&0x10)
      BITS_XY=16;
    if(dat[0]&0x20)
      BITS_A=16;
    }
  if(instr==0xe2) {
    if(dat[0]&0x10)
      BITS_XY=8;
    if(dat[0]&0x20)
      BITS_A=8;
    }
  }


/*
#############################################################################
#
#	.end.
#
#############################################################################
*/
