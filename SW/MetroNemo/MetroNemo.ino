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
int bpmTap;                // bpm value
int bpmPot;                // bpm value
unsigned long time;     // Time value
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
  time = millis(); 
}

void loop() {
  
/************************************************************************************MODE 1**/
  if(mode == 1){  //Metronome using TAP TEMPO
    bpmTap = tapTempo.getBPM();  //Read the Tempo that you chose with the "TapTempo"

    //Metronome work
    if (millis()-time >= 6000/bpmTap)
    { 
       // digitalWrite(LED, LOW);
        noTone(BUZZER);
    }
     if (millis()-time >= (54000/bpmTap))
     {
        //digitalWrite(LED, HIGH);
        tone(BUZZER, 2000);
        Serial.print(bpmTap);
        Serial.println(" BPM Click!");
         
        time = millis(); // update current measure time 
    }
    
    TapTempoFunction();  //TAP Tempo
    
/************************************************************************************MODE 2**/ 
  }else if(mode == 2){  //Metronome using POTE
    bpmPot = map(analogRead(POT), 0, 1023, MIN_BPM, MAX_BPM);  //Read the Tempo that you choose with a Pot

    //Metronome work
    if (millis()-time >= 6000/bpmPot)
    { 
        digitalWrite(LED, LOW);
        noTone(BUZZER);
    }
     if (millis()-time >= (54000/bpmPot))
     {
        digitalWrite(LED, HIGH);
        tone(BUZZER, 2000);
        Serial.print(bpmPot);
        Serial.println(" BPM Click!");
         
        time = millis(); // update current measure time 
    }
    
/************************************************************************************MODE 0**/
  }else{  //Metronome STOP and Config TAP TEMPO
    TapTempoFunction();
    time = millis();
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

void TapTempoFunction(){ //TAP Tempo
  
    but_tap_status = digitalRead(TAP) == LOW; // get the state of the button
    tapTempo.update(but_tap_status); // update ArduinoTapTempo
    if(digitalRead(TAP) == HIGH){
      //Serial.println("TAP!");
      tone(BUZZER, 1000);
    }else{
      noTone(BUZZER);
    }
}

void Check(){ //Check if the button is pressed
  but_status = !but_status; // toggle but_status
  Button();
}

