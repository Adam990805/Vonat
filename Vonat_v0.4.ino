//importok
#include <Wire.h>
#include <Servo.h>
#include "Defines.h"

#include "Lampa_4_led.h"
//#include "Lampa_7_led.h"
#include "Lampa_fenysorompo.h"

#include "Hall.h"
#include "Sorompo.h"
#include "Fenysorompo.h"
#include "Alakjelzo.h"

#define I2C_ADDRESS 0x04

//globális változók

Lampa_4_led lampa_1(1,22,24,26,28); // 1-es lámpa id, PIN22: zöld, PIN24:felső sárga, PIN26: piros, PIN28: alsó sárga
Hall hall_1(1, 44); // 1-es Hall id, PIN44: Hall-szenzor pinje
Hall hall_2(2, 46); // 2-es Hall id, PIN46: Hall-szenzor pinje
Hall hall_3(3, 48); // 3-es Hall id, PIN48: Hall-szenzor pinje
Hall hall_4(4, 50); // 4-es Hall id, PIN50: Hall-szenzor pinje
Sorompo sorompo_1(1,40,42, 0, 18, 114, 155); // 1-es sorompó id, PIN40: egyik oldali sorompó szervo pin, PIN42: másik oldali sorompó szervo pin, lent_1, fent_1, lent_2, fent_2
Fenysorompo fenysorompo_1(1,45,47,49); // 1-es fénysorompó id, PIN44: fehér LED pin, PIN46: piros_1 LED pin, PIN48: piros_2 LED pin
Alakjelzo alakjelzo_1(1, 23, 27, 29, 31, 26, 5); //id, szervó pin, zöld pin, sárga pin, piros pin, szervo értéke tilos állásban, szervo értéke szabad állásban


// Hall teszteléshez:
int van=0; // van: mágnest közelítettünk
int van_old=0;

unsigned long utolso_mozgas; //sorompó időzítői
unsigned long varakozas=5000;

void setup(){
  Serial.begin(9600);
  Wire.begin(I2C_ADDRESS);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  //initek mindenképp kellenek (pinMode, digitalWrite)
  lampa_1.init();
  hall_1.init();
  hall_2.init();
  hall_3.init();
  hall_4.init();
  sorompo_1.init();
  fenysorompo_1.init();
  alakjelzo_1.init();

  lampa_1.lampa_valtoztat(); // sebességek alapján a ledek állapotát kapcsolja, csak egyszer kell meghívni, amikor a sebességek változnak
  

}

void loop(){
    /*van=!digitalRead(hall_1.Hall_pin);
    if (van && !van_old){
      Serial.print("VONAT!\n");
      sorompo_1.set_cel(lent);
      fenysorompo_1.tilos();
    }
    if(!van && van_old){
      sorompo_1.set_cel(fent);
      fenysorompo_1.szabad();
    }
    van_old=van;*/

    if (sorompo_1.jelenleg>=fent && varakozas+utolso_mozgas<=millis()){
      sorompo_1.tilos();
      alakjelzo_1.szabad();
      fenysorompo_1.tilos();
      utolso_mozgas=millis();
    }
    if (sorompo_1.jelenleg<=lent && varakozas+utolso_mozgas<=millis()){
      sorompo_1.szabad();
      alakjelzo_1.tilos();
      fenysorompo_1.szabad();
      utolso_mozgas=millis();
    }

    lampa_1.lampa_update(); // update akkor is kell, ha nincs változás a villogás kezelése miatt
    sorompo_1.sorompo_update(); // ha kel mozgatni a sorompót, akkor mozgatja a célpozíció felé
    fenysorompo_1.lampa_update(); // fénysorompó villogtatása
    alakjelzo_1.alakjelzo_update(); //alakjelző mozgatása, ha kell
}

/*void receiveEvent(int howMany){
  while(1 < Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
}*/

void receiveEvent(int howMany){ // i2c adat beolvasása és lámpa változtatása az alapján (pl üzenet: 1,4,4: az 1-es lámpa utáni sebesség max, utána is max)
  int lampa, utana_szakasz_1, utana_szakasz_2;
  char elv, buffer[50];
  while(4 < Wire.available()) // loop through if five bytes have arrived
  {
    Wire.read();
    Serial.print("\nA lampa id_je: ");
    lampa = Wire.read()-48;
    Serial.print(lampa);
    elv = Wire.read(); // receive byte as a character
    utana_szakasz_1 = Wire.read()-48;
    Serial.print("\nAz 1-es szakasz sebessege: ");
    Serial.print(utana_szakasz_1);
    elv = Wire.read();
    utana_szakasz_2 = Wire.read()-48;
    Serial.print("\nA 2-es szakasz sebessege: ");
    Serial.println(utana_szakasz_2);
  }
  //sprintf(buffer, "\n\nAdat erkezett\nLampa id: ",lampa,"\nszakasz_1 sebesseg: ", utana_szakasz_1,"\nszakasz_2 sebesseg: ",  utana_szakasz_2);
  //Serial.print(buffer);         // print the integer
  /*Serial.print("\n Kuldott 1. seb: ");
  Serial.print(utana_szakasz_1);
  Serial.print("\n Kuldott 2. seb: ");
  Serial.println(utana_szakasz_2);*/
  lampa_1.sebesseg_valtoztat(utana_szakasz_1, utana_szakasz_2);
  Serial.print("\n Valtoztatott 1. seb: ");
  Serial.print(lampa_1.utana_szakasz_1_seb);
  Serial.print("\n Valtoztatott 2. seb: ");
  Serial.println(lampa_1.utana_szakasz_2_seb);
  lampa_1.lampa_valtoztat();
}

void requestEvent(){
  hall_1.hall_send();
  hall_2.hall_send();
  hall_3.hall_send();
  hall_4.hall_send();

  //ha nincs jel a hall szenzoron, átírjuk 0-ra
  hall_1.hall_reset();
  hall_2.hall_reset();
  hall_3.hall_reset();
  hall_4.hall_reset();
  //Serial.println();
  //Serial.println("Kuldve");
}
