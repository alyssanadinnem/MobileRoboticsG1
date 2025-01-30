//MOTOR
int motor1PWM = 37; //left wheel - 1
int motor1Phase = 38;
int motor2PWM = 39; //right wheel - 2, slower
int motor2Phase = 20;

//OPTICAL SENSOR 
int AnalogueValue[6] = {0,0,0,0,0,0};
int AnaloguePin[6] = {5,4,6,7,15,17};

//THRESHOLDS
int WhiteThreshold = 2000;

//LEFT OR RIGHT
int left_or_right = 0;

//SPEED VARIABLES
int straight_l = 110;
int straight_r = 105;

int sharp_right_motor_r = 135;
int sharp_right_motor_l = 0;

int sharp_left_motor_r = 0;
int sharp_left_motor_l = 135;
;

int straighten_left_r = 115;
int straighten_left_l = 135;

int straighten_right_r = 135;
int straighten_right_l = 115;

int tank_turn = 135;

//DISTANCE
int dist = 0;

//ROUTE
int route[] = {0, 6, 1, 7, 3};
int previousPosition = 4;
int currentPosition = 0;
int nextPosition = 6;
int action = 0;
int a = 0;
/*straight=0
  left=1
  right=2
  180+straight=3*/

//JUST AT CHECKPOINT OR JUNCTION
//int checkpoint = 0;
//int junction = 0;

//JUST DONE SHARP OR STRAIGHTEN
//int sharp = 0;
//int straighten = 1;

//FUNCTION DECLARATIONS
void OpticalTest();
void Distancetest();
void GoForwards();
void Left(int turn_right, int turn_left);
void Right(int turn_right, int turn_left);
void TankLeft(int turn_right, int turn_left);
void TankRight(int turn_right, int turn_left);
void GoBackwards();
void Stop();

//CASES
bool BBWBB() { //On white line
  return (AnalogueValue[2] <= WhiteThreshold &&
          AnalogueValue[0] >= WhiteThreshold &&
          AnalogueValue[1] >= WhiteThreshold &&
          AnalogueValue[3] >= WhiteThreshold &&
          AnalogueValue[4] >= WhiteThreshold);
}

