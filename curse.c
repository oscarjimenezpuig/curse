/*
============================================================
  Fichero: curse.c
  Creado: 27-11-2025
  Ultima Modificacion: mar 16 dic 2025 14:34:25
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include <ncurses.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <stdarg.h>
#include <stdbool.h>

#include "curse.h"

//CONSTANTES
#define BUFSIZE 1024

//MACROS
#define TERMDIM getmaxyx(stdscr,TER_R,TER_C)

//VARIABLES

//externas

int CUR_R,CUR_C,TER_R,TER_C;

static bool _flag_end=true;

//estaticas
static char _buffer[BUFSIZE];

//FUNCIONES

//privadas

static void __end() {
	//final del ncurses
	if(!_flag_end) {
		endwin();
		_flag_end=true;
		printf(" and finished\n");//dbg
	}
}

static void __hand_signal(int sig) {
	//final en caso de error
	__end();
	exit(sig);
}

static void __init_pairs() {
	for(int t=BLACK;t<=WHITE;t++) {
		for(int f=BLACK;f<=WHITE;f++) {
			init_pair(t+8*f+1,t,f);
		}
	}
}

static void _at(int r,int c) {
	//coloca el cursor en una determinada posicion
	TERMDIM;
	CUR_R=r;
	CUR_C=c;
}

static void _atributos(int a,int t,int f) {
	//conecta los atributos y el color, desconectando los previos
	const int FLG[]={A_BOLD,A_UNDERLINE,A_REVERSE,A_BLINK,A_PROTECT,A_INVIS,A_DIM};
	const int INT[]={BOLD,UNDERLINE,REVERSE,BLINK,PROTECT,INVIS,DIM};
	const int FCL=7;
	static int atr=0;
	static short clp=1;
	int atrn=0;
	for(int k=0;k<FCL;k++) {
		if(a & INT[k]) atrn|=FLG[k];
	};
	short colpair=t+8*f+1;
	if(atrn!=atr || clp!=colpair) {
		attroff(atr);
		attron(atrn | COLOR_PAIR(colpair));
		atr=atrn;
		clp=colpair;
	}
}

static void _modein(int f) {
	//determina el modo de entrada del curses
	if((f & CURSOR)) curs_set(1);
	else curs_set(0);
	if((f & ECHO)) echo();
	else noecho();
	nodelay(stdscr,(f & DELAY)?FALSE:TRUE);
	if((f & ENTER)) noraw();
	else raw();
}

static bool _prtchr(char c) {
	if(CUR_R>=0 && CUR_R<TER_R && CUR_C>=0 && CUR_C<TER_C) {
		move(CUR_R,CUR_C);
		addch(c);
		CUR_C++;
		return true;
	}
	return false;
}

static void _init() {
	//inicio del ncurses
	signal(SIGINT,__hand_signal);
	signal(SIGSEGV,__hand_signal);
	initscr();
	noecho();
	raw();
	nodelay(stdscr,TRUE);
	curs_set(0);
	keypad(stdscr,TRUE);
	start_color();
	__init_pairs();
	atexit(__end);
	_flag_end=false;
	CUR_R=CUR_C=0;
	move(0,0);
	TERMDIM;
	printf("CURSES inited");
}

//publicas

void cls(int fondo) {
	TERMDIM;
	for(int f=0;f<TER_R;f++) {
		for(int c=0;c<TER_C;c++) {
			printc(f,c,0,fondo,fondo,' ');
		}
	}
	CUR_R=CUR_C=0;
	move(0,0);
}	

int inkey(char c) {
	int count=0;
	char* p=_buffer;
	while(*p!='\0') {
		if(*p++==c) ++count;
	}
	return count;
}

int listen(int modein) {
	static int modeflag=0;
	if(modeflag!=modein) {
		_modein(modein);
		modeflag=modein;
	}
	char* p=_buffer;
	char c=0;
	bool entmod=(modeflag & ENTER)?true:false;
	do {
		c=getch();
		if(c!=ERR) {
			if(c=='\n' && entmod) entmod=false;
			else {
				*p++=c;
			}
		}
	} while(entmod);
	*p='\0';
	return p-_buffer;
}


void printc(int r,int c,int a,int t,int f,char ch) {
	prints(r,c,a,t,f,"%c",ch);
}

void prints(int r,int c,int a,int t,int f,const char* s,...) {
	va_list list;
	va_start(list,s);
	vsprintf(_buffer,s,list);
	va_end(list);
	char* ptr=_buffer;
	_at(r,c);
	_atributos(a,t,f);
	while(*ptr!='\0' && _prtchr(*ptr++));
}

void show() {
	refresh();
}





int main() {
	_init();
	curses();
}
