#ifndef Hall_h
#define Hall_h

#include "Arduino.h"
#include "Defines.h"
#include <Wire.h>

class Hall { // osztály a Hall szenzoroknak
public:
    int id;
    int Hall_pin; //arduino pin a Hall szenzornak
    int aktiv=0;

    Hall(int id, int pin) { // konstruktor a Hall osztályhoz, egyedi id és pin szám
        this->id=id;
        this->Hall_pin=pin;
    }

    void init(void){
        pinMode(Hall_pin,INPUT_PULLUP);
    }

    void pin_valtoztat(int pin) {
        Hall_pin = pin;
    }

    int hall_read(void){
      aktiv=(aktiv || !digitalRead(Hall_pin));
    }
    
    void hall_send(void){
      Wire.write(aktiv);
    }

    void hall_reset(void){
      aktiv=(0 || !digitalRead(Hall_pin));
    }
    
};

#endif
