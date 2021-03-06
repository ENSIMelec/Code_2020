#include <FlexiTimer2.h>


/**
 * Test encodeur
 * 
 * Amélioration: 
 * Rajoute de digitalWriteFast pour permettre la lecture rapide lors de l'interpution (digitalWrite() 6280nS  vs  digitalWriteFast() 125nS.)
 *  permet d’améliorer la rapidité de lecture et d’écriture des entrées digitales pour minimiser le temps passé dans la routine d’interruption
 */

//mapping codeurs droit
const int PULSE_DROIT = 3;
const int DIR_DROIT = 8;

//mapping codeurs gauche
const int PULSE_GAUCHE = 2;
const int DIR_GAUCHE = 7;

volatile long temps; // déclaration variable volatile = stockée en RAM

volatile long nbTicD; // nb tick codeur droit
volatile long nbTicG; // nb tick codeur gauche

#define CANDENCE_MS_LOG 2000    // 2s

void setup() {
  Serial.begin(115200);
  
  pinMode(PULSE_DROIT, INPUT);
  pinMode(DIR_DROIT, INPUT);
  
  pinMode(PULSE_GAUCHE, INPUT);
  pinMode(DIR_GAUCHE, INPUT);

  //int
  attachInterrupt(digitalPinToInterrupt(PULSE_DROIT), countDroit, RISING);
  attachInterrupt(digitalPinToInterrupt(PULSE_GAUCHE), countGauche, RISING);

  reset();

//  Logger debug purpose
  //FlexiTimer2::set(CANDENCE_MS_LOG, 1/1000., loggerDebug);  // rés 1ms);
  //FlexiTimer2::start();
}

void loop() {
  
  raspSerial();
}
// appel à cette fonction quand y a une pulse (interpution sur la broche 0 (D2)

// On regarde l'état de la direction lorsqu'il y a des impulsations, 
//  quand l'état de la direction est au niveau HAUT on décrémente le tick (sens inverse), 
//  et dans le cas contraire on inrémente

void countDroit(void)   {
  if (digitalRead(DIR_DROIT) == LOW) nbTicD--;
    else nbTicD++;
}
void countGauche(void)   {
  if (digitalRead(DIR_GAUCHE) == LOW) nbTicG++;
    else nbTicG--;
}


// reset
void reset() {
  nbTicG = 0;
  nbTicD = 0;
  temps = micros();
}

// debugger
void loggerDebug() {
  Serial.println("===== DEBUG LOG =====");
  temps = micros();
  Serial.print("temps = "); 
  Serial.println(temps);
  Serial.print("tick droit = ");
  Serial.println(nbTicD);
  Serial.print("tick gauche  = ");
  Serial.println(nbTicG);
}

// récupérer des données par la rasp
void raspSerial() {
 if (Serial.available())  
  { 
    int data = Serial.read(); 
    switch (data) 
    { 
        case 'C' : 
          temps = micros();
          Serial.print("?"); 
          Serial.print(nbTicG); 
          Serial.print(","); 
          Serial.print(nbTicD); 
          Serial.print(":"); 
          Serial.print(temps); 
          Serial.print(";"); 
          break; 
        case 'R' : 
           reset(); 
           break; 
     }    
  } 
}
