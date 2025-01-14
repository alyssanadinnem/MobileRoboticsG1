//MOTOR CODE
int motor1PWM = 37;
int motor1Phase = 38;
int motor2PWM = 39;
int motor2Phase = 20;

// the setup routine runs once when you press reset:
void setup() {
 Serial.begin(9600);
 pinMode(motor1PWM, OUTPUT);
 pinMode(motor2PWM, OUTPUT);
 pinMode(motor1Phase, OUTPUT);
 pinMode(motor2Phase, OUTPUT);
 }

// the loop routine runs over and over again continuously:
void loop() {
 GoForward();
 delay(2000); //2 seconds
 GoBackwards();
 delay(2000); //2 seconds
} 

void GoForward() {
 digitalWrite(motor1Phase, HIGH); //forward
 analogWrite(motor1PWM, 100); // set speed of motor
 Serial.println("Forward"); // Display motor direction
 digitalWrite(motor2Phase, HIGH); //forward
 analogWrite(motor2PWM, 100); // set speed of motor
 Serial.println("Forward"); // Display motor direction
}

void GoBackwards() {
 digitalWrite(motor1Phase, LOW); //Backward
 analogWrite(motor1PWM, 100); // set speed of motor
 Serial.println("Backward"); // Display motor direction 
 digitalWrite(motor2Phase, LOW); //Backward
 analogWrite(motor2PWM, 100); // set speed of motor
 Serial.println("Backward"); // Display motor direction
}
