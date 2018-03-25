/*
 * MetroGnome
 * UC3Music: https://music.uc3m.es
 * Author: Pablo Lago (https://github.com/L4g0)
 * 
 * An open-source arduino-based metronome.
 */ 


#define LED     8       // LED pin
#define BUZZER  10      // Buzzer pin
#define MIN_BPM 20      // minimum bpm value
#define MAX_BPM 240     // maximum bpm value
#define POT A0          // pot analog pin

int bpm;


void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);  // LED pin as output
}

void loop() {
  bpm = map(analogRead(POT), 0, 1023, MIN_BPM, MAX_BPM);
  Serial.print(bpm);
  Serial.print(" BPM");
  digitalWrite(LED, HIGH);
  tone(BUZZER, 2000);
  Serial.print("click");
  delay(6000/bpm);        // 10% of T (where T is the time between two BPSs)
  digitalWrite(LED, LOW);
  noTone(BUZZER);
  delay(54000 / bpm);     // 90% of T

}
