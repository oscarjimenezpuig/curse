/*
============================================================
  Fichero: curse.h
  Creado: 27-11-2025
  Ultima Modificacion: dissabte, 29 de novembre de 2025, 09:46:11
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

//palette
#define BRIGHT 1 //paleta de maximo brillante
#define NORMAL 2 //paleta de brillo medio
#define LOW 3 //paleta de brillo bajo
#define GREYS 4 //paleta de grises

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

//mode in
#define INKEY 0
#define INPUT 1

//TIPOS
typedef unsigned char u1;

//VARIABLES

//FUNCIONES

void at(int r,int c);
//situa el cursor en la posicion especificada

void attr(u1 status,u1 attributes);
//define los atributos, status es on o off

void background(u1 color);
//establecer color del fondo

void cls();
//borra toda la pantalla, se substituye por espacios con el fondo especificado. Hace refresh

void ink(u1 color);
//establecer color de la tinta

u1 inc(char c);
//dice si una tecla ha sido pulsada (esta en el buffer de teclas)

void listen();
//escucha de las teclas

void outc(char c);
//imprime un caracter

void outs(const char* s,...);
//imprime un string (tipo printf)

void palette(u1 number);
//define la paleta de colores

void show();
//hace el refresh


#endif /* CURSE_H */



