#include <WiFi.h> 

//MOTOR PINS
int motor1PWM = 37; //LEFT WHEEL: "1"
int motor1Phase = 38;
int motor2PWM = 39; //RIGHT WHEEL: "2"
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
int straighten_left_r = 115;
int straighten_left_l = 135;
int straighten_right_r = 135;
int straighten_right_l = 115;
int tank_turn = 135;

//DISTANCE
int dist = 0;

//HARDCODED ROUTE CONTROL
/*
int route[] = {0,6,1,7,3};
int previousPosition = 4;
int currentPosition = 0;
int nextPosition = 6;
int action = 0;
int a = 0;
*/

//ROUTE CONTROLs 
int previousPosition = 4;
int currentPosition = 100; //any number so it indicates no next point yet
int nextPosition = -1; //any number so it indicates no next point yet
int action = 0;
int beforeSix = 666; //any number
int beforeSeven = 777;

//WIFI DETAILS
char ssid[] = "iot";
char password[] = "unmercenarily56aweto"; // "kabikis75windfall"; 
WiFiClient client;

//SERVER DETAILS
char server[] = "3.250.38.184";
int port = 8000;
const char teamID[] = "afty6723";

//BUFFER SIZE FOR HTTPS RESPONSE
#define BUFSIZE 512

//POSITIONS
int destination;

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

///////////////////////////////  SETUP()  ////////////////////////////////////

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

  //WIFI
  connectToWiFi();
  connectToServer();
}

////////////////////////////////  LOOP()  ////////////////////////////////////

void loop() {
  
  //get the values for Moving()
  OpticalTest();
  Distancetest();
  delay(1);

  /*//print current, next and prev positions
  OpticalPrint();
  DistancePrint();
  */
  
  Moving();
  delay(50);
}

///////////////////////////////  TESTS  ////////////////////////////////////////

void Distancetest() {
  AnalogueValue[5] = analogRead(AnaloguePin[5]);
  dist = AnalogueValue[5];
  /*Serial.print("Distance Sensor Value: ");
  Serial.println(dist);*/
  delay(1);
}

void DistancePrint() {
  Serial.print("Distance Sensor Value: ");
  Serial.println(dist);
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
        delay(1); //display new set of readings every 600mS
      }
  }
}

void OpticalPrint() {
  int i;
  for (i=0;i<5;i++)
  {
  Serial.print(AnalogueValue[i]); //This prints the actual analog reading from the sensors
  Serial.print("\t"); //tab over on screen
  if(i==4)
      {
        Serial.println(""); //carriage return
        delay(1); //display new set of readings every 600mS
      }
  }
}

void ServerTest() {
  for (int i=0; i<5; i++) {
    Serial.print("Sent Message: ");
    Serial.println(i);
    delay(1000);
    String response = UpdateNext(i);
    Serial.print("Received Message: ");
    Serial.println(response);
    delay(1000);
  }
}

/////////////////////////  MOBILITY FUNCTIONS  //////////////////////////////

