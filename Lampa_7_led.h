#ifndef Lampa_7_led_h
#define Lampa_7_led_h

#include "Arduino.h"
#include "Defines.h"

class Lampa_7_led { // osztály a 4 lámpás jelzőnek
public:
    int id; // lámpa egyedi id-je
    int zold_pin; //arduino pinek a ledekhez, a konstruktorban kell megadni, később a pin_valtoztat függvénnyel lehet módosítani, ha kell futásidőben
    int sarga_1_pin;
    int piros_pin;
    int sarga_2_pin;

    int led_szam=7; // ennyi led van a lámpán
    int led_lista[led_szam];
    int * pin_lista[led_szam];
    
    int utana_szakasz_1; // utana_szakasz_1: közvetlen a lámpa utáni szakasz száma (id-je)
    int utana_szakasz_2; // utana_szakasz_2: a lámpa utáni szakaszt követő szakasz száma (id-je)

    int utana_szakasz_1_seb = seb_0; // utana_szakasz_1_seb: közvetlen a lámpa utáni szakasz megengedett sebessége
    int utana_szakasz_2_seb = seb_0; // utana_szakasz_2_seb: a lámpa utáni szakaszt követő szakasz megengedett sebessége

    int zold = ki, sarga_1 = ki, piros = ki, sarga_2 = ki; // felülről lefelé zold, sarga_1, piros, sarga_2 a 4 lámpán lévő LED állapota

    unsigned long utolso_kapcsolas = 0; // az utolsó kapcsolás ideje, ez és az "idotartam" alapján dől el, hogy kell-e váltani a LED állapotát
    unsigned long idotartam = 1000; // 1s-ig van ki- vagy bekapcsolva

    Lampa_7_led(int id, int zold=22, int sarga_1=24, int piros=26, int sarga_2=28) { // konstruktor a Lampa_4_led osztalyhoz, a kimenetek pinjeit itt rendeli hozzá
        zold_pin=zold;
        sarga_1_pin=sarga_1;
        piros_pin=piros;
        sarga_2_pin=sarga_2;
        
        pin_lista[0]=zold;
        pin_lista[1]=sarga_1;
        pin_lista[2]=piros;
        pin_lista[3]=sarga_2;
        pin_lista[4]=feher;
        pin_lista[5]=zold;
        pin_lista[6]=zold;
    }
    void init(void){
        for (int i = 0; i<led_szam; i++){
            pinMode(pin_lista[i],OUTPUT);
            digitalWrite(pin_lista[i],HIGH);
        }
        
    }
    void pin_valtoztat(int zold_pin, int sarga_1_pin, int sarga_2_pin, int piros_pin) {
        this->zold_pin = zold_pin;
        this->sarga_1_pin = sarga_1_pin;
        this->piros_pin = piros_pin;
        this->sarga_2_pin = sarga_2_pin;
    }
    void sebesseg_valtoztat(int seb_1, int seb_2) {
        this->utana_szakasz_1_seb = seb_1;
        this->utana_szakasz_2_seb = seb_2;
    }

