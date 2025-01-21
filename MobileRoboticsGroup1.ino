//MOTOR
int motor1PWM = 37; //left wheel - 1
int motor1Phase = 38;
int motor2PWM = 39; //right wheel - 2
int motor2Phase = 20;

//OPTICAL SENSOR 
int AnalogueValue[5] = {0,0,0,0,0};
int AnaloguePin[5] = {5,4,6,7,15};

//THRESHOLDS

int Threshold = 750;

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

   //state identifier based on sensor readings
  bool state = 0b00000;
  for (int i = 0; i < 5; i++) {
    if (AnalogueValue[i] >= Threshold) {
      state |= (1 << i); // Set the bit if the sensor detects black
    }
    Serial.print(state,BIN);
  }

  //cases for truth table
  switch (state) {
    case 0b00000: //all sensors white
      GoForwards();
      break;
    case 0b11111: //all sensors black
      GoLeft();
      delay(500);
      GoForwards();
      delay(500);
      break;
    case 0b11000: //leftmost sensors detect black
      GoRight();
      break;
    case 0b10000:
      GoRight();
      break;
    case 0b00011: //rightmost sensors detect black
      GoLeft();
      break;
    case 0b00001:
      GoLeft();
      break;
    case 0b00100: //center sensor detects black
      GoForwards();
      break;
    case 0b11101: //center sensor detects black
      GoRight();
      break;
    case 0b10111: //center sensor detects black
      GoLeft();
      break;
    default: //undefined state
      Stop();
      break;
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