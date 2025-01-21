//MOTOR
int motor1PWM = 37; //left wheel - 1
int motor1Phase = 38;
int motor2PWM = 39; //right wheel - 2, slower
int motor2Phase = 20;

//OPTICAL SENSOR 
int AnalogueValue[5] = {0,0,0,0,0};
int AnaloguePin[5] = {5,4,6,7,15};

//THRESHOLDS
int BlackThreshold = 500;
int WhiteThreshold = 1000;

//CASES
bool BBWBB() { //On white line
  return (AnalogueValue[2] <= WhiteThreshold &&
          AnalogueValue[0] >= WhiteThreshold &&
          AnalogueValue[1] >= WhiteThreshold &&
          AnalogueValue[3] >= WhiteThreshold &&
          AnalogueValue[4] >= WhiteThreshold);
}

bool WWWWWW() { //all white
  return (AnalogueValue[2] <= WhiteThreshold &&
          AnalogueValue[0] <= WhiteThreshold &&
          AnalogueValue[1] <= WhiteThreshold &&
          AnalogueValue[3] <= WhiteThreshold &&
          AnalogueValue[4] <= WhiteThreshold);
}

bool BBBBB() { //all black
  return (AnalogueValue[2] >= WhiteThreshold &&
          AnalogueValue[0] >= WhiteThreshold &&
          AnalogueValue[1] >= WhiteThreshold &&
          AnalogueValue[3] >= WhiteThreshold &&
          AnalogueValue[4] >= WhiteThreshold);
}

bool WBBBB() { //white on first
  return (AnalogueValue[0] <= WhiteThreshold &&
          AnalogueValue[1] >= WhiteThreshold &&
          AnalogueValue[2] >= WhiteThreshold &&
          AnalogueValue[3] >= WhiteThreshold &&
          AnalogueValue[4] >= WhiteThreshold);
}

bool BWBBB() { //white on second
  return (AnalogueValue[0] >= WhiteThreshold &&
          AnalogueValue[1] <= WhiteThreshold &&
          AnalogueValue[2] >= WhiteThreshold &&
          AnalogueValue[3] >= WhiteThreshold &&
          AnalogueValue[4] >= WhiteThreshold);
}

bool BBBWB() { //white on fourth
  return (AnalogueValue[0] >= WhiteThreshold &&
          AnalogueValue[1] >= WhiteThreshold &&
          AnalogueValue[2] >= WhiteThreshold &&
          AnalogueValue[3] <= WhiteThreshold &&
          AnalogueValue[4] >= WhiteThreshold);
}

bool BBWWB() { //white on fourth
  return (AnalogueValue[0] >= WhiteThreshold &&
          AnalogueValue[1] >= WhiteThreshold &&
          AnalogueValue[2] <= WhiteThreshold &&
          AnalogueValue[3] <= WhiteThreshold &&
          AnalogueValue[4] >= WhiteThreshold);
}

bool BWWBB() { //white on fourth
  return (AnalogueValue[0] >= WhiteThreshold &&
          AnalogueValue[1] <= WhiteThreshold &&
          AnalogueValue[2] <= WhiteThreshold &&
          AnalogueValue[3] >= WhiteThreshold &&
          AnalogueValue[4] >= WhiteThreshold);
}

bool BBBBW() { //white on fifth
  return (AnalogueValue[0] >= WhiteThreshold &&
          AnalogueValue[1] >= WhiteThreshold &&
          AnalogueValue[2] >= WhiteThreshold &&
          AnalogueValue[3] >= WhiteThreshold &&
          AnalogueValue[4] <= WhiteThreshold);
}

bool WWBBB() { //white on fifth
  return (AnalogueValue[0] <= WhiteThreshold &&
          AnalogueValue[1] <= WhiteThreshold &&
          AnalogueValue[2] >= WhiteThreshold &&
          AnalogueValue[3] >= WhiteThreshold &&
          AnalogueValue[4] >= WhiteThreshold);
}

bool BBBWW() { //white on fifth
  return (AnalogueValue[0] >= WhiteThreshold &&
          AnalogueValue[1] >= WhiteThreshold &&
          AnalogueValue[2] >= WhiteThreshold &&
          AnalogueValue[3] <= WhiteThreshold &&
          AnalogueValue[4] <= WhiteThreshold);
}


bool BWWWW() { //white on fifth
  return (AnalogueValue[0] >= WhiteThreshold &&
          AnalogueValue[1] <= WhiteThreshold &&
          AnalogueValue[2] <= WhiteThreshold &&
          AnalogueValue[3] <= WhiteThreshold &&
          AnalogueValue[4] <= WhiteThreshold);
}

bool WWWWB() { //white on fifth
  return (AnalogueValue[0] <= WhiteThreshold &&
          AnalogueValue[1] <= WhiteThreshold &&
          AnalogueValue[2] <= WhiteThreshold &&
          AnalogueValue[3] <= WhiteThreshold &&
          AnalogueValue[4] >= WhiteThreshold);
}

bool WBBBW() { //white on fifth
  return (AnalogueValue[0] <= WhiteThreshold &&
          AnalogueValue[1] >= WhiteThreshold &&
          AnalogueValue[2] >= WhiteThreshold &&
          AnalogueValue[3] >= WhiteThreshold &&
          AnalogueValue[4] <= WhiteThreshold);
}

bool BWWWB() { //white on fifth
  return (AnalogueValue[0] >= WhiteThreshold &&
          AnalogueValue[1] <= WhiteThreshold &&
          AnalogueValue[2] <= WhiteThreshold &&
          AnalogueValue[3] <= WhiteThreshold &&
          AnalogueValue[4] >= WhiteThreshold);
}

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

  delay(200);
  if (BBWBB() || BWWWW() || WWWWB() || BWWBB() || BBWWB() || WBBBW() || BWWWB()) {
    GoForwards();
  } 
  else if (WWBBB() || WBBBB() || BWBBB()) {
    GoLeft_Fast();
  } 
  else if (BBBWW() || BBBBW() || BBBWB()) {
    GoRight_Fast();
  } 
  else if (WWWWWW()) {
    delay(200);
    GoForwards();
  }
  else if (BBBBB()) {
    GoLeft_Fast();
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

void GoForwards() {
  digitalWrite(motor1Phase, HIGH); //forward
  analogWrite(motor1PWM, 150); // set speed of motor
  digitalWrite(motor2Phase, HIGH); //forward
  analogWrite(motor2PWM, 140); // set speed of motor
}

void GoRight_Fast() {
  digitalWrite(motor1Phase, HIGH);
  analogWrite(motor1PWM, 50);
  digitalWrite(motor2Phase, HIGH);
  analogWrite(motor2PWM, 75);
}

void GoRight_Slow() {
  digitalWrite(motor1Phase, HIGH);
  analogWrite(motor1PWM, 30);
  digitalWrite(motor2Phase, HIGH);
  analogWrite(motor2PWM, 50);
}


void GoLeft_Fast() {
  digitalWrite(motor1Phase, HIGH);
  analogWrite(motor1PWM, 75);
  digitalWrite(motor2Phase, HIGH);
  analogWrite(motor2PWM, 50);
}

void GoLeft_Slow() {
  digitalWrite(motor1Phase, HIGH);
  analogWrite(motor1PWM, 50);
  digitalWrite(motor2Phase, HIGH);
  analogWrite(motor2PWM, 30);
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