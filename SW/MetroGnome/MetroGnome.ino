/*
 * MetroGnome
 * UC3Music: https://music.uc3m.es
 * Author: Pablo Lago (https://github.com/L4g0)
 * 
 * An open-source arduino-based metronome.
 */ 
#include <ArduinoTapTempo.h> //TapTempo library

#define LED     8       // LED pin
#define BUZZER  10      // Buzzer pin
#define MIN_BPM 40      // minimum bpm value
#define MAX_BPM 240     // maximum bpm value
#define POT A0          // pot analog pin
#define TAP 5           //PIN for the TapTempo

bool but_status = 0;    // start or stop variable
int bpm;                //bpm value

ArduinoTapTempo tapTempo; //Arduino TapTempo Object

void setup() {
  Serial.begin(9600);
  
  pinMode(LED, OUTPUT);  // LED pin as output
  
  attachInterrupt(0, Button, FALLING);  // ISR on pin 2 (INT0) when there is a falling edge
  
  pinMode(TAP, INPUT); //Conf button for TapTempo
  digitalWrite(TAP, HIGH);
}

void loop() {
  if(but_status ==1){
    //bpm = map(analogRead(POT), 0, 1023, MIN_BPM, MAX_BPM);  //Read the Tempo with a Pot
    
    bpm = tapTempo.getBPM();  //Read the Tempo with the "TapTempo"
    //Serial.print("Tap bpm: ");  //Test the asigned Tempo
    //Serial.println(tapTempo.getBPM());
    
    digitalWrite(LED, HIGH);
    tone(BUZZER, 2000);
    Serial.print(bpm);
    Serial.println(" BPM Click!");
    delay(6000/bpm);        // 10% of T (where T is the time between two BPSs)
    digitalWrite(LED, LOW);
    noTone(BUZZER);
    delay(54000 / bpm);     // 90% of T
  }else{
    boolean buttonDown = digitalRead(TAP) == LOW; // get the state of the button
    tapTempo.update(buttonDown); // update ArduinoTapTempo
  }
}

void Button()
{
  but_status = !but_status;   // toggle but_status
  delay(10);                  // waits 10 milliseconds
  if(but_status == 0)         // if but_status is 0...
  {
    Serial.println("Stop");   // prints on serial monitor "Stop"
    noTone(BUZZER);           // stops generation of sound
  }    
  else
    Serial.println("Started at:");  // prints on serial monitor
}
