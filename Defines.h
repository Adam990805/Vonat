#ifndef Defines_h
#define Defines_h

#include "Arduino.h" 

// sebességek definiálása
#define seb_0   0b000  // 0
#define seb_40  0b001  // 1
#define seb_80  0b010  // 2
#define seb_120 0b011  // 3
#define seb_max 0b100  // 4

// lámpa állapotok definiálása
#define ki                0
#define be                1
#define villog_lassu      2
#define villog_gyors      3

// sorompó állapotok definiálása
#define fent        1
#define lent        0
#define lepeskoz    0.03
#define lepesszam   35

#endif
