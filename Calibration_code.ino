//MOTOR
int motor1PWM = 37; //left wheel - 1
int motor1Phase = 38;
int motor2PWM = 39; //right wheel - 2, slower
int motor2Phase = 20;

//OPTICAL SENSOR 
int AnalogueValue[5] = {0,0,0,0,0};
int AnaloguePin[5] = {5,4,6,7,15};

//THRESHOLDS
int min = 1000000;
int max = 1;
int Threshold = 0;

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
  for(int a=0; a++; a<=1000){
  OpticalTest();
  Threshold = ((max-min)/2)
  Serial.print(Threshold);
  Serial.print("\t");
  Left(150, 150)
  delay(10);
  }
}

void OpticalTest() {
  int i;
  for (i=0;i<5;i++)
  {
  AnalogueValue[i]=analogRead(AnaloguePin[i]);
  if(AnalogueValue[i]<=min){
    AnalogueValue[i]=min;
  }
  if(AnalogueValue[i]>=max){
    AnalogueValue[i]=max;
  }
  //Serial.print(AnalogueValue[i]); // This prints the actual analog reading from the sensors
  //Serial.print("\t"); //tab over on screen
  if(i==4)
      {
        //Serial.println(""); //carriage return
        delay(100); // display new set of readings every 600mS
      }
  }
}


void Left(int turn_right, int turn_left) {
  digitalWrite(motor1Phase, LOW);
  analogWrite(motor1PWM, turn_right);
  digitalWrite(motor2Phase, HIGH);
  analogWrite(motor2PWM, turn_left);
}
