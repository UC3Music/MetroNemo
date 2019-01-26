/*
 * MetroNemo
 * UC3Music: https://music.uc3m.es
 * Authors:
 * Pablo Lago (https://github.com/L4g0)
 * Tommy Guns (https://github.com/MrTommyGuns)
 *
 * An open-source arduino-based vibrating metronome.
 */
 
#include <AiEsp32RotaryEncoder.h> //Source: https://github.com/igorantolic/ai-esp32-rotary-encoder

/*
connecting Rotary encoder
CLK (A pin) - to any microcontroler intput pin with interrupt -> in this example pin 32
DT (B pin) - to any microcontroler intput pin with interrupt -> in this example pin 21
SW (button pin) - to any microcontroler intput pin -> in this example pin 25
VCC - to microcontroler VCC (then set ROTARY_ENCODER_VCC_PIN -1) or in this example pin 25
GND - to microcontroler GND
*/
#define ROTARY_ENCODER_A_PIN 19
#define ROTARY_ENCODER_B_PIN 21
#define ROTARY_ENCODER_BUTTON_PIN 18
#define ROTARY_ENCODER_VCC_PIN -1 /*put -1 of Rotary encoder Vcc is connected directly to 3,3V; else you can use declared output pin for powering rotary encoder */

#define LED 2 //Pin where the LED is attached (2 for internal)
#define BUZZ 5 //Pin for the buzzer
int bpm;
int interval;

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN);

void rotary_onButtonClick() {
  //rotaryEncoder.reset();
  //rotaryEncoder.disable();
  int16_t encoderValue = rotaryEncoder.readEncoder();
  bpm = encoderValue;
}

void rotary_loop() {
  //first lets handle rotary encoder button click
  if (rotaryEncoder.currentButtonState() == BUT_RELEASED) {
    //we can process it here or call separate function like:
    rotary_onButtonClick();
  }

  //lets see if anything changed
  int16_t encoderDelta = rotaryEncoder.encoderChanged();
  
  //optionally we can ignore whenever there is no change
  if (encoderDelta == 0) return;
  
  //for some cases we only want to know if value is increased or decreased (typically for menu items)
  //if (encoderDelta>0) Serial.print("+"); //Uncomment 4 debug
  //if (encoderDelta<0) Serial.print("-"); //Uncomment 4 debug

  //for other cases we want to know what is current value. Additionally often we only want if something changed
  //example: when using rotary encoder to set termostat temperature, or sound volume etc
  
  //if value is changed compared to our last read
  if (encoderDelta!=0) {
    //now we need current value
    int16_t encoderValue = rotaryEncoder.readEncoder();
    //process new value. Here is simple output.
    Serial.print("Value: ");
    Serial.print(encoderValue);
    Serial.println(" - Click to select");
    
  }
  
}

void setup() {
  bpm=120;
  Serial.begin(115200);
  Serial.println("Started\nInitial Tempo: 120 bpm");
  pinMode(LED,OUTPUT);
  pinMode(BUZZ,OUTPUT);
  //we must initialize rorary encoder 
  rotaryEncoder.begin();
  rotaryEncoder.setup([]{rotaryEncoder.readEncoder_ISR();});
  //optionally we can set boundaries and if values should cycle or not
  rotaryEncoder.setBoundaries(40, 208, false); //minValue, maxValue, cycle values (when max go to min and vice versa)
}

void loop() {
  //in loop call your custom function which will process rotary encoder values
  rotary_loop();
  
  interval = 60000/bpm;
  Serial.print("BPM: ");
  Serial.println(bpm);
  delay(interval-10);
  digitalWrite(LED,HIGH);
  digitalWrite(BUZZ,HIGH);
  delay(10);
  digitalWrite(LED,LOW);
  digitalWrite(BUZZ,LOW);

  //delay(50);                               
  if (millis()>20000) rotaryEncoder.enable ();
}
