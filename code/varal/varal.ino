// C++ code
// Varal Inteligente - Estacio - A.D.S.
// Alunos: Fernando Henrique, Guilherme Felix Maria Luiza Andrade

#define PRIMARY_MOTOR 3
#define PRIMARY_MOTOR_TO_OPEN 2
#define PRIMARY_MOTOR_TO_CLOSE 4
#define TIME_MILLISECOND 5000
#define WATER_SENSOR_SIGNAL A0

int speed = 255;
int valueWaterSensor = 0;
int offWaterSensor = 0;
bool flagOpenRoof = false;

void setup()
{
  pinMode(PRIMARY_MOTOR, OUTPUT);
  pinMode(PRIMARY_MOTOR_TO_OPEN, OUTPUT);
  pinMode(PRIMARY_MOTOR_TO_CLOSE, OUTPUT);
  pinMode(WATER_SENSOR_SIGNAL, INPUT);
  
  analogWrite(PRIMARY_MOTOR, 0);
  digitalWrite(PRIMARY_MOTOR_TO_OPEN, LOW);
  digitalWrite(PRIMARY_MOTOR_TO_CLOSE, LOW);
  
  Serial.begin(9600);
}

void loop()
{
  valueWaterSensor = analogRead(WATER_SENSOR_SIGNAL);
  
  if(valueWaterSensor > offWaterSensor && !flagOpenRoof){
  	motorToClose(PRIMARY_MOTOR, flagOpenRoof);
  	delay(1000);
  } else if (valueWaterSensor <= offWaterSensor && flagOpenRoof){
  	motorToOpen(PRIMARY_MOTOR, flagOpenRoof);
  }
  Serial.println("start");
  delay(1000);
}

void motorToOpen(int motor, bool &flag){
  Serial.println("entrei to open");
  analogWrite(motor, speed); // set speed
  
  /*set rotation (to open)*/
  digitalWrite(PRIMARY_MOTOR_TO_OPEN, HIGH);
  digitalWrite(PRIMARY_MOTOR_TO_CLOSE, LOW);
  
  /*Maybe change it to an infrared sensor*/
  delay(TIME_MILLISECOND); // operation time
  motorTurnOff(motor);
  
  flag = false;
}

void motorTurnOff(int motor){
  //turn off 
  analogWrite(motor, 0);
  digitalWrite(PRIMARY_MOTOR_TO_OPEN, LOW);
  digitalWrite(PRIMARY_MOTOR_TO_CLOSE, LOW);  
}

void motorToClose(int motor, bool &flag){
  Serial.println("entrei to close");
  analogWrite(motor, speed); // set speed
  
  /*set rotation (to close)*/
  digitalWrite(PRIMARY_MOTOR_TO_OPEN, LOW);
  digitalWrite(PRIMARY_MOTOR_TO_CLOSE, HIGH);
  
  /*Maybe change it to an infrared sensor*/
  delay(TIME_MILLISECOND); // operation time
  motorTurnOff(motor);
  
  flag = true;
  
}