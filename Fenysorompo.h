#ifndef Fenysorompo_h
#define Fenysorompo_h

#include "Arduino.h"
#include "Defines.h"

class Fenysorompo { // osztály a fénysorompónak
public:
    int id; // lámpa egyedi id-je
    int feher_pin; //arduino pinek a ledekhez, a konstruktorban kell megadni, később a pin_valtoztat függvénnyel lehet módosítani, ha kell futásidőben
    int piros_1_pin;
    int piros_2_pin;

    int feher = villog_lassu, piros_1 = ki, piros_2 = ki;

    unsigned long utolso_kapcsolas_feher = 0; // az utolsó kapcsolás ideje lassú villogás esetén, ez és az "idotartam" alapján dől el, hogy kell-e váltani a LED állapotát
    unsigned long idotartam_lassu = 700; // 1s-ig van ki- vagy bekapcsolva

    unsigned long utolso_kapcsolas_piros_1 = 0; // az utolsó kapcsolás ideje gyors villogás esetén, ez és az "idotartam" alapján dől el, hogy kell-e váltani a LED állapotát
    unsigned long utolso_kapcsolas_piros_2 = 0; // az utolsó kapcsolás ideje gyors villogás esetén, ez és az "idotartam" alapján dől el, hogy kell-e váltani a LED állapotát
    unsigned long idotartam_gyors = 500; // 0.5s-ig van ki- vagy bekapcsolva

    Fenysorompo(int id, int feher_pin=22, int piros_1_pin=24, int piros_2_pin=26) { // konstruktor a Lampa_fenysorompo osztalyhoz, a kimenetek pinjeit itt rendeli hozzá
        this->feher_pin=feher_pin;
        this->piros_1_pin=piros_1_pin;
        this->piros_2_pin=piros_2_pin;
    }
    void init(void){
        pinMode(feher_pin,OUTPUT);
        pinMode(piros_1_pin,OUTPUT);
        pinMode(piros_2_pin,OUTPUT);
        
        //digitalWrite(feher_pin,HIGH);
        //digitalWrite(piros_1_pin,HIGH);
        //digitalWrite(piros_2_pin,HIGH);
    }
    void pin_valtoztat(int zold_pin, int sarga_1_pin, int sarga_2_pin, int piros_pin) {
        this->feher_pin = feher_pin;
        this->piros_1_pin = piros_1_pin;
        this->piros_2_pin = piros_2_pin;
    }

    void tilos(void) { // 
        feher=ki;
        piros_1=villog_gyors;
        digitalWrite(piros_1_pin, LOW);
        piros_2=villog_gyors;
    }

    void szabad(void){
      feher=villog_lassu;
      piros_1=ki;
      piros_2=ki;
    }

    void lampa_update(void) {
        // az összes LED-re meghívódik a led_kapcsol fv, ami eldőnti az adott LED állapota alapján, hogy fel, vagy le kell-e kapcsolni
        led_kapcsol(feher, feher_pin, &utolso_kapcsolas_feher); // minden LED-nek külön idő van pointerként megadva, így csak akkor változtat az állapotán, ha az utolsó villanás óta eltelt "idotartam" idő.
                                                                // Ezt most lehetne egyszerűbben, de ha több ledet kell egyszerre villogtatni különböző gyorsasággal egy időben, akkor hasznos
        led_kapcsol(piros_1, piros_1_pin, &utolso_kapcsolas_piros_1);
        led_kapcsol(piros_2, piros_2_pin, &utolso_kapcsolas_piros_2);
    }

    void led_kapcsol(int led, int led_pin, unsigned long *utolso_kapcsolas) {
        // ez dönti el, hogy milyen értékre (LED állapotra) milyen output írás történjen, villogás logikája itt van
        if (led == be) {
            digitalWrite(led_pin, LOW);
        }
        else if (led == ki) {
            digitalWrite(led_pin, HIGH);
        }
        else if (led == villog_lassu) {
            if ((*utolso_kapcsolas)+idotartam_lassu<=millis()) { // ha eltelt az utolsó kapcsolás óta "idotartam_lassu" idő, akkor kapcsoljon a LED
                *utolso_kapcsolas = millis();
                if (digitalRead(led_pin)==LOW) { // ha LOW volt, vagyis fel volt kapcsolva, akkor legyen HIGH, vagyis legyen felkapcsolva
                    digitalWrite(led_pin, HIGH);
                }
                else {
                    digitalWrite(led_pin, LOW);
                }
            }

        }
        else if (led == villog_gyors) {
            if ((*utolso_kapcsolas)+idotartam_gyors<=millis()) { // ha eltelt az utolsó kapcsolás óta "idotartam_gyors" idő, akkor kapcsoljon a LED
                *utolso_kapcsolas = millis();
                if (digitalRead(led_pin)==LOW) { // ha LOW volt, vagyis fel volt kapcsolva, akkor legyen HIGH, vagyis legyen lekapcsolva
                    digitalWrite(led_pin, HIGH);
                }
                else {
                    digitalWrite(led_pin, LOW);
                }
            }

        }
    }
};

#endif
