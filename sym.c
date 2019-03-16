/*
#############################################################################
#
#	$VER: sym.c 1.0 (25.07.95)
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
#	Symbol table definitions
#
#############################################################################
*/

#define SYMBOLS_PER_BLOCK	200
#define SYMBOLS_TEXT_SIZE	4000
#define idSB 0x69645342
#define idST 0x69645354


/*
#############################################################################
#
#	Symbol table storage structures
#
#############################################################################
*/

struct symblock {
  unsigned int ID;
  struct symblock *next;
  int symvalue[SYMBOLS_PER_BLOCK];
  char *symname[SYMBOLS_PER_BLOCK];
  };

struct symtext {
  unsigned int ID;
  struct symtext *next;
  int freespace;
  char text[SYMBOLS_TEXT_SIZE];
  };


/*
#############################################################################
#
#	Symbol table globals
#
#############################################################################
*/

int symcount=SYMBOLS_PER_BLOCK;
struct symblock *rootsymblock=NULL;
struct symtext *rootsymtext=NULL;


/*
#############################################################################
#
#	Symbol table code - get symbol table from file
#
#############################################################################
*/

void getsymtable(char *filename) {
  char symline[80],symname[80];
  int symmode=16,symvalue;
  if((ifp=fopen(filename,"r"))==NULL)
    bye("Error opening symbol table file",1);
  for(;;) {
    fgets(symline,80,ifp);
    if(feof(ifp))
      break;
    if(symline[0]=='#') {
      if(!strncmp(&symline[1],"DEC",3)) symmode=10;
      else if(!strncmp(&symline[1],"HEX",3)) symmode=16;
      else if(strncmp(&symline[1],"REM",3)) bye("Invalid symbol command",1);
      }
    else {
      switch(symmode) {
        case 10: sscanf(symline,"%d %s",&symvalue,symname); break;
        case 16: sscanf(symline,"%x %s",&symvalue,symname); break;
        }
      addsym(symvalue,symname);
      }
    }
  fclose(ifp);
  ifp=NULL;
  }


/*
#############################################################################
#
#	Symbol table code - add symbol
#
#############################################################################
*/

void addsym(int value,char *name) {
  struct symblock *sb,*newsb;
  struct symtext *st,*newst;
  int namelen;
  int neednewst=0;
  namelen=strlen(name)+1;
  if(rootsymblock) {
    sb=rootsymblock;
    while(sb->next)
      sb=sb->next;
    if(sb->ID!=idSB)
      bye("\n\n\t*** SERIOUS SYSTEM ERROR - CORRUPT SB STRUCTURE ***\n\n",20);
    }
  if(symcount==SYMBOLS_PER_BLOCK) {
    if((newsb=malloc(sizeof(struct symblock)))==NULL)
      bye("Error allocating symblock",1);
    if(rootsymblock) {
      sb->next=newsb;
      sb=sb->next;
      }
    else
      rootsymblock=sb=newsb;
    sb->ID=idSB;
    sb->next=NULL;
    symcount=0;
    }
  if(rootsymtext) {
    st=rootsymtext;
    while((st->next)&&(st->freespace<namelen))
      st=st->next;
    if(st->ID!=idST)
      bye("\n\n\t*** SERIOUS SYSTEM ERROR - CORRUPT ST STRUCTURE ***\n\n",20);
    if(st->freespace<namelen)
      neednewst=1;
    }
  else
    neednewst=1;
  if(neednewst) {
    if((newst=malloc(sizeof(struct symtext)))==NULL)
      bye("Error allocating symtext",1);
    newst->next=NULL;
    newst->freespace=SYMBOLS_TEXT_SIZE;
    if(rootsymtext) {
      st->next=newst;
      st=st->next;
      }
    else
      rootsymtext=st=newst;
    st->ID=idST;
    }
  sb->symvalue[symcount]=value;
  sb->symname[symcount]=&st->text[SYMBOLS_TEXT_SIZE-st->freespace];
  strcpy(sb->symname[symcount],name);
  symcount++;
  st->freespace-=namelen;
  if(namelen-1>maxsymnamelen)
    maxsymnamelen=namelen-1;
  }


/*
#############################################################################
#
#	Symbol table code - clear
#
#############################################################################
*/

void clrsym() {
  struct symblock *sb,*sbnext;
  struct symtext *st,*stnext;
  if(rootsymblock) {
    sb=rootsymblock;
    while(sb) {
      sbnext=sb->next;
      free(sb);
      sb=sbnext;
      }
    rootsymblock=NULL;
    }
  if(rootsymtext) {
    st=rootsymtext;
    while(st) {
      stnext=st->next;
      free(st);
      st=stnext;
      }
    rootsymtext=NULL;
    }
  }


/*
#############################################################################
#
#	Symbol table code - lookup symbol
#
#############################################################################
*/

char *getsym(int value) {
  struct symblock *sb;
  int sym,numbsyms;
  if(rootsymblock) {
    sb=rootsymblock;
    while(sb) {
      if(sb->next)
        numbsyms=SYMBOLS_PER_BLOCK;
      else
        numbsyms=symcount;
      for(sym=0;sym<numbsyms;sym++)
        if(sb->symvalue[sym]==value)
          return(sb->symname[sym]);
      sb=sb->next;
      }
    }
  return(NULL);
  }


/*
#############################################################################
#
#	.end.
#
#############################################################################
*/
