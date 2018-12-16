#define LED 2
#define BUZZ 25
int bpm;
int interval;
void setup() {
  bpm=120;
  Serial.begin(9600);
  Serial.println("Started\nInitial Tempo: 120 bpm");
  pinMode(LED,OUTPUT);
  pinMode(BUZZ,OUTPUT);
}

void loop() {
//  if (Serial.read() > 0){
//    bpm=Serial.read();
//  }
  interval = 60000/bpm;
  Serial.println(bpm);
  delay(interval-10);
  digitalWrite(LED,HIGH);
  digitalWrite(BUZZ,HIGH);
  delay(10);
  digitalWrite(LED,LOW);
  digitalWrite(BUZZ,LOW);
}
