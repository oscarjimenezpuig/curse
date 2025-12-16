/*
============================================================
  Fichero: curse.h
  Creado: 27-11-2025
  Ultima Modificacion: mar 16 dic 2025 14:33:39
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#ifndef CURSE_H
#define CURSE_H


//CONSTANTES

//palette
#define BRIGHT 1 //paleta de maximo brillante
#define MEDIUM 2 //paleta de brillo medio
#define LOW 3 //paleta de brillo bajo
#define GREYS 4 //paleta de grises

//atributos
#define NONE 0 //no se aplica nada
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

//mode in
#define NORMAL 0 //No hay echo, no se espera a la introduccion de teclas, no se espera al enter no hay cursor
#define ECHO 1 //las teclas se reproducen en pantalla
#define DELAY 2 //se espera a la introduccion de teclas
#define ENTER 4// se espera al enter
#define CURSOR 8 //pone el cursor

//METODOS

//VARIABLES
//estas variables solo sirven para adquirir los valores, nunca para cambiarlos
extern int CUR_R; //fila del cursor
extern int CUR_C; //columna del cursor
extern int TER_R; //dimension en filas de la terminal
extern int TER_C; //dimension en columnas de la terminal

//FUNCIONES

//principales

//secundarias (acceso mediante metodos)

void cls(int fondo);
//hace un cls de toda la pantalla en el fondo elegido, coloca los cursores en 0,0

int inkey(char chr);
//comprueba si el caracter c esta en el buffer (devuelve el numero de veces)

int listen(int modein_flags);
//escuchamos el teclado, introduciendo las banderas indicando como queremos que sea la escucha

void printc(int r,int c,int atributo,int tinta,int fondo,char chr);
//impresion de un solo caracter

void prints(int r,int c,int atributo,int tinta,int fondo,const char* str,...);
//impresion de un string en la posicion r,c con atributos y colores

void show();
//muestra el contenido de la pantalla

void curses();
//funcion donde se alojara todo el programa que usa curses


#endif /* CURSE_H */



