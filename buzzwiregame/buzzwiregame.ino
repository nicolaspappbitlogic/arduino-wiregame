#include <pitches.h>

// music
int songState = 0;

int melody[] = {
 NOTE_F4, NOTE_E4, NOTE_D4, NOTE_CS4,
 NOTE_C4, NOTE_B3, NOTE_AS3, NOTE_A3,
 NOTE_G3, NOTE_A3, NOTE_AS3, NOTE_A3, 
 NOTE_G3, NOTE_C4, 0, 
 
 NOTE_C4, NOTE_A3, NOTE_A3, NOTE_A3,
 NOTE_GS3, NOTE_A3, NOTE_F4, NOTE_C4, 
 NOTE_C4, NOTE_A3, NOTE_AS3, NOTE_AS3, 
 NOTE_AS3, NOTE_C4, NOTE_D4, 0, 
 
 NOTE_AS3, NOTE_G3, NOTE_G3, NOTE_G3,
 NOTE_FS3, NOTE_G3, NOTE_E4, NOTE_D4, 
 NOTE_D4, NOTE_AS3, NOTE_A3, NOTE_A3, 
 NOTE_A3, NOTE_AS3, NOTE_C4, 0,
 
 NOTE_C4, NOTE_A3, NOTE_A3, NOTE_A3,
 NOTE_GS3, NOTE_A3, NOTE_A4, NOTE_F4, 
 NOTE_F4, NOTE_C4, NOTE_B3, NOTE_G4, 
 NOTE_G4, NOTE_G4, NOTE_G4, 0,
 
 NOTE_G4, NOTE_E4, NOTE_G4, NOTE_G4,
 NOTE_FS4, NOTE_G4, NOTE_D4, NOTE_G4, 
 NOTE_G4, NOTE_FS4, NOTE_G4, NOTE_C4, 
 NOTE_B3, NOTE_C4, NOTE_B3, NOTE_C4, 0
};

int tempo[] = {
 8, 16, 8, 16,
 8, 16, 8, 16,
 16, 16, 16, 8,
 16, 8, 3,
 
 12, 16, 16, 16,
 8, 16, 8, 16,
 8, 16, 8, 16,
 8, 16, 4, 12,
 
 12, 16, 16, 16,
 8, 16, 8, 16,
 8, 16, 8, 16,
 8, 16, 4, 12,
 
 12, 16, 16, 16,
 8, 16, 8, 16,
 8, 16, 8, 16,
 8, 16, 4, 16,
 
 12, 17, 17, 17,
 8, 12, 17, 17, 
 17, 8, 16, 8,
 16, 8, 16, 8, 1 
};


// non blocking setup
// free play
unsigned long previousMillis1 = 0; // time words last changed
const long interval1 = 1500; // interval between changing

// music
unsigned long previousMillis2 = 0; // time last changed
const long interval2 = 100; // interval between notes

int displayStatus = 0; // keep track of what's displayed
int mode = 0; // keep track of game mode -- change to 0 or 1 for different modes

bool countdown = false;

unsigned long previousMillis3 = 0; // time last changed
const long interval3 = 1000; // interval between countdown
int count = 40; // challenge mode timer

int numeroIntentos = 0;
long int tiempoUltimoIntento = 0;
const int maxTiempoEspera = 1000;

const int SERIAL_BAUD_RATE = 9600;

void setup() { 
   // put your setup code here, to run once:
   Serial.begin(SERIAL_BAUD_RATE);
   pinMode(18, INPUT); // setup circuit
   pinMode(10, OUTPUT); // setup buzzer 1
   pinMode(2, INPUT); // setup button
   pinMode(3, OUTPUT); // setup led
   digitalWrite(3, HIGH);
   attachInterrupt(digitalPinToInterrupt(18), aumentarIntentos, RISING);
   Serial.println("Inicializando Sistema");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(mode == 0) {
    // challenge mode
    if(digitalRead(2) == HIGH) {
      delay(25);
      if(digitalRead(2) == HIGH) {
        if(countdown == false){
          Serial.println("Empieza el conteo! Tienes " + String(count) + " segundos!");  
        }
        else{
          // Finalizando juego
          count = 1;
        }
        countdown = true; // stop the countdown
      }
      else {
        countdown = false; // stop the countdown
      }
    } 
    if(countdown) {
      showCountdown(); // advance countdown
    } 
  }
  else {
    // free play
    toggleFreePlay(); 
  }
  
  if(countdown){
    sing();
  }
}