void GoForwards() {
  digitalWrite(motor1Phase, HIGH); //forward
  analogWrite(motor1PWM, straight_l); //set speed of motor
  digitalWrite(motor2Phase, HIGH); //forward
  analogWrite(motor2PWM, straight_r); //set speed of motor
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

void Moving() {
  
  if (BBWBB() || WBBBW() || BWWWB()) {
    Serial.printf("%d ", __LINE__);
    GoForwards();
  }
  else if (BWWBB() || BWBBB() || BWBBW() || WWWBB()) {
    Serial.printf("%d", __LINE__);
    Left(straighten_left_l, straighten_left_r);
  }
  else if (WBBBB() || WWBBB()) {
    Serial.printf("%d", __LINE__);
    Left(sharp_left_motor_l, sharp_left_motor_r);
  }
  else if (BBBWW() || BBBBW()) {
    Serial.printf("%d", __LINE__);
    Right(sharp_right_motor_l, sharp_right_motor_r);
  }
  else if (BBWWW() || BBBWB() || BBWWB() || WBBWB() || WBWBB()) {
    Serial.printf("%d", __LINE__);
    Right(straighten_right_l, straighten_right_r);
  }
  else if (WWWWW() || BWWWW() || WWWWB()) {
    
    Serial.printf("%d", __LINE__);
    delay(100);
    Stop();
    Serial.println("Sending Message...");

    Serial.print("CURRENT POINT: ");
    Serial.println(currentPosition);

    if (nextPosition == -1 || nextPosition == currentPosition) {
      String nextPoint = UpdateNext(currentPosition); //get response from the server
      nextPosition = nextPoint.toInt(); //convert response to an integer
      Serial.print("NEXT POINT: ");
      Serial.println(nextPosition);
    }
  
    switchCase();
    delay(50);

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
    
  }
  else if (BBBBB()) {

    Serial.printf("%d", __LINE__);

    if (left_or_right == 0) {
    Left(sharp_left_motor_l, sharp_left_motor_r);
    }
    else{
    Right(sharp_right_motor_l, sharp_right_motor_r);
    }
  }
  else {

    Serial.printf("%d", __LINE__);
    Stop();
  }  
}

////////////////////////////////  CASES  /////////////////////////////////////

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

bool BWBBW() { //white on fifth
  return (AnalogueValue[0] >= WhiteThreshold &&
          AnalogueValue[1] <= WhiteThreshold &&
          AnalogueValue[2] >= WhiteThreshold &&
          AnalogueValue[3] >= WhiteThreshold &&
          AnalogueValue[4] <= WhiteThreshold);
}

bool WBBWB() { //white on fifth
  return (AnalogueValue[0] <= WhiteThreshold &&
          AnalogueValue[1] >= WhiteThreshold &&
          AnalogueValue[2] >= WhiteThreshold &&
          AnalogueValue[3] <= WhiteThreshold &&
          AnalogueValue[4] >= WhiteThreshold);
}

bool WBWBB() { //white on fifth
  return (AnalogueValue[0] <= WhiteThreshold &&
          AnalogueValue[1] >= WhiteThreshold &&
          AnalogueValue[2] <= WhiteThreshold &&
          AnalogueValue[3] >= WhiteThreshold &&
          AnalogueValue[4] >= WhiteThreshold);
}

/////////////////////////////  SWITCH CASES  //////////////////////////////////////

void switchCase() {

  previousPosition = currentPosition;
  currentPosition = nextPosition;

  if (nextPosition == -1 || nextPosition == currentPosition) {
    String nextPoint = UpdateNext(currentPosition); //get response from the server
    nextPosition = nextPoint.toInt(); //convert response to an integer
    Serial.print("NEXT POINT: ");
    Serial.println(nextPosition);
  }

  //Determine the action based on current, next, and previous positions
  switch (currentPosition) {

  case 0: //DONE: Next Positions = 4, 1, 2 (6)
    Serial.printf("%d Current Position: 0\n", __LINE__);
    switch (nextPosition) {
      case 4:
        Serial.printf("%d Next Position: 4\n", __LINE__);
        switch (previousPosition) {
          case 4:
            Serial.printf("%d Previous Position: 4\n", __LINE__);
            Serial.printf("%d Action: Do a 180 and go straight to node 0\n", __LINE__);
            action = 3;
            break;
          default:
            Serial.printf("%d Action: Go straight to node 4\n", __LINE__);
            action = 0;
            break;
        }
        break;
      case 6:
        Serial.printf("%d Next Position: 6\n", __LINE__);
        switch (previousPosition) {
          case 6:
            Serial.printf("%d Previous Position: 6\n", __LINE__);
            Serial.printf("%d Action: Do a 180 and go straight to node 0\n", __LINE__);
            action = 3;
            break;
          default:
            Serial.printf("%d Action: Go straight to node 6\n", __LINE__);
            action = 0;
            break;
        }
        break;
      default:
        //if nextPosition is 1 or 2, default to going to node 6
        if (nextPosition == 1 || nextPosition == 2) {
          Serial.printf("%d Next Position is 1 or 2. Defaulting to node 6.\n", __LINE__);
    
          beforeSix = nextPosition;
          Serial.print("ORIGINAL Next Point: ");
          Serial.println(beforeSix);

          nextPosition = 6; //set nextPosition to 6
          Serial.printf("%d Action: Go straight to node 6\n", __LINE__);
          action = 0;
        } 
        else {
          Serial.printf("%d Invalid next position for current node 0\n", __LINE__);
        }
        break;
    }
    break;
  //Current = 0, DONE

    case 1: //DONE: Next Position = 2,3,4,5 (6, 7)
      Serial.printf("%d Current Position: 1\n", __LINE__);
      switch (nextPosition) {
        case 6:
          Serial.printf("%d Next Position: 6\n", __LINE__);
          switch (previousPosition) {
            case 6:
              Serial.printf("%d Previous Position: 6\n", __LINE__);
              Serial.printf("%d Action: Do a 180 and go straight to node 6\n", __LINE__);
              action = 3;
              break;
            default:
              Serial.printf("%d Action: Go straight to node 6\n", __LINE__);
              action = 0;
              break;
          }
          break;
        case 7:
          Serial.printf("%d Next Position: 7\n", __LINE__);
          switch (previousPosition) {
            case 7:
              Serial.printf("%d Previous Position: 7\n", __LINE__);
              Serial.printf("%d Action: Do a 180 and go straight to node 7\n", __LINE__);
              action = 3;
              break;
            default:
              Serial.printf("%d Action: Go straight to node 7\n", __LINE__);
              action = 0;
              break;
          }
          break;
        default:
          //if nextPosition is 0 or 2, default to going to node 6
          if (nextPosition == 0 || nextPosition == 2) {
            Serial.printf("%d Next Position is 0 or 2. Defaulting to node 6.\n", __LINE__);
      
            beforeSix = nextPosition;
            Serial.print("ORIGINAL Next Point: ");
            Serial.println(beforeSix);

            nextPosition = 6; //set nextPosition to 6
            Serial.printf("%d Action: Go straight to node 6\n", __LINE__);
            action = 0;
          } 
          //if nextPosition is 3 or 4, default to going to node 7
          else if (nextPosition == 3 || nextPosition == 4) {
            Serial.printf("%d Next Position is 3 or 4. Defaulting to node 7.\n", __LINE__);
      
            beforeSeven = nextPosition;
            Serial.print("ORIGINAL Next Point: ");
            Serial.println(beforeSeven);

            nextPosition = 7; //set nextPosition to 7
            Serial.printf("%d Action: Go straight to node 7\n", __LINE__);
            action = 0;
          } 
          else {
            Serial.printf("%d Invalid next position for current node 0\n", __LINE__);
          }
          break;
      }
      break;
      //Current = 1, DONE
      
    case 2:
      Serial.printf("%d Current Position: 2\n", __LINE__);
      switch (nextPosition) {
        case 3:
          Serial.printf("%d Next Position: 3\n", __LINE__);
          switch (previousPosition) {
            case 3:
              Serial.printf("%d Previous Position: 3\n", __LINE__);
              Serial.printf("%d Action: Do a 180 and go straight to node 3\n", __LINE__);
              action = 3;
              break;
            default:
              Serial.printf("%d Action: Go straight to node 3\n", __LINE__);
              action = 0;
              break;
          }
          break;
        case 6:
          Serial.printf("%d Next Position: 6\n", __LINE__);
          switch (previousPosition) {
            case 6:
              Serial.printf("%d Previous Position: 6\n", __LINE__);
              Serial.printf("%d Action: Do a 180 and go straight to node 6\n", __LINE__);
              action = 3;
              break;
            default:
              Serial.printf("%d Action: Go straight to node 6\n", __LINE__);
              action = 0;
              break;
          }
          break;
        default:
          //if nextPosition is 0 or 1, default to going to node 6
          if (nextPosition == 0 || nextPosition == 1) {
            Serial.printf("%d Next Position is 0 or 1. Defaulting to node 6.\n", __LINE__);
      
            beforeSix = nextPosition;
            Serial.print("ORIGINAL Next Point: ");
            Serial.println(beforeSix);

            nextPosition = 6; //set nextPosition to 6
            Serial.printf("%d Action: Go straight to node 6\n", __LINE__);
            action = 0;
          } 
          else {
            Serial.printf("%d Invalid next position for current node 0\n", __LINE__);
          }
          break;
      }
      break;

    case 3:
      Serial.printf("%d Current Position: 3\n", __LINE__);
      switch (nextPosition) {
        case 2:
          Serial.printf("%d Next Position: 2\n", __LINE__);
          switch (previousPosition) {
            case 2:
              Serial.printf("%d Previous Position: 2\n", __LINE__);
              Serial.printf("%d Action: Do a 180 and go straight to node 2\n", __LINE__);
              action = 3;
              break;
            default:
              Serial.printf("%d Action: Go straight to node 2\n", __LINE__);
              action = 0;
              break;
          }
          break;
        case 7:
          Serial.printf("%d Next Position: 7\n", __LINE__);
          switch (previousPosition) {
            case 7:
              Serial.printf("%d Previous Position: 7\n", __LINE__);
              Serial.printf("%d Action: Do a 180 and go straight to node 7\n", __LINE__);
              action = 3;
              break;
            default:
              Serial.printf("%d Action: Go straight to node 7\n", __LINE__);
              action = 0;
              break;
          }
          break;
        default:
          if (nextPosition == 1 || nextPosition == 4) {
            Serial.printf("%d Next Position is 1 or 4. Defaulting to node 7.\n", __LINE__);
      
            beforeSeven = nextPosition;
            Serial.print("ORIGINAL Next Point: ");
            Serial.println(beforeSeven);

            nextPosition = 7; //set nextPosition to 7
            Serial.printf("%d Action: Go straight to node 7\n", __LINE__);
            action = 0;
          } 
          else {
            Serial.printf("%d Invalid next position for current node 0\n", __LINE__);
          }
          break;
      }
      break;

    case 4:
      Serial.printf("%d Current Position: 4\n", __LINE__);
      switch (nextPosition) {
        case 0:
          Serial.printf("%d Next Position: 0\n", __LINE__);
          switch (previousPosition) {
            case 0:
              Serial.printf("%d Previous Position: 0\n", __LINE__);
              Serial.printf("%d Action: Do a 180 and go straight to node 0\n", __LINE__);
              action = 3;
              break;
            default:
              Serial.printf("%d Action: Go straight to node 0\n", __LINE__);
              action = 0;
              break;
          }
          break;
        case 7:
          Serial.printf("%d Next Position: 7\n", __LINE__);
          switch (previousPosition) {
            case 7:
              Serial.printf("%d Previous Position: 7\n", __LINE__);
              Serial.printf("%d Action: Do a 180 and go straight to node 7\n", __LINE__);
              action = 3;
              break;
            default:
              Serial.printf("%d Action: Go straight to node 7\n", __LINE__);
              action = 0;
              break;
          }
          break;
        default:
          if (nextPosition == 1 || nextPosition == 3) {
            Serial.printf("%d Next Position is 1 or 3. Defaulting to node 7.\n", __LINE__);
      
            beforeSeven = nextPosition;
            Serial.print("ORIGINAL Next Point: ");
            Serial.println(beforeSeven);

            nextPosition = 7; //set nextPosition to 7
            Serial.printf("%d Action: Go straight to node 7\n", __LINE__);
            action = 0;
          } 
          else {
            Serial.printf("%d Invalid next position for current node 0\n", __LINE__);
          }
          break;
      }
      break;

    case 5:
      Serial.printf("%d Current Position: 5\n", __LINE__);
      switch (nextPosition) {
        case 7:
          Serial.printf("%d Next Position: 7\n", __LINE__);
          switch (previousPosition) {
            case 7:
              Serial.printf("%d Previous Position: 7\n", __LINE__);
              Serial.printf("%d Action: Do a 180 and go straight to node 7\n", __LINE__);
              action = 3;
              break;
            default:
              Serial.printf("%d Previous Position: 7\n", __LINE__);
              Serial.printf("%d Action: Do a 180 and go straight to node 7\n", __LINE__);
              action = 3;
              break;
          }
          break;
      }
      break;

    case 6:
      Serial.printf("%d Current Position: 6\n", __LINE__);

      Serial.print("ORIGINAL Next Position: ");
      Serial.println(beforeSix);

      nextPosition = beforeSix;

      switch (nextPosition) {
        case 0:
          Serial.printf("%d Next Position: 0\n", __LINE__);
          switch (previousPosition) {
            case 0:
              Serial.printf("%d Previous Position: 0\n", __LINE__);
              Serial.printf("%d Action: Do a 180 and go straight to node 0\n", __LINE__);
              action = 3;
              break;
            case 1:
              Serial.printf("%d Previous Position: 1\n", __LINE__);
              Serial.printf("%d Action: Turn right\n", __LINE__);
              action = 2;
              break;
            default:
              Serial.printf("%d Action: Go straight to node 0\n", __LINE__);
              action = 0;
              break;
          }
          break;
        case 1:
          Serial.printf("%d Next Position: 1\n", __LINE__);
          switch (previousPosition) {
            case 1:
              Serial.printf("%d Previous Position: 1\n", __LINE__);
              Serial.printf("%d Action: Do a 180 and go straight to node 1\n", __LINE__);
              action = 3;
              break;
            case 0:
              Serial.printf("%d Previous Position: 0\n", __LINE__);
              Serial.printf("%d Action: Turn left\n", __LINE__);
              action = 1;
              break;
            default:
              Serial.printf("%d Action: Turn right\n", __LINE__);
              action = 2;
              break;
          }
          break;
        case 2:
          Serial.printf("%d Next Position: 2\n", __LINE__);
          switch (previousPosition) {
            case 2:
              Serial.printf("%d Previous Position: 2\n", __LINE__);
              Serial.printf("%d Action: Do a 180 and go straight to node 2\n", __LINE__);
              action = 3;
              break;
            case 1:
              Serial.printf("%d Previous Position: 1\n", __LINE__);
              Serial.printf("%d Action: Turn left\n", __LINE__);
              action = 1;
              break;
            default:
              Serial.printf("%d Action: Go straight\n", __LINE__);
              action = 0;
              break;
          }
          break;
        default:
          Serial.printf("%d Invalid next position for current node 6\n", __LINE__);
          break;
      }
      break;

    case 7:

      Serial.print("ORIGINAL Next Position: ");
      Serial.println(beforeSeven);

      nextPosition = beforeSeven; 

      Serial.printf("%d Current Position: 7\n", __LINE__);
      switch (nextPosition) {
        case 1:
          Serial.printf("%d Next Position: 1\n", __LINE__);
          switch (previousPosition) {
            case 1:
              Serial.printf("%d Previous Position: 1\n", __LINE__);
              Serial.printf("%d Action: Do a 180 and go straight to node 1\n", __LINE__);
              action = 3;
              break;
            case 3:
              Serial.printf("%d Previous Position: 3\n", __LINE__);
              Serial.printf("%d Action: Turn left\n", __LINE__);
              action = 1;
              break;
            case 4:
              Serial.printf("%d Previous Position: 4\n", __LINE__);
              Serial.printf("%d Action: Turn right\n", __LINE__);
              action = 2;
              break;
            default:
              Serial.printf("%d Action: Go straight to node 1\n", __LINE__);
              action = 0;
              break;
          }
          break;
        case 3:
          Serial.printf("%d Next Position: 3\n", __LINE__);
          switch (previousPosition) {
            case 3:
              Serial.printf("%d Previous Position: 3\n", __LINE__);
              Serial.printf("%d Action: Do a 180 and go straight to node 3\n", __LINE__);
              action = 3;
              break;
            case 1:
              Serial.printf("%d Previous Position: 1\n", __LINE__);
              Serial.printf("%d Action: Turn right\n", __LINE__);
              action = 2;
              break;
            case 5:
              Serial.printf("%d Previous Position: 5\n", __LINE__);
              Serial.printf("%d Action: Turn left\n", __LINE__);
              action = 1;
              break;
            default:
              Serial.printf("%d Action: Go straight\n", __LINE__);
              action = 0;
              break;
          }
          break;
        case 4:
          Serial.printf("%d Next Position: 4\n", __LINE__);
          switch (previousPosition) {
            case 4:
              Serial.printf("%d Previous Position: 4\n", __LINE__);
              Serial.printf("%d Action: Do a 180 and go straight to node 4\n", __LINE__);
              action = 3;
              break;
            case 1:
              Serial.printf("%d Previous Position: 1\n", __LINE__);
              Serial.printf("%d Action: Turn left\n", __LINE__);
              action = 1;
              break;
            case 5:
              Serial.printf("%d Previous Position: 5\n", __LINE__);
              Serial.printf("%d Action: Turn right\n", __LINE__);
              action = 2;
              break;
            default:
              Serial.printf("%d Action: Go straight\n", __LINE__);
              action = 0;
              break;
          }
          break;
        case 5:
          Serial.printf("%d Next Position: 5\n", __LINE__);
          switch (previousPosition) {
            case 5:
              Serial.printf("%d Previous Position: 5\n", __LINE__);
              Serial.printf("%d Action: Do a 180 and go straight to node 5\n", __LINE__);
              action = 3;
              break;
            case 3:
              Serial.printf("%d Previous Position: 3\n", __LINE__);
              Serial.printf("%d Action: Turn right\n", __LINE__);
              action = 2;
              break;
            case 4:
              Serial.printf("%d Previous Position: 4\n", __LINE__);
              Serial.printf("%d Action: Turn left\n", __LINE__);
              action = 1;
              break;
            default:
              Serial.printf("%d Action: Go straight\n", __LINE__);
              action = 0;
              break;
          }
          break;
        default:
          Serial.printf("%d Invalid next position for current node 4\n", __LINE__);
          break;
      }
      break;

    default:
      Serial.printf("%d Invalid current position\n", __LINE__);
      break;
  }
}

////////////////////////////  WIFI FUNCTIONS  //////////////////////////////////////

void connectToWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    Serial.print(".");
    delay(500);
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect to WiFi.");
  }
}

