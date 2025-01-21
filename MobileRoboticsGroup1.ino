//MOTOR
int motor1PWM = 37; //left wheel - 1
int motor1Phase = 38;
int motor2PWM = 39; //right wheel - 2
int motor2Phase = 20;

//OPTICAL SENSOR 
int AnalogueValue[5] = {0,0,0,0,0};
int AnaloguePin[5] = {5,4,6,7,15};

//THRESHOLDS
int BlackThreshold = 500;
int WhiteThreshold = 1000;

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

  if (BBWBB()) {
    GoForwards();
  } 
  else if (AnalogueValue[1] <= WhiteThreshold || AnalogueValue[0] <= WhiteThreshold) {
    GoRight();
  } 
  else if (AnalogueValue[3] <= WhiteThreshold || AnalogueValue[4] <= WhiteThreshold) {
    GoLeft();
  } 
  else if (WWWWWW()) {
    delay(100);
    GoForwards();
  }
  else if (BBBBB()) {
    GoLeft();
  }
  else {
    Stop();
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

bool BBWBB() {
  return (AnalogueValue[2] <= WhiteThreshold &&
          AnalogueValue[0] >= WhiteThreshold &&
          AnalogueValue[1] >= WhiteThreshold &&
          AnalogueValue[3] >= WhiteThreshold &&
          AnalogueValue[4] >= WhiteThreshold);
}

bool WWWWWW() {
  return (AnalogueValue[2] <= WhiteThreshold &&
          AnalogueValue[0] <= WhiteThreshold &&
          AnalogueValue[1] <= WhiteThreshold &&
          AnalogueValue[3] <= WhiteThreshold &&
          AnalogueValue[4] <= WhiteThreshold);
}

bool BBBBB() {
  return (AnalogueValue[2] >= WhiteThreshold &&
          AnalogueValue[0] >= WhiteThreshold &&
          AnalogueValue[1] >= WhiteThreshold &&
          AnalogueValue[3] >= WhiteThreshold &&
          AnalogueValue[4] >= WhiteThreshold);
}

bool WBBBB() {
  return (AnalogueValue[0] <= WhiteThreshold &&
          AnalogueValue[1] >= WhiteThreshold &&
          AnalogueValue[2] >= WhiteThreshold &&
          AnalogueValue[3] >= WhiteThreshold &&
          AnalogueValue[4] >= WhiteThreshold);
}

void GoForwards() {
  digitalWrite(motor1Phase, HIGH); //forward
  analogWrite(motor1PWM, 150); // set speed of motor
  digitalWrite(motor2Phase, HIGH); //forward
  analogWrite(motor2PWM, 150); // set speed of motor
}

void GoLeft() {
  digitalWrite(motor1Phase, LOW);
  analogWrite(motor1PWM, 50);
  digitalWrite(motor2Phase, HIGH);
  analogWrite(motor2PWM, 50);
}

void GoRight() {
  digitalWrite(motor1Phase, HIGH);
  analogWrite(motor1PWM, 50);
  digitalWrite(motor2Phase, LOW);
  analogWrite(motor2PWM, 50);
}

void GoBackwards() {
  digitalWrite(motor1Phase, LOW);
  analogWrite(motor1PWM, 100);
  digitalWrite(motor2Phase, LOW);
  analogWrite(motor2PWM, 100);
}

void Stop() {
  //delay(100000000000000000);
  analogWrite(motor1PWM, 0); 
  analogWrite(motor2PWM, 0); 
}
/*
void calc_turn() {
  error_value = constrain(error_value, -256, 256);

  if (error_value < 0) {
    right_speed = max_speed + error_value;
    left_speed = max_speed;
  } else {
    right_speed = max_speed;
    left_speed = max_speed - error_value;
  }
}*/