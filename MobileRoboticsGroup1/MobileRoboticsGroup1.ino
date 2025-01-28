#include <WiFi.h>

//MOTOR
int motor1PWM = 37; //left wheel - 1
int motor1Phase = 38;
int motor2PWM = 39; //right wheel - 2, slower
int motor2Phase = 20;

//OPTICAL SENSOR 
int AnalogueValue[5] = {0,0,0,0,0};
int AnaloguePin[5] = {5,4,6,7,15};

//THRESHOLDS
int WhiteThreshold = 1000;

//LEFT OR RIGHT
int left_or_right = 0;

//SPEED VARIABLES
int straight_l = 130;
int straight_r = 125;
int sharp_right_motor_r = 140;
int sharp_right_motor_l = 0;
int sharp_left_motor_r = 0;
int sharp_left_motor_l = 140;
int straighten_left_r = 110;
int straighten_left_l = 130;
int straighten_right_r = 130;
int straighten_right_l = 110;

//WIFI DETAILS
char ssid[] = "***";
char password[] = "***";
WiFiClient client;

//SERVER DETAILS
char server[] = "54.78.246.30";
int port = 8081;

//RESPONSES
#define BUFSIZE 512
int destination
String response = readResponse(); // read response

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

  //WIFI
  delay(1000);
  connectToWiFi();
  connect();
}

// the loop routine runs over and over again continuously:
void loop() {

  OpticalTest();
  delay(10);

<<<<<<< Updated upstream
  //MOBILITY
  delay(1);
  if (BBWBB() || WBBBW() || BWWWB()) {
=======
  if (BBWBB() || BWWWW() || WWWWB() || BWWBB() || BBWWB() || WBBBW() || BWWWB() || BWBBB() || BBBWB() || BBWWW() || WWWBB()) {
>>>>>>> Stashed changes
    GoForwards();
  }
  else if (BWWBB() || BWBBB()) { 
    Left(straighten_left_l, straighten_left_r);
  } 
<<<<<<< Updated upstream
  else if (WWWBB()) {
    TankLeft(straighten_left_l, straighten_left_r);
  }
  else if (WBBBB() || WWBBB()) {
    Left(sharp_left_motor_l, sharp_left_motor_r);
  } 
  else if (BBBWW() || BBBBW()) {
    Right(sharp_right_motor_l, sharp_right_motor_r);
  } 
  else if (BBWWW()) {
    TankRight(straighten_right_l, straighten_right_r);
  }
  else if (BBBWB() || BBWWB()) {
    Right(straighten_right_l, straighten_right_r);
  } 
  else if (WWWWWW() || BWWWW() || WWWWB()) {
    Stop();
    delay(2000);
    GoForwards();
  }
  else if (BBBBB()) {
    if (left_or_right == 0) {
    Left(sharp_left_motor_l, sharp_left_motor_r);
    }
    else{
    Right(sharp_right_motor_l, sharp_right_motor_r);
    }
=======
  else if (WBBBB()) {
    GoRight(120);
  } 
  else if (WWBBB() || BWBBB()) {
    GoRight(80);
  } 
  else if (BBBBW()) {
    GoLeft(115);
  } 
  else if (BBBWW() || BBBWB()) {
    GoLeft(70);
  } 
  else if (WWWWWW()) {
    Stop();
    GoForwards();
  }
  else if (BBBBB()) {
    GoLeft(120);
>>>>>>> Stashed changes
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
  analogWrite(motor1PWM, straight_l); // set speed of motor
  digitalWrite(motor2Phase, HIGH); //forward
<<<<<<< Updated upstream
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
  digitalWrite(motor1Phase, LOW);
  analogWrite(motor1PWM, turn_right);
  digitalWrite(motor2Phase, HIGH);
  analogWrite(motor2PWM, turn_left);
  left_or_right = 0;

}

void TankRight(int turn_right, int turn_left) {
  digitalWrite(motor1Phase, HIGH);
  analogWrite(motor1PWM, turn_right);
  digitalWrite(motor2Phase, LOW);
  analogWrite(motor2PWM, turn_left);
  left_or_right = 1;

=======
  analogWrite(motor2PWM, 115); // set speed of motor
}

void GoRight(int turn_right) {
  digitalWrite(motor1Phase, HIGH);
  analogWrite(motor1PWM, turn_right);
  digitalWrite(motor2Phase, LOW);
  analogWrite(motor2PWM, 40);
}

void GoLeft(int turn_left) {
  digitalWrite(motor1Phase, LOW);
  analogWrite(motor1PWM, 35);
  digitalWrite(motor2Phase, HIGH);
  analogWrite(motor2PWM, turn_left);
>>>>>>> Stashed changes
}

void GoBackwards() {
  digitalWrite(motor1Phase, LOW);
  analogWrite(motor1PWM, straight_l);
  digitalWrite(motor2Phase, LOW);
  analogWrite(motor2PWM, straight_r);
}

void Stop() {
  analogWrite(motor1PWM, 0); 
<<<<<<< Updated upstream
  analogWrite(motor2PWM, 0);
}

void connectToWiFi() {
  Serial.print("Connecting to network: ");
  Serial.print(ssid);
  Serial.flush();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    Serial.flush();
    delay(300);
  }
  Serial.println("Connected");
  Serial.print("Obtaining IP address");
  Serial.flush();

  while (WiFi.localIP() == INADDR_NONE) {
    Serial.print(".");
    Serial.flush();
    delay(300);
  }
  
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

bool connect() {
  if (!client.connect(server, port)) {
    Serial.println("error connecting to server");
    return false;
  }
    return true;
}

String SendMessage() {
  // post body
  String postBody("position=");
  postBody += position;
  // send post request and headers
  client.println("POST /api/arrived/INSERT_YOUR_TEAM_ID_HERE HTTP/1.1");
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.print("Content-Length: ");
  client.println(postBody.length());
  client.println("Connection: close");
  client.println();
  // send post body
  client.println(postBody);
}

String readResponse() {
  char buffer[BUFSIZE];
  memset(buffer, 0, BUFSIZE);
  client.readBytes(buffer, BUFSIZE);
  String response(buffer);
  return response;
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

// get status code
int statusCode = getStatusCode(response);
  if (statusCode == 200) {
    // success, read body
    String body = getResponseBody(response);
    // check if at final destination
    if (!body.equals("undefined")) {
      destination = body.toInt();
    }
}

// disconnect
client.stop();
=======
  analogWrite(motor2PWM, 0); 
<<<<<<< HEAD
}
=======
}
//edit
//hello
//hi
>>>>>>> c5787955cf76e1d86542fe3576de1ce3d1848af0
>>>>>>> Stashed changes
