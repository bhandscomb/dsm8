/*
#############################################################################
#
#	$VER: cpu6502.c 2.1 (28.07.95)
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

void am6502_rel(void);
void am6502_imm(void);
void am6502_z(void);
void am6502_zx(void);
void am6502_abs(void);
void am6502_absx(void);
void am6502_absy(void);
void am6502_izx(void);
void am6502_izy(void);
void am6502_ind(void);


/*
#############################################################################
#
#	Instruction table
#
#############################################################################
*/

char instrtable6502[256][6] = {
	"BRK\0a","ORA\0i","","","","ORA\0d","ASL\0d","",
	"PHP\0a","ORA\0c","ASL\0a","","","ORA\0f","ASL\0f","",
	"BPL\0b","ORA\0j","","","","ORA\0e","ASL\0e","",
	"CLC\0a","ORA\0h","","","","ORA\0g","ASL\0g","",
	"JSR\0f","AND\0i","","","BIT\0d","AND\0d","ROL\0d","",
	"PLP\0a","AND\0c","ROL\0a","","BIT\0f","AND\0f","ROL\0f","",
	"BMI\0b","AND\0j","","","","AND\0e","ROL\0e","",
	"SEC\0a","AND\0h","","","","AND\0g","ROL\0g","",
	"RTI\0a","EOR\0i","","","","EOR\0d","LSR\0d","",
	"PHA\0a","EOR\0c","LSR\0a","","JMP\0f","EOR\0f","LSR\0f","",
	"BVC\0b","EOR\0j","","","","EOR\0e","LSR\0e","",
	"CLI\0a","EOR\0h","","","","EOR\0g","LSR\0g","",
	"RTS\0a","ADC\0i","","","","ADC\0d","ROR\0d","",
	"PLA\0a","ADC\0c","ROR\0a","","JMP\0k","ADC\0f","ROR\0f","",
	"BVS\0b","ADC\0j","","","","ADC\0e","ROR\0e","",
	"SET\0a","ADC\0h","","","","ADC\0g","ROR\0g","",
	"","STA\0i","","","STY\0d","STA\0d","STX\0d","",
	"DEY\0a","","TXA\0a","","STY\0f","STA\0f","STX\0f","",
	"BCC\0b","STA\0j","","","STY\0e","STA\0e","STX\0e","",
	"TYA\0a","STA\0h","TXS\0a","","","STA\0g","","",
	"LDY\0c","LDA\0i","LDX\0c","","LDY\0d","LDA\0d","LDX\0d","",
	"TAY\0a","LDA\0c","TAX\0a","","LDY\0f","LDA\0f","LDX\0f","",
	"BCS\0b","LDA\0j","","","LDY\0e","LDA\0e","LDX\0e","",
	"CLV\0a","LDA\0h","TSX\0a","","LDY\0g","LDA\0g","LDX\0h","",
	"CPY\0c","CMP\0i","","","CPY\0d","CMP\0d","DEC\0d","",
	"INY\0a","CMP\0c","DEX\0a","","CPY\0f","CMP\0f","DEC\0f","",
	"BNE\0b","CMP\0j","","","","CMP\0e","DEC\0e","",
	"CLD\0a","CMP\0h","","","","CMP\0g","DEC\0g","",
	"CPX\0c","SBC\0i","","","CPX\0d","SBC\0d","INC\0d","",
	"INX\0a","SBC\0c","NOP\0a","","CPX\0f","SBC\0f","INC\0f","",
	"BEQ\0b","SBC\0j","","","","SBC\0e","INC\0e","",
	"SED\0a","SBC\0h","","","","SBC\0g","INC\0g",""
	};


/*
#############################################################################
#
#	Addressing mode table
#
#############################################################################
*/

void (*fmt6502[])(void) = {
	am6502_rel,
	am6502_imm,
	am6502_z,
	am6502_zx,
	am6502_abs,
	am6502_absx,
	am6502_absy,
	am6502_izx,
	am6502_izy,
	am6502_ind
	};


/*
#############################################################################
#
#	Addressing mode code
#
#############################################################################
*/

void am6502_rel(void) {
  char *symbol;
  if(opt_rel) {
    if((symbol=getsym(filepos+(char)dat[0])))
      printf(" %s",symbol);
    else
      printf(" %c$%04x",(dat[0]>=128)?'-':'+',filepos+(char)dat[0]);
    }
  else
    printf(" %c$%02x",(dat[0]>=128)?'-':'+',abs((char)dat[0]));
  }

void am6502_imm(void) {
  printf(" #$%02x",dat[0]);
  }

void am6502_z(void) {
  printf(" $%02x",dat[0]);
  }

void am6502_zx(void) {
  printf(" $%02x,X",dat[0]);
  }

void am6502_abs(void) {
  char *symbol;
  if((symbol=getsym(dat[1]<<8|dat[0])))
    printf(" %s",symbol);
  if(!symbol)
    printf(" $%02x%02x",dat[1],dat[0]);
  }

void am6502_absx(void) {
  char *symbol;
  if((symbol=getsym(dat[1]<<8|dat[0])))
    printf(" %s",symbol);
  if(!symbol)
    printf(" $%02x%02x,X",dat[1],dat[0]);
  }

void am6502_absy(void) {
  char *symbol;
  if((symbol=getsym(dat[1]<<8|dat[0])))
    printf(" %s",symbol);
  if(!symbol)
    printf(" $%02x%02x,Y",dat[1],dat[1]);
  }

void am6502_izx(void) {
  printf(" ($%02x,X)",dat[0]);
  }

void am6502_izy(void) {
  printf(" ($%02x),Y",dat[0]);
  }

void am6502_ind(void) {
  char *symbol;
  if((symbol=getsym(dat[1]<<8|dat[0])))
    printf(" %s",symbol);
  if(!symbol)
    printf(" ($%02x%02x)",dat[1],dat[0]);
  }


/*
#############################################################################
#
#	6502 decoder
#
#############################################################################
*/

void decode6502(void) {
  unsigned char instr,imode;
  char *symbol;
  if(opt_addr)
    printf("%04x\t",filepos&0xffff);
  if(opt_sym) {
    if((symbol=getsym(filepos&0xffff)))
      printf(symprintfspec,symbol);
    else
      printf(symprintfspec,"");
    }
  instr=getbyte();
  imode=instrtable6502[instr][4];
  if(instrtable6502[instr][0]) {
    if(imode=='a') {
      if(opt_hex)
        putchar('\t');
      printf("%s",instrtable6502[instr]);
      }
    else {
      dat[0]=getbyte();
      if(((imode>='f')&&(imode<='h'))||(imode=='k'))
        dat[1]=getbyte();
      if(opt_hex)
        putchar('\t');
      printf("%s",instrtable6502[instr]);
      (*fmt6502[imode-'b'])();
      }
    putchar('\n');
    }
  else {
    if(opt_hex)
      putchar('\t');
    printf("DB  $%02x\n",instr);
    }
  }


/*
#############################################################################
#
#	.end.
#
#############################################################################
*/