bool connectToServer() {
  if (!client.connect(server, port)) {
    Serial.println("Error connecting to server.");
    return false;
  }
  Serial.println("Connected to server!");
  return true;
}

String UpdateNext(int position) { //previously SendMessage()

  if (!client.connected()) {
    Serial.println("Client not connected, reconnecting...");
    if (!connectToServer()) {
      return "";
    }
  }

  //send to server if its valid nodes 0,1,2,3,4,5 
 if (currentPosition == 1 || currentPosition == 2 || currentPosition == 3 || currentPosition == 4 || currentPosition == 0 || currentPosition == 5) {
    String postBody = "position=" + String(position);

    client.println("POST /api/arrived/" + String(teamID) + " HTTP/1.1");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(postBody.length());
    client.println("Connection: keep-alive");
    client.println();
    client.println(postBody);
    delay(500);

    Serial.print("Position is Valid. Sent message: ");
    Serial.println(position);
    delay(500);
  } 

  String response = readResponse();
  int statusCode = getStatusCode(response);

  Serial.print("Status Code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);

  if (statusCode == 200 || statusCode == 400) { //changed for some reason
    String responseBody = getResponseBody(response);
    Serial.print("Response Body: ");
    Serial.println(responseBody);

    //Check if the response ends with "finished"
    if (responseBody == "Finished") {
      Serial.println("Received 'Finished' from server. Stopping Moby.");
      Stop(); //Stop the robot
      delay(9999999);
    }

  Serial.print("Received Next Node: ");
  Serial.println(responseBody);
  Serial.println("");

  return responseBody;
  }
  return "";
}

String readResponse() {
  char buffer[BUFSIZE + 1];
  memset(buffer, 0, BUFSIZE + 1);
  int bytesRead = client.readBytes(buffer, BUFSIZE);
  buffer[bytesRead] = '\0';
  return String(buffer);
}

int getStatusCode(String& response) {
  String code = response.substring(9, 12);
  return code.toInt();
}

String getResponseBody(String& response) {
  int split = response.indexOf("\r\n\r\n");
  String body = response.substring(split+4, response.length());
  body.trim();
  return body;
}

String receiveNextPoint(int position) {
  if (!client.connected()) {
    Serial.println("Client not connected, reconnecting...");
    if (!connectToServer()) {
      return "";
    }
  }

  String getRequest = "GET /api/receiveNextPoint/" + String(teamID) + "?position=" + String(position) + " HTTP/1.1";
  client.println(getRequest);
  client.println("Host: " + String(server));
  client.println("Connection: close");
  client.println();

  String response = readResponse();
  int statusCode = getStatusCode(response);
  if (statusCode == 200) {
    return getResponseBody(response);
  }
  Serial.println("Error: " + response);
  return "";
}
