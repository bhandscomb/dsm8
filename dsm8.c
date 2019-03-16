/*
#############################################################################
#
#	$VER: dsm8.c 1.1 (28.07.95)
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

#include "protos.h"


/*
#############################################################################
#
#	Version string
#
#############################################################################
*/

char	VersionString[]="$VER: dsm8 1.1 (28.07.95)";


/*
#############################################################################
#
#	Supported CPU definitions
#
#############################################################################
*/

typedef struct {
	char 	*specifier;
	void	(*decoder)();
	} CPUINFO;

CPUINFO cpuinfo[]={
	{"6502",decode6502},
	{"65816",decode65816},
	{"z80",decodez80},
	{NULL,NULL}
	};


/*
#############################################################################
#
#	Standard global vars
#
#############################################################################
*/

FILE *	ifp;			/* The input file to disassemble */
int	filepos=0,
	skip,
	opt_addr=1,		/* OPTION: Show addresses? */
	opt_hex=0,		/* OPTION: Show hex opcodes? */
	opt_rel=0,		/* OPTION: Calculate relative targets? */
	opt_sym=0,		/* OPTION: Symbol table package active? */
	maxsymnamelen=0;	/* Maximum length symbol found */
char	symprintfspec[15];	/* printf() spec for symbol name output */
void	(*decoder)()=NULL;	/* Selected decode routine */

unsigned char	dat[3];		/* Used by both 6502 and 65816 */


/*
#############################################################################
#
#	Standard 'get out clause'
#
#############################################################################
*/

void bye(char *msg,int rc) {
  clrsym();
  if(ifp) fclose(ifp);
  puts(msg);
  exit(rc);
  }


/*
#############################################################################
#
#	Usage info
#
#############################################################################
*/

void usage()
  {
  puts (
    "Usage: dsm8 [<options>] <filename> [<from> <to>]\n"
    "\tOPTIONS:\n"
    "\t\t-a    turn off addresses\n"
    "\t\t-cCPU select CPU (6502/65816/z80) --- MUST be specified!\n"
    "\t\t-h    turn on hex display\n"
    "\t\t-oX   origin of code\n"
    "\t\t-r    turn on relative calc\n"
    "\t\t-sX   header skip\n"
    "\t\t-tFN  symbol table file"
    );
  exit(1);
  }


/*
#############################################################################
#
#	Byte fetcher
#
#############################################################################
*/

unsigned char getbyte() {
  int inchar;
  inchar=fgetc(ifp);
  filepos++;
  if(inchar==EOF)
    bye(" <EOF>",0);
  if(opt_hex)
    printf("%02x",inchar);
  return((unsigned char)inchar);
  }


/*
#############################################################################
#
#	The 'main' program
#
#############################################################################
*/

int main(int argc,char *argv[]) {
  int from,to;
  CPUINFO *cpu;
  if(argc<2) usage();
  while(argv[1][0]=='-') {
    switch(argv[1][1]) {
      case 'a':
        opt_addr=0;
        break;
      case 'c':
        cpu=cpuinfo;
        while(cpu->specifier) {
          if(!strcmp(cpu->specifier,&argv[1][2]))
            break;
          cpu++;
          }
        if(cpu)
          decoder=cpu->decoder;
        else {
          printf("Invalid CPU '%s'\n",&argv[1]);
          clrsym();
          exit(1);
          }
        break;
      case 'h':
        opt_hex=1;
        break;
      case 'o':
        sscanf(&argv[1][2],"%x",&filepos);
        break;
      case 'r':
        opt_rel=1;
        break;
      case 's':
        sscanf(&argv[1][2],"%x",&skip);
        break;
      case 't':
        getsymtable(&argv[1][2]);
        opt_sym=1;
        break;
      default:
        printf("Invalid option '-%c'\n",argv[1][1]);
        clrsym();
        exit(1);
      }
    argv++;
    argc--;
    }
  if(((argc!=2)&&(argc!=4))||(decoder==NULL))
    usage();
  if(argc==4) {
    sscanf(argv[2],"%x",&from);
    sscanf(argv[3],"%x",&to);
    }
  else {
    from=0;
    to=0xffff;
    }
  to+=filepos;
  filepos+=from;
  if(argv[1][0]=='?')
    usage();
  if((ifp=fopen(argv[1],"rb"))) {
    if(skip) fseek(ifp,skip,SEEK_CUR);
    if(from) fseek(ifp,from,SEEK_CUR);
    if(opt_sym)
      sprintf(symprintfspec,"%%-%1ds  ",maxsymnamelen);
    while(filepos<=to)
      (*decoder)();
    }
  else
    puts("Unable to open requested file.");
  return(0);
  }


/*
#############################################################################
#
#	.end.
#
#############################################################################
*/
