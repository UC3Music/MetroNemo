/*
 * MetroNemo
 * UC3Music: https://music.uc3m.es
 * Authors:
 * Pablo Lago (https://github.com/L4g0)
 * Tommy Guns (https://github.com/MrTommyGuns)
 *
 * An open-source arduino-based metronome.
 */
#include <Bounce2.h> 
#include <ArduinoTapTempo.h> //TapTempo library

#define LED     8       // LED pin
#define BUZZER  10      // Buzzer pin
#define MIN_BPM 40      // minimum bpm value
#define MAX_BPM 240     // maximum bpm value
#define POT A0          // pot analog pin
#define MOD 2           // PIN for the MOD Button
#define TAP 5           // PIN for the TapTempo

ArduinoTapTempo tapTempo;     //Arduino TapTempo Object
Bounce debouncer = Bounce();  //Arduino Bounce Object

bool but_status = 0;    // start or stop variable
bool but_tap_status = 0;// TapTempo button status
int mode = 0;           // Set 
int bpm;                // bpm value
int value;
int aux;
void setup() {
  Serial.begin(9600);

  Serial.println("Started\nIncicial Tempo 120 bpm\nTap to set a Tempo or press the Mode Button");
  
  pinMode(LED, OUTPUT);  // LED pin as output

  pinMode(MOD,INPUT);
  debouncer.attach(MOD);
  debouncer.interval(10); // interval in ms
  
  attachInterrupt(digitalPinToInterrupt(MOD), Check, FALLING);  // ISR on pin 2 (INT0) when there is a falling edge
  
  pinMode(TAP, INPUT); //Conf pin for Tap Tempo button
  digitalWrite(TAP, HIGH); //Conf pin for Tap Tempo button
}

void loop() {
  if(mode == 1){  //Metronome using TAP TEMPO
    bpm = tapTempo.getBPM();  //Read the Tempo with the "TapTempo"
    //Serial.println(tapTempo.getBPM());
    Tempo();
  }else if(mode == 2){  //Metronome using POTE
    bpm = map(analogRead(POT), 0, 1023, MIN_BPM, MAX_BPM);  //Read the Tempo with a Pot
    Serial.print("Tap bpm: ");  //Test the asigned Tempo
    Tempo();
  }else{  //Metronome STOP and Config TAP TEMPO
    but_tap_status = digitalRead(TAP) == LOW; // get the state of the button
    tapTempo.update(but_tap_status); // update ArduinoTapTempo
    if(digitalRead(TAP) == HIGH){
      //Serial.println("TAP!");
      tone(BUZZER, 1000);
    }else{
      noTone(BUZZER);
    }
  }/*
  if(digitalRead(MOD)==HIGH){
    Serial.println("IF");
    Button();
  }*/
}

void Button(){
  
  while(digitalRead(MOD)== HIGH)/*Serial.println("While")*/;
  mode = mode+1;
  if(mode > 2) mode = 0;
  //delay(100);                  // waits 100 milliseconds
  if(mode == 0){         // Stop And Tap Tempo Mode
    Serial.println("Stop\nTap a Tempo!");   // prints on serial monitor "Stop"
    noTone(BUZZER);           // stops generation of sound
  }else if(mode == 1){
    Serial.println("MODE: Tap");  // prints on serial monitor   
  }else{
    Serial.println("MODE: Pote");  // prints on serial monitor
  }
}

void Tempo(){
    digitalWrite(LED, HIGH);
    tone(BUZZER, 2000);
    Serial.print(bpm);
    Serial.println(" BPM Click!");
    delay(6000/bpm);        // 10% of T (where T is the time between two BPSs)
    digitalWrite(LED, LOW);
    noTone(BUZZER);
    delay(54000 / bpm);     // 90% of T
}

void Check(){
  but_status = !but_status; // toggle but_status
  Button();
}