void aumentarIntentos() {
  unsigned long tiempoActual = millis(); // current time
  if(countdown && (tiempoActual - tiempoUltimoIntento) > maxTiempoEspera) {
    digitalWrite(3, LOW);
    buzz(10, NOTE_B0, 1000/2);
    digitalWrite(3, HIGH);
    numeroIntentos++;
    Serial.println("Has tocado el circuito! Intentos: " + String(numeroIntentos));
    //Serial.println("Ultimo: " + String(tiempoUltimoIntento));
    //Serial.println("Actual: " + String(tiempoActual));
    tiempoUltimoIntento = tiempoActual;
  }
}

void showCountdown() {
  // countdown the time remaining
  unsigned long currentMillis = millis(); // current time
  if (currentMillis - previousMillis3 >= interval3) {
    previousMillis3 = currentMillis;
    --count;
    Serial.println("Le quedan: " + String(count) + " segundos.");
    //showNumber(count);
    if(count == 0) {
      // game over
      Serial.println("Juego terminado! Tocaste el circuito: " + String(numeroIntentos) + " veces");
      countdown = false;
      count = 40;
      numeroIntentos = 0;
      // reset countdown
      // buzz 3 times
      buzz(10, NOTE_B0, 1000/24); 
      delay(100);
      buzz(10, NOTE_B0, 1000/24);
      delay(100);
      buzz(10, NOTE_B0, 1000/24);
    }
  }
}

void toggleFreePlay() {
   // scroll between words without blocking
   unsigned long currentMillis = millis(); // current time
   if (currentMillis - previousMillis1 >= interval1) {
    previousMillis1 = currentMillis;
    /*if(displayStatus == 1) 
      showPlay();
    else
      showFree();*/
   }
}

/*void showPlay() {
   // write "PLAY" to the display
   _display->display(0, 21); // write P to segment 1
   _display->display(1, 18); // write L to segment 2
   _display->display(2, 10); // write A to segment 3
   _display->display(3, 4); // write Y to segment 4
   displayStatus = 2;
}*/

/*void showFree() {
   // write "Free" to the display
   _display->display(0, 15); // write F to segment 1
   _display->display(1, 23); // write r to segment 2
   _display->display(2, 14); // write E to segment 3
   _display->display(3, 14); // write E to segment 4
   displayStatus = 1;
}*/

void buzz(int targetPin, long frequency, long length) {
   /* Buzzer example function by Rob Faludi
   http://www.faludi.com
   https://gist.github.com/AnthonyDiGirolamo/1405180 
   */
   long delayValue = 1000000/frequency/2; // calculate the delay value between transitions
   //// 1 second's worth of microseconds, divided by the frequency, then split in half since
   //// there are two phases to each cycle
   long numCycles = frequency * length/ 1000; // calculate the number of cycles for proper timing
   //// multiply frequency, which is really cycles per second, by the number of seconds to
   //// get the total number of cycles to produce
   for (long i=0; i < numCycles; i++){ // for the calculated length of time...
   digitalWrite(targetPin,HIGH); // write the buzzer pin high to push out the diaphragm
   delayMicroseconds(delayValue); // wait for the calculated delay value
   digitalWrite(targetPin,LOW); // write the buzzer pin low to pull back the diaphragm
   delayMicroseconds(delayValue); // wait again for the calculated delay value
   }
}


void sing() {
   // play the song in a non blocking way
   unsigned long currentMillis = millis();
  
   if (currentMillis - previousMillis2 >= interval2) {
    previousMillis2 = currentMillis;
    int noteDuration = 1000 / tempo[songState];
    buzz(10, melody[songState], noteDuration);
    int pauseBetweenNotes = noteDuration;
    delay(pauseBetweenNotes);
  
    // stop the tone playing:
    buzz(10, 0, noteDuration);
   
    ++songState;
    // start song again if finished
    if(songState > 79) {
      songState = 14; // skip intro 
    } 
   }
}