    void lampa_valtoztat(void) {
        // logika, ami a sebességből lámpaszíneket hoz létre, megj: ha nincs keigészítő jelzés, nagyon sok lámpajelzés ugyanaz, mivel csak a kieg jelzés tesz különbséget közöttük
        if (utana_szakasz_1_seb == seb_max) {
            if (utana_szakasz_2_seb == seb_max) zold = be, sarga_1 = ki, piros = ki, sarga_2 = ki; // max sebesség, következő jelzőn max, 15. ábra
            if (utana_szakasz_2_seb == seb_120) zold = ki, sarga_1 = be, piros = ki, sarga_2 = ki; // max sebesség, következő jelzőn 120, 16. ábra
            if (utana_szakasz_2_seb == seb_80) zold = villog_lassu, sarga_1 = ki, piros = ki, sarga_2 = ki; // max sebesség, következő jelzőn 80, 17. ábra
            if (utana_szakasz_2_seb == seb_40) zold = ki, sarga_1 = villog_lassu, piros = ki, sarga_2 = ki; // max sebesség, következő jelzőn 40, 18. ábra
            if (utana_szakasz_2_seb == seb_0) zold = ki, sarga_1 = be, piros = ki, sarga_2 = ki; // max sebesség, következő jelzőn megállj, 19. ábra
        }
        if (utana_szakasz_1_seb == seb_40) {
            if (utana_szakasz_2_seb == seb_max) zold = be, sarga_1 = ki, piros = ki, sarga_2 = be; // 40, következő jelzőn max, 20. ábra, ugyanaz, mint a 25. ábra kiegészítő jelzés nélkül
            if (utana_szakasz_2_seb == seb_120) zold = ki, sarga_1 = be, piros = ki, sarga_2 = be; // 40, következő jelzőn 120, 21. ábra, ugyanaz, mint a 24. ábra kiegészítő jelzés nélkül, ugyanaz, mint a 26. ábra kiegészítő jelzés nélkül
            if (utana_szakasz_2_seb == seb_80) zold = villog_lassu, sarga_1 = ki, piros = ki, sarga_2 = be; // 40, következő jelzőn 80, 22. ábra
            if (utana_szakasz_2_seb == seb_40) zold = ki, sarga_1 = villog_lassu, piros = ki, sarga_2 = be; // 40, következő jelzőn 40, 23. ábra
            if (utana_szakasz_2_seb == seb_0) zold = ki, sarga_1 = be, piros = ki, sarga_2 = be; // 40, következő jelzőn megállj, 24. ábra, ugyanaz, mint a 21. ábra kiegészítő jelzés nélkül
        }
        if (utana_szakasz_1_seb == seb_120) {
            if (utana_szakasz_2_seb == seb_max) zold = be, sarga_1 = ki, piros = ki, sarga_2 = be; // 120, következő jelzőn max, 25. ábra, ugyanaz, mint a 20. ábra kiegészítő jelzés nélkül
            if (utana_szakasz_2_seb == seb_120) zold = ki, sarga_1 = be, piros = ki, sarga_2 = be; // 120, következő jelzőn 120, 26. ábra, ugyanaz, mint a 21. ábra kiegészítő jelzés nélkül
            if (utana_szakasz_2_seb == seb_80) zold = ki, sarga_1 = be, piros = ki, sarga_2 = be; // 120, következő jelzőn 80, 27. ábra
            if (utana_szakasz_2_seb == seb_40) zold = ki, sarga_1 = be, piros = ki, sarga_2 = be; // 120, következő jelzőn 40, 28. ábra
            if (utana_szakasz_2_seb == seb_0) zold = ki, sarga_1 = be, piros = ki, sarga_2 = be; // 120, következő jelzőn megállj, 29. ábra
        }
        if (utana_szakasz_1_seb == seb_80) {
            if (utana_szakasz_2_seb == seb_max) zold = be, sarga_1 = ki, piros = ki, sarga_2 = be; // 80, következő jelzőn max, 30. ábra
            if (utana_szakasz_2_seb == seb_120) zold = ki, sarga_1 = be, piros = ki, sarga_2 = be; // 80, következő jelzőn 120, 31. ábra
            if (utana_szakasz_2_seb == seb_80) zold = ki, sarga_1 = be, piros = ki, sarga_2 = be; // 80, következő jelzőn 80, 32. ábra
            if (utana_szakasz_2_seb == seb_40) zold = ki, sarga_1 = be, piros = ki, sarga_2 = be; // 80, következő jelzőn 40, 33. ábra
            if (utana_szakasz_2_seb == seb_0) zold = ki, sarga_1 = be, piros = ki, sarga_2 = be; // 80, következő jelzőn megállj, 34. ábra
        }
        if (utana_szakasz_1_seb == seb_0) {
            zold = ki, sarga_1 = ki, piros = be, sarga_2 = ki; // megállj jelzés, 35. ábra
        }
    }

    void lampa_update(void) {
        // az összes LED-re meghívódik a led_kapcsol fv, ami eldönti az adott LED állapota alapján, hogy fel, vagy le kell-e kapcsolni
        led_kapcsol(zold, zold_pin);
        led_kapcsol(sarga_1, sarga_1_pin);
        led_kapcsol(piros, piros_pin);
        led_kapcsol(sarga_2, sarga_2_pin);
    }

    void led_kapcsol(int led, int led_pin) {
        // ez dönti el, hogy milyen értékre (LED állapotra) milyen output írás történjen, villogás logikája itt van
        if (led == be) {
            digitalWrite(led_pin, LOW);
        }
        else if (led == ki) {
            digitalWrite(led_pin, HIGH);
        }
        else if (led == villog_lassu) {
            if (utolso_kapcsolas+idotartam<=millis()) { // ha eltelt az utolsó kapcsolás óta "idotartam" idő, akkor kapcsoljon a LED
                utolso_kapcsolas = millis();
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
