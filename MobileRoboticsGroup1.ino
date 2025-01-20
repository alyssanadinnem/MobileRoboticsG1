//MOTOR
int motor1PWM = 37; //left wheel - 1
int motor1Phase = 38;
int motor2PWM = 39; //right wheel - 2
int motor2Phase = 20;

//OPTICAL SENSOR 
int AnalogueValue[5] = {0,0,0,0,0};
int AnaloguePin[5] = {5,4,6,7,15};

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
    pinMode(AnaloguePin[i], INPUT);
  }
}

// the loop routine runs over and over again continuously:
void loop() {

  OpticalTest();
  GoForwards();
  if (AnalogueValue[2] <= 500 && AnalogueValue[0] >= 1000 && AnalogueValue[1] >= 1000 && AnalogueValue[3] >= 1000 && AnalogueValue[4] >= 1000) {
    GoForwards();
    delay(10);
  }
  else if (AnalogueValue[1] <= 500 && AnalogueValue[0] >= 1000 && AnalogueValue[2] >= 1000 && AnalogueValue[3] >= 1000 && AnalogueValue[4] >= 1000) {
    GoLeft();
    delay(10);
  }
  else if (AnalogueValue[3] <= 500 && AnalogueValue[0] >= 1000 && AnalogueValue[2] >= 1000 && AnalogueValue[1] >= 1000 && AnalogueValue[4] >= 1000) {
    GoRight();
    delay(10);
  }

}

void OpticalTest() {
  int i;
  for (i=0;i<5;i++)
  {
  AnalogueValue[i]=analogRead(AnaloguePin[i]);
  Serial.print(AnalogueValue[i]); // This prints the actual analog reading from the sensors
  Serial.print("\t"); //tab over on screen
  if(i==4)
      {
        Serial.println(""); //carriage return
        delay(100); // display new set of readings every 600mS
      }
  }
}

void GoForwards() {
  digitalWrite(motor1Phase, HIGH); //forward
  analogWrite(motor1PWM, 100); // set speed of motor
  digitalWrite(motor2Phase, HIGH); //forward
  analogWrite(motor2PWM, 100); // set speed of motor
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
  digitalWrite(motor2Phase, LOW); //Backward
  analogWrite(motor2PWM, 100); // set speed of motor
}

void Stop() {
  //delay(100000000000000000);
  analogWrite(motor1PWM, LOW); 
  analogWrite(motor2PWM, LOW); 
}
