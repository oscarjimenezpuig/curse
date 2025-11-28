/*
============================================================
  Fichero: curse.c
  Creado: 27-11-2025
  Ultima Modificacion: vie 28 nov 2025 12:31:09
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include <ncurses.h>

#include "curse.h"

static int _cursor_r=0;
static int _cursor_c=0;
static u1 _atrflg=0;
static u1 _ink=BLACK;
static u1 _background=BLACK;

static void _cinit() {
	initscr();
	raw();
	noecho();
	start_color();
	for(u1 k=BLACK;k<=WHITE;k++) init_pair(k*8,BLACK,k);
	attron(COLOR_PAIR(BLACK+8*BLACK));
}

static void _cend() {
	endwin();
}

void at(int r,int c) {
	_cursor_r=r;
	_cursor_c=c;
	move(r,c);
}

static void _attron() {
	attron(_atrflg | COLOR_PAIR(_ink+8*_background));
}

void attr(u1 t,u1 a) {
	const int FLG[]={A_BOLD,A_UNDERLINE,A_REVERSE,A_BLINK,A_PROTECT,A_INVIS,A_DIM};
	const int INT[]={BOLD,UNDERLINE,REVERSE,BLINK,PROTECT,INVIS,DIM};
	const int SIZ=7;
	u1 flag=0;
	for(u1 f=0;f<SIZ;f++) if(a & INT[f]) flag|=FLG[f];
	if(t==ON) _atrflg|=flag;
	else _atrflg&=~(flag);
	_attron();
}

#define CDD 8

void color(u1 i,u1 b) {
	static u1 coldefs[CDD];
	static u1 inited=0;
	if(i!=_ink || b!=_background) {
		if(!inited) {
			u1* p=coldefs;
			while(p!=coldefs+CDD) *p++=0;
			inited=1;
		}
		if(i!=BLACK) {
			u1 fi=1<<i;
			if((coldefs[b] & fi)==0) {
				init_pair(i+8*b,i,b);
				coldefs[b]|=fi;
			}
		}
		_attron();
	}
}

#undef CDD

void outc(char c) {
	addch(c);
	_cursor_c++;
	move(_cursor_r,_cursor_c);
}

void outs(const char* s,...) {
	char str[1024];
	va_list list;
	va_start(list,s);
	vsprintf(str,s,list);
	va_end(list);
	char* c=str;
	while(*c!='\0') outc(*c++);
}

//prueba

int main() {
	_cinit();
	attr(ON,BOLD);
	color(WHITE,RED);
	at(10,10);
	outs("Hola %i",7);
	refresh();
	getch();
	_cend();
}
