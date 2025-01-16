//MOTOR
int motor1PWM = 37; //left wheel - 1
int motor1Phase = 38;
int motor2PWM = 39; //right wheel - 2
int motor2Phase = 20;

//OPTICAL SENSOR 
int AnalogValue[5] = {0,0,0,0,0};
int AnalogPin[5] = {5,4,6,7,15};

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  //MOTOR
  pinMode(motor1PWM, OUTPUT);
  pinMode(motor2PWM, OUTPUT);
  pinMode(motor1Phase, OUTPUT);
  pinMode(motor2Phase, OUTPUT);

  //OPTICAL SENSOR
  int i;
  for(i=0; i<5; i++) {
    pinMode(AnalogPin[i], INPUT);
  }
}

// the loop routine runs over and over again continuously:
void loop() {
//MOTOR CODE:
  /*GoForward();
  delay(2000); //2 seconds
  GoBackwards();
  delay(2000); //2 secondS
  */

//OPTICAL SENSOR TEST:
  int i;
  for (i=0;i<5;i++)
  {
  AnalogValue[i]=analogRead(AnalogPin[i]);
  Serial.print(AnalogValue[i]); // This prints the actual analog reading from the sensors
  Serial.print("\t"); //tab over on screen
  if(i==4)
      {
        Serial.println(""); //carriage return
        delay(100); // display new set of readings every 600mS
      }
  }

  GoForward();
  delay(2000);
  GoRight();
  delay(2000);
  GoLeft();
  delay(2000);
  GoBackwards();
  delay(2000);
  Stop();
  delay(2000);
}

void GoForward() {
  digitalWrite(motor1Phase, HIGH); //forward
  analogWrite(motor1PWM, 100); // set speed of motor
  Serial.println("Forward"); // Display motor direction
  digitalWrite(motor2Phase, HIGH); //forward
  analogWrite(motor2PWM, 100); // set speed of motor
  Serial.println("Forward"); // Display motor direction
}

void GoRight() {
  digitalWrite(motor1Phase, LOW); //forward
  analogWrite(motor1PWM, 100); // set speed of motor
  digitalWrite(motor2Phase, HIGH); //forward
  analogWrite(motor2PWM, 100); // set speed of motor
}

void GoLeft() {
  digitalWrite(motor1Phase, HIGH); //forward
  analogWrite(motor1PWM, 100); // set speed of motor
  digitalWrite(motor2Phase, LOW); //forward
  analogWrite(motor2PWM, 100); // set speed of motor
}

void GoBackwards() {
  digitalWrite(motor1Phase, LOW); //Backward
  analogWrite(motor1PWM, 100); // set speed of motor
  Serial.println("Backward"); // Display motor direction 
  digitalWrite(motor2Phase, LOW); //Backward
  analogWrite(motor2PWM, 100); // set speed of motor
  Serial.println("Backward"); // Display motor direction
}

void Stop() {
  //delay(100000000000000000);
  analogWrite(motor1PWM, LOW); 
  analogWrite(motor2PWM, LOW); 
}