bool WWWWW() { //all white
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

bool WWWBB() { //white on fifth
  return (AnalogueValue[0] <= WhiteThreshold &&
          AnalogueValue[1] <= WhiteThreshold &&
          AnalogueValue[2] <= WhiteThreshold &&
          AnalogueValue[3] >= WhiteThreshold &&
          AnalogueValue[4] >= WhiteThreshold);
}

bool BBWWW() { //white on fifth
  return (AnalogueValue[0] >= WhiteThreshold &&
          AnalogueValue[1] >= WhiteThreshold &&
          AnalogueValue[2] <= WhiteThreshold &&
          AnalogueValue[3] <= WhiteThreshold &&
          AnalogueValue[4] <= WhiteThreshold);
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
  Distancetest();
  delay(1);

  currentPosition = route[a];
  nextPosition = route[a+1];
  
  // Determine the action based on current, next, and previous positions
  switch (currentPosition) {
    case 0:
      Serial.println("Current Position: 0");
      switch (nextPosition) {
        case 4:
          Serial.println("Next Position: 4");
          switch (previousPosition) {
            case 4:
              Serial.println("Previous Position: 4");
              Serial.println("Action: Do a 180 and go straight to node 0");
              action = 3;
              break;
            default:
              Serial.println("Action: Go straight to node 4");
              action = 0;
              break;
          }
          break;
        case 6:
          Serial.println("Next Position: 6");
          switch (previousPosition) {
            case 6:
              Serial.println("Previous Position: 6");
              Serial.println("Action: Do a 180 and go straight to node 0");
              action = 3;
              break;
            default:
              Serial.println("Action: Go straight to node 6");
              action = 0;
              break;
          }
          break;
        default:
          Serial.println("Invalid next position for current node 0");
          break;
      }
      break;

    case 1:
      Serial.println("Current Position: 1");
      switch (nextPosition) {
        case 6:
          Serial.println("Next Position: 6");
          switch (previousPosition) {
            case 6:
              Serial.println("Previous Position: 6");
              Serial.println("Action: Do a 180 and go straight to node 6");
              action = 3;
              break;
            default:
              Serial.println("Action: Go straight to node 6");
              action = 0;
              break;
          }
          break;
        case 7:
          Serial.println("Next Position: 7");
          switch (previousPosition) {
            case 7:
              Serial.println("Previous Position: 7");
              Serial.println("Action: Do a 180 and go straight to node 7");
              action = 3;
              break;
            default:
              Serial.println("Action: Go straight to node 7");
              action = 0;
              break;
          }
          break;
        default:
          Serial.println("Invalid next position for current node 1");
          break;
      }
      break;
      
    case 2:
      Serial.println("Current Position: 2");
      switch (nextPosition) {
        case 3:
          Serial.println("Next Position: 3");
          switch (previousPosition) {
            case 3:
              Serial.println("Previous Position: 3");
              Serial.println("Action: Do a 180 and go straight to node 3");
              action = 3;
              break;
            default:
              Serial.println("Action: Go straight to node 3");
              action = 0;
              break;
          }
          break;
        case 6:
          Serial.println("Next Position: 6");
          switch (previousPosition) {
            case 6:
              Serial.println("Previous Position: 6");
              Serial.println("Action: Do a 180 and go straight to node 6");
              action = 3;
              break;
            default:
              Serial.println("Action: Go straight to node 6");
              action = 0;
              break;
          }
          break;
        default:
          Serial.println("Invalid next position for current node 2");
          break;
      }
      break;

    case 3:
      Serial.println("Current Position: 3");
      switch (nextPosition) {
        case 2:
          Serial.println("Next Position: 2");
          switch (previousPosition) {
            case 2:
              Serial.println("Previous Position: 2");
              Serial.println("Action: Do a 180 and go straight to node 2");
              action = 3;
              break;
            default:
              Serial.println("Action: Go straight to node 2");
              action = 0;
              break;
          }
          break;
        case 7:
          Serial.println("Next Position: 7");
          switch (previousPosition) {
            case 7:
              Serial.println("Previous Position: 7");
              Serial.println("Action: Do a 180 and go straight to node 7");
              action = 3;
              break;
            default:
              Serial.println("Action: Go straight to node 7");
              action = 0;
              break;
          }
          break;
        default:
          Serial.println("Invalid next position for current node 3");
          break;
      }
      break;

    case 4:
      Serial.println("Current Position: 4");
      switch (nextPosition) {
        case 0:
          Serial.println("Next Position: 0");
          switch (previousPosition) {
            case 0:
              Serial.println("Previous Position: 0");
              Serial.println("Action: Do a 180 and go straight to node 0");
              action = 3;
              break;
            default:
              Serial.println("Action: Go straight to node 0");
              action = 0;
              break;
          }
          break;
        case 7:
          Serial.println("Next Position: 7");
          switch (previousPosition) {
            case 7:
              Serial.println("Previous Position: 7");
              Serial.println("Action: Do a 180 and go straight to node 7");
              action = 3;
              break;
            default:
              Serial.println("Action: Go straight to node 7");
              action = 0;
              break;
          }
          break;
        default:
          Serial.println("Invalid next position for current node 4");
          break;
      }
      break;

    case 5:
      Serial.println("Current Position: 5");
      switch (nextPosition) {
        case 7:
          Serial.println("Next Position: 7");
          switch (previousPosition) {
            case 7:
              Serial.println("Previous Position: 7");
              Serial.println("Action: Do a 180 and go straight to node 7");
              action = 3;
              break;
            default:
              Serial.println("Previous Position: 7");
              Serial.println("Action: Do a 180 and go straight to node 7");
              action = 3;
              break;
          }
          break;
      }
      break;

    case 6:
      Serial.println("Current Position: 6");
      switch (nextPosition) {
        case 0:
          Serial.println("Next Position: 0");
          switch (previousPosition) {
            case 0:
              Serial.println("Previous Position: 0");
              Serial.println("Action: Do a 180 and go straight to node 0");
              action = 3;
              break;
            case 1:
              Serial.println("Previous Position: 1");
              Serial.println("Action: Turn right");
              action = 2;
              break;
            default:
              Serial.println("Action: Go straight to node 0");
              action = 0;
              break;
          }
          break;
        case 1:
          Serial.println("Next Position: 1");
          switch (previousPosition) {
            case 1:
              Serial.println("Previous Position: 1");
              Serial.println("Action: Do a 180 and go straight to node 1");
              action = 3;
              break;
            case 0:
              Serial.println("Previous Position: 0");
              Serial.println("Action: Turn left");
              action = 1;
              break;
            default:
              Serial.println("Action: Turn right");
              action = 2;
              break;
          }
          break;
        case 2:
          Serial.println("Next Position: 2");
          switch (previousPosition) {
            case 2:
              Serial.println("Previous Position: 2");
              Serial.println("Action: Do a 180 and go straight to node 2");
              action = 3;
              break;
            case 1:
              Serial.println("Previous Position: 1");
              Serial.println("Action: Turn left");
              action = 1;
              break;
            default:
              Serial.println("Action: Go straight");
              action = 0;
              break;
          }
          break;
        default:
          Serial.println("Invalid next position for current node 6");
          break;
      }
      break;

    case 7:
      Serial.println("Current Position: 7");
      switch (nextPosition) {
        case 1:
          Serial.println("Next Position: 1");
          switch (previousPosition) {
            case 1:
              Serial.println("Previous Position: 1");
              Serial.println("Action: Do a 180 and go straight to node 1");
              action = 3;
              break;
            case 3:
              Serial.println("Previous Position: 3");
              Serial.println("Action: Turn left");
              action = 1;
              break;
            case 4:
              Serial.println("Previous Position: 4");
              Serial.println("Action: Turn right");
              action = 2;
              break;
            default:
              Serial.println("Action: Go straight to node 1");
              action = 0;
              break;
          }
          break;
        case 3:
          Serial.println("Next Position: 3");
          switch (previousPosition) {
            case 3:
              Serial.println("Previous Position: 3");
              Serial.println("Action: Do a 180 and go straight to node 3");
              action = 3;
              break;
            case 1:
              Serial.println("Previous Position: 1");
              Serial.println("Action: Turn right");
              action = 2;
              break;
            case 5:
              Serial.println("Previous Position: 5");
              Serial.println("Action: Turn left");
              action = 1;
              break;
            default:
              Serial.println("Action: Go straight");
              action = 0;
              break;
          }
          break;
        case 4:
          Serial.println("Next Position: 4");
          switch (previousPosition) {
            case 4:
              Serial.println("Previous Position: 4");
              Serial.println("Action: Do a 180 and go straight to node 4");
              action = 3;
              break;
            case 1:
              Serial.println("Previous Position: 1");
              Serial.println("Action: Turn left");
              action = 1;
              break;
            case 5:
              Serial.println("Previous Position: 5");
              Serial.println("Action: Turn right");
              action = 2;
              break;
            default:
              Serial.println("Action: Go straight");
              action = 0;
              break;
          }
          break;
        case 5:
          Serial.println("Next Position: 5");
          switch (previousPosition) {
            case 5:
              Serial.println("Previous Position: 5");
              Serial.println("Action: Do a 180 and go straight to node 5");
              action = 3;
              break;
            case 3:
              Serial.println("Previous Position: 3");
              Serial.println("Action: Turn right");
              action = 2;
              break;
            case 4:
              Serial.println("Previous Position: 4");
              Serial.println("Action: Turn left");
              action = 1;
              break;
            default:
              Serial.println("Action: Go straight");
              action = 0;
              break;
          }
          break;
        default:
          Serial.println("Invalid next position for current node 4");
          break;
      }
      break;


    default:
      Serial.println("Invalid current position");
      break;
  }

  if (BBWBB() || WBBBW() || BWWWB()) {
    GoForwards();
    //straighten = 1;
    //sharp = 0;
  }
  else if (BWWBB() || BWBBB()) { 
    Left(straighten_left_l, straighten_left_r);
    //straighten = 1;
    //sharp = 0;
  }
  else if (WWWBB()) {
    Left(straighten_left_l, straighten_left_r);
    //sharp = 1;
    //straighten = 0;
  }
  else if (WBBBB() || WWBBB()) {
    Left(sharp_left_motor_l, sharp_left_motor_r);
    //sharp = 1;
    //straighten = 0;
  }
  else if (BBBWW() || BBBBW()) {
    Right(sharp_right_motor_l, sharp_right_motor_r);
    //sharp = 1;
    //straighten = 0;
  }
  else if (BBWWW()) {
    Right(straighten_right_l, straighten_right_r);
    //sharp = 1;
    //straighten = 0;
  }
  else if (BBBWB() || BBWWB()) {
    Right(straighten_right_l, straighten_right_r);
    //straighten = 1;
    //sharp = 0;
  }
  else if (WWWWW() || BWWWW() || WWWWB()) {
    Stop();
    delay(2000);
    if (action==3){
      TankLeft(tank_turn, tank_turn);
      left_or_right=0;
      delay(1000);
    }
    else if (action==1){
      TankLeft(tank_turn, tank_turn);
      left_or_right=0;
      delay(400);
    }
    else if (action==2){
      TankRight(tank_turn, tank_turn);
      left_or_right=1;
      delay(400);
    }
    else {
      GoForwards();
    }
    previousPosition = currentPosition;
    a++;
    if (a>=5){
      Stop();
      delay(9999999999);
    }
  }
  else if (BBBBB()) {
    if (left_or_right == 0) {
    Left(sharp_left_motor_l, sharp_left_motor_r);
    }
    else{
    Right(sharp_right_motor_l, sharp_right_motor_r);
    }
    //sharp = 1;
    //straighten = 0;
  }

  else {
    Stop();
  }
}

void Distancetest() {
  AnalogueValue[5] = analogRead(AnaloguePin[5]);
  dist = AnalogueValue[5];
  Serial.print("Distance Sensor Value: ");
  Serial.println(dist);
  delay(1);
}

void OpticalTest() {
  int i;
  for (i=0;i<5;i++)
  {
  AnalogueValue[i]=analogRead(AnaloguePin[i]);
  //Serial.print(AnalogueValue[i]); // This prints the actual analog reading from the sensors
  //Serial.print("\t"); //tab over on screen
  if(i==4)
      {
        //Serial.println(""); //carriage return
        delay(1); // display new set of readings every 600mS
      }
  }
}

void GoForwards() {
  digitalWrite(motor1Phase, HIGH); //forward
  analogWrite(motor1PWM, straight_l); // set speed of motor
  digitalWrite(motor2Phase, HIGH); //forward
  analogWrite(motor2PWM, straight_r); // set speed of motor
}

void Left(int turn_right, int turn_left) {
  digitalWrite(motor1Phase, HIGH);
  analogWrite(motor1PWM, turn_right);
  digitalWrite(motor2Phase, HIGH);
  analogWrite(motor2PWM, turn_left);
  left_or_right = 0;

}

void Right(int turn_right, int turn_left) {
  digitalWrite(motor1Phase, HIGH);
  analogWrite(motor1PWM, turn_right);
  digitalWrite(motor2Phase, HIGH);
  analogWrite(motor2PWM, turn_left);
  left_or_right = 1;
}

void TankLeft(int turn_right, int turn_left) {
  digitalWrite(motor1Phase, HIGH);
  analogWrite(motor1PWM, turn_right);
  digitalWrite(motor2Phase, LOW);
  analogWrite(motor2PWM, turn_left);
  left_or_right = 0;

}

void TankRight(int turn_right, int turn_left) {
  digitalWrite(motor1Phase, LOW);
  analogWrite(motor1PWM, turn_right);
  digitalWrite(motor2Phase, HIGH);
  analogWrite(motor2PWM, turn_left);
  left_or_right = 1;

}

void GoBackwards() {
  digitalWrite(motor1Phase, LOW);
  analogWrite(motor1PWM, straight_l);
  digitalWrite(motor2Phase, LOW);
  analogWrite(motor2PWM, straight_r);
}

void Stop() {
  analogWrite(motor1PWM, 0); 
  analogWrite(motor2PWM, 0);
}
