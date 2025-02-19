#include <WiFi.h> 
#include <Arduino.h>
#include <climits>
#include <ESP32Servo.h>
#include <esp_now.h>

// Define constants for Ultrasonic Sensor pins and motor control
#define ULTRASONIC_TRIG_PIN 10  // Pin for triggering the ultrasonic pulse
#define ULTRASONIC_ECHO_PIN 11  // Pin for receiving the ultrasonic echo
#define SERVO_PIN 12            // Pin to which the servo motor is attached
#define SOUND_SPEED 0.034       // Speed of sound in cm per microsecond (340 m/s)

#define BUFSIZE 512             //BUFFER SIZE FOR HTTPS RESPONSE

//MOTOR PINS
int motor1PWM = 37; //LEFT WHEEL: "1"
int motor1Phase = 38;
int motor2PWM = 39; //RIGHT WHEEL: "2"
int motor2Phase = 20;

//ANGLES
int previous_angle = 14;
int current_angle = 15;
bool increasing = true;  // Flag to track direction

//SERVO VARIABLES
long timeInterval;
int distance;
Servo motorControl;
bool LoopOnce = 0;

// Structure for ESP-NOW
typedef struct struct_message {
  int angle;
  int distance;
} struct_message;

// Create an instance
struct_message distanceAngle;

//OPTICAL SENSOR 
int AnalogueValue[6] = {0,0,0,0,0,0};
int AnaloguePin[6] = {5,4,6,7,15,17};

//THRESHOLDS
int WhiteThreshold = 2000;

//LEFT OR RIGHT
int left_or_right = 0;

//SPEED VARIABLES
int multiplier = 1.2;
int straight_l = 115*multiplier;
int straight_r = 105;
int sharp_right_motor_r = 155;
int sharp_right_motor_l = 0*multiplier;
int sharp_left_motor_r = 0;
int sharp_left_motor_l = 155*multiplier;
int straighten_left_r = 115;
int straighten_left_l = 135*multiplier;
int straighten_right_r = 135;
int straighten_right_l = 115*multiplier;
int tank_turn = 135*multiplier;
int slow_forward = 80*multiplier;

//DISTANCE
int dist = 0;
int end = 0;
int error = 0;

//ROUTE CONTROLs 
int previousPosition = 4;
int currentPosition = 0; //any number so it indicates no next point yet
int nextPosition = -1; //any number so it indicates no next point yet
int realNext;
int action = 0;
int afterSix = 666; //any number
int afterSeven = 777;

//DIJKSTRA ALGORITHM
#define INF 99999 //unreachable nodes
#define NODES 8
int path[NODES]; // Array to store the path
int pathIndex = 0; // Index to track the current position in the path
int pathNum = 0;

//WIFI DETAILS
char ssid[] = "iot";
char password[] = "kabikis75windfall"; //"needlings84wheezily"; 
WiFiClient client;

//SERVER DETAILS
char server[] = "3.250.38.184";
int port = 8000;
const char teamID[] = "afty6723";

//POSITIONS
int destination;

// MAC address = 48:ca:43:06:13:fc
uint8_t broadcastAddress[] = {0x48, 0xCA, 0x43, 0x06, 0x13, 0xFC}; 

esp_now_peer_info_t peerInfo;

// Callback for ESP-NOW
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

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
void Parking();
int getDistance();

///////////////////////////////  SETUP()  ////////////////////////////////////

void setup() {
  Serial.begin(115200);

  //MOTOR
  pinMode(motor1PWM, OUTPUT);
  pinMode(motor2PWM, OUTPUT);
  pinMode(motor1Phase, OUTPUT);
  pinMode(motor2Phase, OUTPUT);

  //SERVO
  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);
  motorControl.attach(SERVO_PIN);

  //OPTICAL SENSOR
  int i;
  for(i=0; i<5; i++) {
    pinMode(AnaloguePin[i], INPUT);
  }

  //WIFI
  connectToWiFi();
  connectToServer();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(); // Ensure ESP32 is in standalone mode

  //ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  else {
    Serial.println("esp INITIALIZED");
  }

  //PEER INITIALISE
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  //ADD PEER
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  else {
    Serial.println("PEER added");
  }

  //CALLBACK
  esp_now_register_send_cb(OnDataSent);
}

////////////////////////////////  LOOP()  ////////////////////////////////////

void loop() {
  
  //get the values for Moving()
  OpticalTest();
  OpticalPrint();
  Distancetest();
  DistancePrint();
  Moving();
  ServoMovement();
  delay(1);
  LoopOnce = 0;
}

///////////////////////////////  TESTS  ////////////////////////////////////////

