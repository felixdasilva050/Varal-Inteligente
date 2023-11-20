/* C++ code
* Varal Inteligente - Estacio do Recife- A.D.S.
* Students: Guilherme Felix, Fernando Henrique, Maria Luiza Andrade
* Author: Guilherme Felix da Silva
* Contact: guilhermefelix.ofwork@gmail.com
*/

//LIBRARIES
#include <ESP8266WiFi.h>   // if not 
#include <PubSubClient.h> //library PubSubClient by Nick O'Leary
#include <Stepper.h>
extern "C"{
  #include "user_interface.h" 
}

//GATES
#define e1 D0
#define e2 D1
#define e3 D2
#define e4 D3
#define WATER_SENSOR_SIGNAL A0
#define ROOF_SENSOR_OPEN D8
#define ROOF_SENSOR_CLOSE D7

// WiFi CONFIG
//credentials
#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define API_KEY ""
#define SERVER ""

bool flag=false

os_timer_t mTimer;
bool _timeout = false;

WiFiClient client;

int VALUE_WATER_SENSOR = 0;
const int OFF_WATER_SENSOR = 200;

//STEPPER MOTOR VARIABLES
const int STEPS_PER_SPIN = 64; //see how many steps your motor needs to complete one rotation and change if necessary
const int ROTATION_SPEED=500; //change if necessary. Higher values may not work very well. check your engine before changing
const int CYCLES = 1285;

//STEPPER MOTOR CONFIG
Stepper mp(STEPS_PER_SPIN, e1,e3,e2,e4);

void setup(){
  pinMode(WATER_SENSOR_SIGNAL, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  
  mp.setSpeed(ROTATION_SPEED);

  initWiFi();
  usrInit(); 
  // PINS TO STOP ROOF
  // pinMode(ROOF_SENSOR_OPEN, INPUT);
  // pinMode(ROOF_SENSOR_CLOSE, INPUT);
}

void loop(){
  VALUE_WATER_SENSOR = analogRead(WATER_SENSOR_SIGNAL);
  digitalWrite(LED_BUILTIN, HIGH);
  if(VALUE_WATER_SENSOR <= OFF_WATER_SENSOR){
    Serial.println("IF");
  	motorToOpen();
  } else if (VALUE_WATER_SENSOR > OFF_WATER_SENSOR){
    Serial.println("ELSE");
    motorToClose();
  }
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  yield();
}

void motorToOpen(){
  for(int i = 0; i<CYCLES;i++){
    if(VALUE_WATER_SENSOR <= OFF_WATER_SENSOR){
      mp.step(STEPS_PER_SPIN);
      Serial.print(i);
      Serial.print(", ");
    }else{
      break;
    }
  }
}

void motorToClose(){
  for(int i = 0; i<CYCLES;i++){
    if (VALUE_WATER_SENSOR > OFF_WATER_SENSOR){
      mp.step(-STEPS_PER_SPIN);
      Serial.print(i);
      Serial.print(", ");
    }else{
      break;
    }
  }
}

bool verifyRoofOpenLimit(){
  int valueSensor = digitalRead(ROOF_SENSOR_OPEN);
  return valueSensor > 0;
}

bool verifyRoofCloseLimit(){
  int valueSensor = digitalRead(ROOF_SENSOR_CLOSE);
  return valueSensor > 0;
}

// WiFi METHODS
void tCallback(void *tCall){
    _timeout = true;
}

void usrInit(void) {
    os_timer_setfn(&mTimer, tCallback, NULL); //Indicates the subroutine to the timer.
    os_timer_arm(&mTimer, 10000, true); //Indicates to the Timer the time in ms and whether it will be repeated or just once (loop = true)
}

void initWiFi() {
  Serial.println("entrei");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}