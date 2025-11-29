/*
============================================================
  Fichero: curse.c
  Creado: 27-11-2025
  Ultima Modificacion: dissabte, 29 de novembre de 2025, 12:16:21
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include <ncurses.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

#include "curse.h"

//constantes privadas

#define BUFKEYSIZ 5 //numero de teclas que se almacenan
#define CLKKEYDUR 100 //numero de clocks que aguantara la tecla en el buffer

//tipos privados

struct _key_s {
	char chr;
	clock_t clk;
};

//variables estaticas

static int _rows=0; //numero de filas de la terminal
static int _columns=0; //numero de columnas de la terminal
static int _cursor_r=0; //row del cursor
static int _cursor_c=0; //column del cursor
static int _atrflg=0; //bandera de atributos
static u1 _ink=BLACK; //tinta
static u1 _background=BLACK; //fondo
static struct {
	u1 cur : 1;
	u1 col : 1;
	u1 atr : 1;
	u1 end : 1;
} _flag={0,0,0,1}; 
//banderas:
//cur: cursor, col: color, atr: atributo -> indican si hay cambio
//end: indica si ha sido finalizado el curses
struct _key_s _bufkey[BUFKEYSIZ]; //buffer de keys 


//privadas

static void __bufkeyini() {
	struct _key_s* p=_bufkey;
	while(p!=_bufkey+BUFKEYSIZ) {
		*p++=(struct _key_s){0,0};
	}
}

static void __end() {
	if(_flag.end==0) {
		endwin();
		_flag.end=1;
	}
}

static void _attron() {
	attron(_atrflg | COLOR_PAIR(_ink+8*_background));
}

#define CDD 8

static void _color() {
	static u1 coldefs[CDD];
	static u1 inited=0;
	if(!inited) {
		u1* p=coldefs;
		while(p!=coldefs+CDD) *p++=0;
		inited=1;
	}
	if(_ink!=BLACK) {
		u1 fi=1<<_ink;
		if((coldefs[_background] & fi)==0) {
			init_pair(_ink+8*_background,_ink,_background);
			coldefs[_background]|=fi;
		}
	}
}

#undef CDD

static void _init() {
	initscr();
	curs_set(0);
	raw();
	noecho();
	nodelay(stdscr,TRUE);
	start_color();
	for(u1 k=BLACK;k<=WHITE;k++) init_pair(k*8,BLACK,k);
	attron(COLOR_PAIR(BLACK+8*BLACK));
	atexit(__end);
	getmaxyx(stdscr,_rows,_columns);
	__bufkeyini();
	_flag.end=0;
}

static void _signal_resize(int sig) {
	getmaxyx(stdscr,_rows,_columns);
}

//publicas

void at(int r,int c) {
	if(_cursor_r!=r || _cursor_c!=c) {
		_cursor_r=r;
		_cursor_c=c;
		_flag.cur=1;
	}
}

void attr(u1 t,u1 a) {
	const int FLG[]={A_BOLD,A_UNDERLINE,A_REVERSE,A_BLINK,A_PROTECT,A_INVIS,A_DIM};
	const int INT[]={BOLD,UNDERLINE,REVERSE,BLINK,PROTECT,INVIS,DIM};
	const int SIZ=7;
	int flag=0;
	for(u1 f=0;f<SIZ;f++) if(a & INT[f]) flag|=FLG[f];
	if(flag) {
		if(t==ON) _atrflg|=flag;
		else _atrflg&=~(flag);
		_flag.atr=1;
	}
}

void background(u1 c) {
	if(_background!=c) {
		_background=c;
		_flag.atr=_flag.col=1;
	}
}

void cls() {
	at(0,0);
	do {
		outc(' ');
		if(_cursor_c==_columns) {
			_cursor_r++;
			_cursor_c=0;
		}
	}while(_cursor_r<_rows);
}

void ink(u1 c) {
	if(_ink!=c) {
		_ink=c;
		_flag.atr=_flag.col=1;
	}
}

u1 inc(char c) {
	u1 ret=0;
	if(c) {
		clock_t clk=clock();
		struct _key_s* p=_bufkey;
		while(p!=_bufkey+BUFKEYSIZ) {
			if(p->clk<=clk) {
				p->chr=0;
				p->clk=0;
			} else if(ret==0 && p->chr==c) ret=1;
			p++;
		}
	}
	return ret;
}

void listen() {
	int ent=getch();
	if(ent!=ERR) {
		char chr=(char)ent;
		clock_t clk=clock();
		u1 in=0;
		struct _key_s* p=_bufkey;
		struct _key_s* f=NULL;
		while(p!=_bufkey+BUFKEYSIZ) {
			if(p->chr==chr) {
				p->clk=clk+CLKKEYDUR;
				in=1;
			}
			if(p->clk<=clk) {
				if(in==0) {
					f=p;
					in=1;
				} else {
					p->chr=0;
					p->clk=0;
				}
			}
			p++;
		}
		if(f) {
			f->chr=chr;
			f->clk=clk+CLKKEYDUR;
		}
	}
}

void outc(char c) {
	if(_flag.cur) {
		move(_cursor_r,_cursor_c);
		_flag.cur=0;
	}
	if(_flag.atr) {
		if(_flag.col) {
			_color();
			_flag.col=0;
		}
		_attron();
		_flag.atr=0;
	}
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

void palette(u1 n) {
	const int COLS[]={COLOR_BLACK,COLOR_RED,COLOR_GREEN,COLOR_YELLOW,COLOR_BLUE,COLOR_MAGENTA,COLOR_CYAN,COLOR_WHITE};
	const int DIF=125;
	if(n!=GREYS) {
		int value=1000;
		if(n==NORMAL) value=600;
		else if(n==LOW) value=200;
		init_color(COLOR_BLACK,0,0,0);
		init_color(COLOR_RED,value,0,0);
		init_color(COLOR_GREEN,0,value,0);
		init_color(COLOR_YELLOW,value,value,0);
		init_color(COLOR_BLUE,0,0,value);
		init_color(COLOR_MAGENTA,value,0,value);
		init_color(COLOR_CYAN,0,value,value);
		init_color(COLOR_WHITE,value,value,value);
	} else for(int k=0;k<8;k++) init_color(COLS[k],DIF*k,DIF*k,DIF*k);
}

void show() {
	refresh();
}

//prueba

#include <stdio.h>

struct {
	clock_t act;
	clock_t clk;
	char chr;
} save;

int main() {
	_init();
	palette(BRIGHT);
	background(WHITE);
	ink(BLACK);
	cls();
	at(10,10);
	outs("Hola %i",7);
	refresh();
	int time=0;
	while(inc('q')==0) {
		at(20,20);
		outs("Time %04i",time++);
		refresh();
		listen();
	}
	__end();
	clock_t clk=clock();
	for(int k=0;k<BUFKEYSIZ;k++) {
		printf("clock=%li chr=%c clk=%li\n",clk,_bufkey[k].chr,_bufkey[k].clk);
	}
}