void Distancetest() {
  AnalogueValue[5] = analogRead(AnaloguePin[5]);
  dist = AnalogueValue[5];
  /* UNDO COMMENT FOR DEBUGGING
  Serial.print("Distance Sensor Value: ");
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
  /* UNDO COMMENT FOR DEBUGGING
  Serial.print(AnalogueValue[i]); // This prints the actual analog reading from the sensors
  Serial.print("\t"); //tab over on screen
  */
  if(i==4)
      {
        //Serial.println(""); //carriage return (UNDO COMMENT FOR DEBUGGING)
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

void GoForwardsSlow() {
  digitalWrite(motor1Phase, HIGH); //forward
  analogWrite(motor1PWM, slow_forward); // set speed of motor
  digitalWrite(motor2Phase, HIGH); //forward
  analogWrite(motor2PWM, slow_forward); // set speed of motor
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

void Parking() {
  while(end==0){
    Distancetest();
    if(error<10){
      //Serial.println("Entering IF: Moving Forward");
      straight_l = 138;
      straight_r = 105;
      GoForwards();
      if(dist > 500) {
      error++;
      }
    }
    else {
      Serial.println("Entering ELSE: Moving Slowly");
      GoForwardsSlow();
      delay(970);
      Stop();
      delay(9999999999999);
    }
  }
}

void Moving() {
  
  if (BBWBB() || WBBBW() || BWWWB()) {
    GoForwards();
  }
  else if (BWWBB() || BWBBB() || BWBBW() || WWWBB()) {
    Left(straighten_left_l, straighten_left_r);
  }
  else if (WBBBB() || WWBBB()) {
    Left(sharp_left_motor_l, sharp_left_motor_r);
  }
  else if (BBBWW() || BBBBW()) {
    Right(sharp_right_motor_l, sharp_right_motor_r);
  }
  else if (BBWWW() || BBBWB() || BBWWB() || WBBWB() || WBWBB()) {
    Right(straighten_right_l, straighten_right_r);
  }
  else if (WWWWW() || BWWWW() || WWWWB()) {

    delay(100);
    Stop();

    Serial.print(" ");
    Serial.print("CHECK CURRENT POINT: ");
    Serial.println(currentPosition);

    //retrieve initial next position
    if (nextPosition == -1) {
      pathIndex = 0; //reset the path array
      Serial.println("Sending Message...");
      String nextPoint = UpdateNext(currentPosition); //get response from the server
      realNext = nextPoint.toInt(); //convert response to an integer
      Serial.print("TARGET POINT: ");
      Serial.println(realNext);
      dijkstraAlgorithm(currentPosition, realNext);  //find shortest path using Dijkstra's
    }

    switchCase(); //determine the action based on the next node
    actions(); //do the action
    pathNum++;

  }
  else if (BBBBB()) {

    if (left_or_right == 0) {
    Left(sharp_left_motor_l, sharp_left_motor_r);
    }
    else{
    Right(sharp_right_motor_l, sharp_right_motor_r);
    }
  }
  else {

    Stop();
  }  
}

/////////////////////////  SERVO AND ESP NOW  //////////////////////////////

/*void ServoMovement(){
  if(previous_angle < current_angle){
    go to current_angle
    current_angle++;
    previous_angle++;
  }
  else{
    go to current_angle
    current_angle--;
  previous_angle--;
  }
  delay(1);
}*/

void ServoMovement() {
  // Move servo to the current angle
  motorControl.write(current_angle);
  distance = getDistance();  // Measure distance

  // Set values to send
  distanceAngle.angle = current_angle;
  distanceAngle.distance = distance;
  esp_now_send(broadcastAddress, (uint8_t *)&distanceAngle, sizeof(distanceAngle));

  // Print for debugging
  Serial.print("Angle, Distance ");
  Serial.print(distanceAngle.angle);
  Serial.print(",");
  Serial.print(distanceAngle.distance);
  Serial.print(".");

  // Update previous angle
  previous_angle = current_angle;

  // Change the angle by 1 degree
  if (increasing) {
    current_angle++;
    if (current_angle >= 165) {
      increasing = false;  // Change direction at max
    }
  } else {
    current_angle--;
    if (current_angle <= 15) {
      increasing = true;  // Change direction at min
    }
  }
}












/*
void ServoMovement(){
  // Sweep servo from 15 to 165 degrees and measure distance
  while (LoopOnce == 0) {
    for (int angle = 15; angle <= 165; angle++) {
      motorControl.write(angle);  // Rotate servo to current angle
      delay(1);                  // Wait for servo to reach position
      distance = getDistance(); // Get distance from ultrasonic sensor
    
      // Set values to send
      distanceAngle.angle = angle; 
      distanceAngle.distance = distance;
      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&distanceAngle, sizeof(distanceAngle));
      esp_now_send(broadcastAddress, (uint8_t *)&distanceAngle, sizeof(distanceAngle));

      Serial.print(distanceAngle.angle);
      Serial.print(",");
      Serial.print(distanceAngle.distance);
      Serial.println(".");
    }

    // Sweep servo back from 165 to 15 degrees
    for (int angle = 165; angle >= 15; angle--) {
      motorControl.write(angle);
      delay(1);
      distance = getDistance();
    
      // Set values to send
      distanceAngle.angle = angle; 
      distanceAngle.distance = distance;
      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&distanceAngle, sizeof(distanceAngle));
      esp_now_send(broadcastAddress, (uint8_t *)&distanceAngle, sizeof(distanceAngle));

      Serial.print(distanceAngle.angle);
      Serial.print(",");
      Serial.print(distanceAngle.distance);
      Serial.println(".");

      if (angle == 15){
        LoopOnce = 1;
      }
    }
  }
  delay(1);
}

*/






// Get distance function
int getDistance() {
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  delay(1);
  digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
  delay(1);
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  
  timeInterval = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);
  distance = timeInterval * SOUND_SPEED / 2;
  delay(1);

  return distance;
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

  Serial.println(" ");
  Serial.print("Path Num: ");
  Serial.println(pathNum);
  Serial.print("Path Index: ");
  Serial.print(pathIndex);
  Serial.println(" ");
  
  currentPosition = path[pathNum];
  nextPosition = path[pathNum+1];

  Serial.println(" ");
  Serial.print("PREVIOUS POINT: ");
  Serial.println(previousPosition);
  Serial.print("CURRENT POINT: ");
  Serial.println(currentPosition);
  Serial.print("NEXT POINT: ");
  Serial.println(nextPosition);
  Serial.println(" ");

  if (currentPosition == realNext) {
    pathIndex = 0; //reset the path array
    String nextPoint = UpdateNext(currentPosition); //get response from the server
    realNext = nextPoint.toInt(); //convert response to an integer
    Serial.print("TARGET POINT: ");
    Serial.println(realNext);
    dijkstraAlgorithm(currentPosition, realNext); //find the shortest path using Dijkstra's algorithm
    currentPosition = path[pathNum];
    nextPosition = path[pathNum+1];

    /* UNDO COMMENT FOR DEBUGGING
    Serial.println(" ");
    Serial.println("AFTER UPDATING...");
    Serial.print("PREVIOUS POINT: ");
    Serial.println(previousPosition);
    Serial.print("CURRENT POINT: ");
    Serial.println(currentPosition);
    Serial.print("NEXT POINT: ");
    Serial.println(nextPosition);
    Serial.println(" ");
    */
  }

  //determine the action based on current, next, and previous positions
  switch (currentPosition) {
    case 0:
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
          Serial.printf("%d Invalid next position for current node 0\n", __LINE__);
          break;
      }
      break;

    case 1:
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
          Serial.printf("%d Invalid next position for current node 1\n", __LINE__);
          break;
      }
      break;
      
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
          Serial.printf("%d Invalid next position for current node 2\n", __LINE__);
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
          Serial.printf("%d Invalid next position for current node 3\n", __LINE__);
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
          Serial.printf("%d Invalid next position for current node 4\n", __LINE__);
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


  if (previousPosition == nextPosition || previousPosition == 100 && nextPosition == 4) {
      action = 3;
  }

  previousPosition = currentPosition;

}

/////////////////////////////  ACTIONS  ///////////////////////////////////////

void actions() {

  if (action==3){ //180
      TankLeft(tank_turn, tank_turn);
      left_or_right=0;
      delay(1200);
    }
    else if (action==1){ //left
      GoForwardsSlow();
      delay(700);
      TankLeft(tank_turn, tank_turn);
      left_or_right=0;
      delay(600);
    }
    else if (action==2){ //right
      GoForwardsSlow();
      delay(700);
      TankRight(tank_turn, tank_turn);
      left_or_right=1;
      delay(600);
    }
    else if (action==4){ //parking from 3
      GoForwardsSlow();
      delay(700);
      tank_turn=80;
      TankRight(tank_turn, tank_turn);
      left_or_right=1;
      delay(1000);
      OpticalTest();
      while(AnalogueValue[2] >= WhiteThreshold){
        OpticalTest();
        TankRight(tank_turn, tank_turn);
      }
      Parking();
    }
    else if (action==5){ //parking from 4
      GoForwardsSlow();
      delay(700);
      tank_turn=80;
      TankLeft(tank_turn, tank_turn);
      left_or_right=0;
      delay(1000);
      OpticalTest();
      while(AnalogueValue[2] >= WhiteThreshold){
        OpticalTest();
        TankLeft(tank_turn, tank_turn);
      }
      Parking();
    }
    else if (action==6){ //parking from 7, prev 5
      TankLeft(tank_turn, tank_turn);
      left_or_right=0;
      delay(1200);
      Parking();
    }
    else if (action==7){ //parking from 6, 1, or 7
        slow_forward = 80;
        GoForwardsSlow();
        delay(250);
        OpticalTest();
        if (AnalogueValue[0] <= WhiteThreshold) {
            straighten_left_l=80;
            straighten_left_r=60;
            Left(straighten_left_l, straighten_left_r);
            delay(500);
        }
        if (AnalogueValue[4] <= WhiteThreshold) {
            straighten_right_l=60;
            straighten_right_r=80;
            Right(straighten_right_l, straighten_right_r);
            delay(500);
        }
        Parking();
    }
    else if(action==0){
      GoForwards();
      delay(1000);
    }
}

///////////////////////////  WIFI FUNCTIONS  //////////////////////////////////////

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

String UpdateNext(int position) { 

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
    delay(10);

    Serial.print("Position is Valid. Sent message: ");
    Serial.println(position);
    delay(10);
  } 

  String response = readResponse();
  int statusCode = getStatusCode(response);

  /* UNDO COMMENT FOR DEBUGGING
  Serial.print("Status Code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
  */

  if (statusCode == 200 || statusCode == 400) { //changed for some reason
    String responseBody = getResponseBody(response);
    Serial.print("Response Body: ");
    Serial.println(responseBody);

    //check if the response ends with "finished"
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

//////////////////////////////  DIJKSTRA'S  ////////////////////////////////////////

//adj matrix representing cost graph
int nodes[NODES][NODES] = {
    {0, INF, INF, INF, 1, INF, 1, INF}, // Node 0
    {INF, 0, INF, INF, INF, INF, 1, 1}, // Node 1
    {INF, INF, 0, 1, INF, INF, 1, INF}, // Node 2
    {INF, INF, 1, 0, INF, INF, INF, 2}, // Node 3
    {1, INF, INF, INF, 0, INF, INF, 2}, // Node 4
    {INF, INF, INF, INF, INF, 0, INF, 1}, // Node 5
    {1, 1, 1, INF, INF, INF, 0, INF}, // Node 6
    {INF, 1, INF, 2, 2, 1, INF, 0}  // Node 7
};

//function to find the node with the minimum distance that hasn't been visited
int shortestPath(int costs[], bool visited[]) {
  int minimumCost = INF;
  int nearestNode = -1;

  for (int i = 0; i < NODES; i++) {
    if (!visited[i] && costs[i] < minimumCost) {
      minimumCost = costs[i];
      nearestNode = i;
    }
  }
  return nearestNode;
}

//dijkstra's algorithm to find the shortest path
void dijkstraAlgorithm(int initial, int final) {

  int costs[NODES]; //stores the shortest distance from initial to each node
  bool visited[NODES];  //tracks which nodes have been visited
  int previous[NODES];  //stores the previous node in the optimal path
  pathNum = 0; 

  for (int i = 0; i < NODES; i++) {
    costs[i] = INF;
    visited[i] = false;
    previous[i] = -1;
  }

  costs[initial] = 0;

  for (int i = 0; i < NODES; i++) {
    int current = shortestPath(costs, visited);

    if (current == -1) {
      break; //no more nodes to process
    }

    visited[current] = true;

    //updste costs
    for (int neighbour = 0; neighbour < NODES; neighbour++) {
      if (!visited[neighbour] && nodes[current][neighbour] != INF) {
        int distance = costs[current] + nodes[current][neighbour];

        if (distance < costs[neighbour]) {
          costs[neighbour] = distance;
          previous[neighbour] = current;
        }
      }
    }
  }

  int current = final;

  while (current != -1) {
    path[pathIndex] = current;
    pathIndex++;
    current = previous[current];
  }

  //path given backwards, reverse it
  for (int i = 0; i < pathIndex / 2; i++) {
    int temp = path[i];
    path[i] = path[pathIndex - 1 - i];
    path[pathIndex - 1 - i] = temp;
  }

  //print path
  Serial.println("The Path:");
  for (int i = 0; i < pathIndex; i++) {
    Serial.println(path[i]);
  }
}