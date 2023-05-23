#ifndef Alakjelzo_h
#define Alakjelzo_h

#include "Arduino.h"
#include "Defines.h"
#include <Servo.h>

class Alakjelzo { // osztály a Hall szenzoroknak
public:
    int id;
    Servo szervo_1;
    int cel; //0: lent, 1: fent
    //float jelenleg=lent_1; 
    float jelenleg;
    int tilos_poz, szabad_poz;
    float lepeskoz_1;

    int szervo_1_pin; // arduino pin az alakjelző szervonak
    int zold_pin;
    int sarga_pin;
    int piros_pin;

    unsigned long utolso_lepes=0; // az utolsó lépés időpillanata
    unsigned long idotartam=15; // (ms) ennyi időnként módosítja a szervo állását
    //ha szabad: zöld és sárga világít, alak: //
    //ha tilos: csak a piros, alak: |__

    Alakjelzo(int id, int szervo_pin, int zold_pin, int sarga_pin, int piros_pin, int tilos_poz, int szabad_poz) { // konstruktor a Hall osztályhoz, egyedi id és pin szám
        this->id=id;
        this->szervo_1_pin=szervo_pin;
        this->zold_pin=zold_pin;
        this->sarga_pin=sarga_pin;
        this->piros_pin=piros_pin;
        this->tilos_poz=tilos_poz;
        this->szabad_poz=szabad_poz;
        jelenleg=szabad_poz;
        cel=szabad_poz;
        
    }

    void init(void){
        pinMode(zold_pin,OUTPUT);
        pinMode(sarga_pin,OUTPUT);
        pinMode(piros_pin,OUTPUT);
        szervo_1.attach(szervo_1_pin);  // a megfelelő pinekhez csatolja a szervo objektumokat
        szervo_1.write(szabad_poz);
    }

    /*void pin_valtoztat(int pin) {
        Hall_pin = pin;
    }*/

    void set_fel(void){
          //this->cel=fent_1;
          //this->lepeskoz_1=(fent_1-lent_1)/lepesszam;
    }

    void set_le(void){
          //this->cel=szabad_poz;
          //this->lepeskoz_1=(lent_1-fent_1)/lepesszam;
    }
    
    void szabad(void){
      digitalWrite(zold_pin,HIGH);
      digitalWrite(sarga_pin,HIGH);
      digitalWrite(piros_pin,LOW);
      cel=szabad_poz;
      lepeskoz_1=float(szabad_poz-tilos_poz)/lepesszam;
    }

    void tilos(void){
      digitalWrite(zold_pin,LOW);
      digitalWrite(sarga_pin,LOW);
      digitalWrite(piros_pin,HIGH);
      cel=tilos_poz;
      lepeskoz_1=float(tilos_poz-szabad_poz)/lepesszam;
    }

    void alakjelzo_update(void) {
        // fokozatosan juttatja el a szervot a célállapotba, idotartam időket várva a lépések között. Ha a jelenlegi állapot mindkét szervo esetében megegyezik a céllal, akkor nem változtat
        
        if (idotartam+utolso_lepes<=millis()){
            if ((cel-jelenleg)*lepeskoz_1>=0){
                jelenleg+=lepeskoz_1;
                szervo_1.write(int(jelenleg));
                utolso_lepes=millis();
            }
            else{
                szervo_1.write(int(jelenleg-10*lepeskoz_1));
            }
        }
    }
    
};

#endif
