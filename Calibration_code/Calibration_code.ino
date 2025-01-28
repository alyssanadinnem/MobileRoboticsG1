// MOTOR
int motor1PWM = 37; // left wheel - 1
int motor1Phase = 38;
int motor2PWM = 39; // right wheel - 2, slower
int motor2Phase = 20;

// OPTICAL SENSOR
int AnalogueValue[5] = {0, 0, 0, 0, 0};
int AnaloguePin[5] = {5, 4, 6, 7, 15};

// THRESHOLDS
int sensorMin = 1023; // Start with the maximum possible analog value
int sensorMax = 0;    // Start with the minimum possible analog value
int Threshold = 0;

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);

  // MOTOR setup
  pinMode(motor1PWM, OUTPUT);
  pinMode(motor2PWM, OUTPUT);
  pinMode(motor1Phase, OUTPUT);
  pinMode(motor2Phase, OUTPUT);

  // OPTICAL SENSOR setup
  for (int i = 0; i < 5; i++) {
    pinMode(AnaloguePin[i], INPUT);
  }

  Serial.println("Setup complete. Starting calibration...");
}

// the loop routine runs continuously:
void loop() {
  // Rotate for 10 seconds (1000 iterations with 10ms delay)
  for (int a = 0; a < 1000; a++) {
    OpticalTest();
    Left(100, 100); // Rotate the robot
    delay(10);
  }

  // Stop the robot
  StopMotors();

  // Calculate and print the threshold
  Threshold = (sensorMax + sensorMin) / 2; // Midpoint of min and max
  Serial.println("Calibration complete.");
  Serial.print("Min Value: ");
  Serial.println(sensorMin);
  Serial.print("Max Value: ");
  Serial.println(sensorMax);
  Serial.print("Threshold: ");
  Serial.println(Threshold);

  // Pause before repeating (shortened delay for debugging purposes)
  delay(5000);
}

// Function to test optical sensors and update min/max values
void OpticalTest() {
  for (int i = 0; i < 5; i++) {
    AnalogueValue[i] = analogRead(AnaloguePin[i]);

    // Update min and max values
    if (AnalogueValue[i] < sensorMin) {
      sensorMin = AnalogueValue[i];
    }
    if (AnalogueValue[i] > sensorMax) {
      sensorMax = AnalogueValue[i];
    }

    // Debug sensor readings
    //Serial.print("Sensor ");
    //Serial.print(i);
    //Serial.print(": ");
    //Serial.println(AnalogueValue[i]);
  }
}

// Function to rotate the robot to the left
void Left(int turn_right, int turn_left) {
  digitalWrite(motor1Phase, LOW);
  analogWrite(motor1PWM, turn_right);
  digitalWrite(motor2Phase, HIGH);
  analogWrite(motor2PWM, turn_left);
}

// Function to stop the robot
void StopMotors() {
  analogWrite(motor1PWM, 0); // Stop left motor
  analogWrite(motor2PWM, 0); // Stop right motor
  digitalWrite(motor1Phase, LOW);
  digitalWrite(motor2Phase, LOW);
}