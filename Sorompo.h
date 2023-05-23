#ifndef Sorompo_h
#define Sorompo_h

#include "Arduino.h"
#include "Defines.h"
#include <Wire.h>
#include <Servo.h>

class Sorompo { // osztály a Sorompónak
public:
    Servo szervo_1;  // szervo objektumok a szervók irányításához
    Servo szervo_2;
    
    int cel=lent; //0: lent, 1: fent
    float jelenleg=lent; // 0 és 1 közötti szám, 0 jelenti azt, hogy lent van a sorompó, 1 azt, hogy fent
    int lent_1, lent_2, fent_1, fent_2;

    int id;
    int szervo_1_pin; // arduino pin az egyik sorompó szervonak
    int szervo_2_pin; // arduino pin a másik sorompó szervonak

    unsigned long utolso_lepes=0; // az utolsó lépés időpillanata
    unsigned long idotartam=15; // (ms) ennyi időnként módosítja a szervo állását

    Sorompo(int id, int pin_1, int pin_2, int lent_1, int fent_1, int lent_2, int fent_2) { // konstruktor a Hall osztályhoz, egyedi id és pin szám a szervóknak
        this->id=id;
        this->szervo_1_pin=pin_1;
        this->szervo_2_pin=pin_2;
        this->lent_1=lent_1;
        this->lent_2=lent_2;
        this->fent_1=fent_1;
        this->fent_2=fent_2;
    }

    void init(){
        
        szervo_1.attach(szervo_1_pin);  // a megfelelő pinekhez csatolja aszervo objektumokat
        szervo_2.attach(szervo_2_pin);  
        szervo_1.write(lent_1);
        szervo_2.write(lent_2);
    }

    void pin_valtoztat(int pin_1, int pin_2) {
        szervo_1_pin = pin_1;
        szervo_2_pin = pin_2;
    }

    void set_cel(int cel){
        this->cel=cel;
    }

    void szabad(void){
      cel=fent;
    }

    void tilos(void){
      cel=lent;
    }

    void sorompo_update(void) {
        // fokozatosan juttatja el a szervot a célállapotba, idotartam időket várva a lépések között. Ha a jelenlegi állapot mindkét szervo esetében megegyezik a céllal, akkor nem változtat
        if (idotartam+utolso_lepes<=millis() && cel!=jelenleg){
            if (cel==fent && jelenleg<cel){ // ha a sorompókat fel akarjuk emelni, és még nincsenek fent
                jelenleg+=lepeskoz;
            }
            else if (cel==lent && jelenleg>cel){ // ha a sorompókat le akarjuk ereszteni, és még nincsenek lent
                jelenleg-=lepeskoz;
            }
            szervo_1.write(int(lent_1+jelenleg*(fent_1-lent_1)));
            szervo_2.write(int(lent_2+jelenleg*(fent_2-lent_2)));
            utolso_lepes=millis();
        }
    }
};

#endif
