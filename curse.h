/*
============================================================
  Fichero: curse.h
  Creado: 27-11-2025
  Ultima Modificacion: dijous, 27 de novembre de 2025, 20:02:58
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#ifndef CURSE_H
#define CURSE_H

#include <stdarg.h>

//CONSTANTES

//atributos
#define BOLD 1 //caracter en resaltado
#define UNDERLINE 2 //caracter subrayado
#define REVERSE 4 //caracter inverso
#define BLINK 8 //caracter parpadeante
#define PROTECT 16 //caracter protegido
#define INVIS 32 //caracter invisible
#define DIM 64 //caracter en resaltado medio

//colors
#define BLACK 0
#define RED 1
#define GREEN 2
#define YELLOW 3
#define BLUE 4
#define MAGENTA 5
#define CYAN 6
#define WHITE 7

//on/off
#define OFF 0
#define ON 1

//TIPOS
typedef unsigned char u1;

//VARIABLES

//FUNCIONES

void at(int r,int c);
//situa el cursor en la posicion especificada

void attr(u1 status,u1 attributes);
//define los atributos, status es on o off

void color(u1 ink,u1 background);
//establece el color, fondo y tinta

void outc(char c);
//imprime un caracter

void outs(const char* s,...);
//imprime un string (tipo printf)


#endif /* CURSE_H */



