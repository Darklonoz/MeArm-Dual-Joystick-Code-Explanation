#include <Servo.h>

// Declare Servo Objects
Servo baseServo;
Servo leftServo;  // Left joint / Shoulder
Servo rightServo; // Right joint / Elbow
Servo clawServo;  // Gripper

// Define Joystick Analog Pins
const int JOY1_X = A0; // Base control
const int JOY1_Y = A1; // Left joint control
const int JOY2_X = A2; // Right joint control
const int JOY2_Y = A3; // Claw control

// Starting Positions (Degrees)
int baseAngle  = 90;
int leftAngle  = 90;
int rightAngle = 90;
int clawAngle  = 90;

// Deadzone & Speed Configuration
const int DEADZONE_LOW  = 400; // Deadzone threshold to stop idle drift
const int DEADZONE_HIGH = 600;
const int STEP_SPEED    = 2;   // Degrees moved per loop (increase for faster movement)

void setup() {
  // Attach Servos to your specific pins
  baseServo.attach(9);  // Base
  leftServo.attach(8);  // Left Joint
  rightServo.attach(7);  // Right Joint
  clawServo.attach(6);   // Claw

  // Set Initial Arm Pose
  baseServo.write(baseAngle);
  leftServo.write(leftAngle);
  rightServo.write(rightAngle);
  clawServo.write(clawAngle);

  delay(1000);
}

void loop() {
  // -------------------------------------------------------------
  // 1. BASE SERVO CONTROL (Pin 9 | Joystick 1 - X axis)
  // -------------------------------------------------------------
  int joy1XVal = analogRead(JOY1_X);
  if (joy1XVal < DEADZONE_LOW) {
    baseAngle = max(0, baseAngle - STEP_SPEED);
  } else if (joy1XVal > DEADZONE_HIGH) {
    baseAngle = min(180, baseAngle + STEP_SPEED);
  }
  baseServo.write(baseAngle);

  // -------------------------------------------------------------
  // 2. LEFT JOINT CONTROL (Pin 8 | Joystick 1 - Y axis)
  // -------------------------------------------------------------
  int joy1YVal = analogRead(JOY1_Y);
  if (joy1YVal < DEADZONE_LOW) {
    leftAngle = max(30, leftAngle - STEP_SPEED);  // 30° safety limit
  } else if (joy1YVal > DEADZONE_HIGH) {
    leftAngle = min(150, leftAngle + STEP_SPEED); // 150° safety limit
  }
  leftServo.write(leftAngle);

  // -------------------------------------------------------------
  // 3. RIGHT JOINT CONTROL (Pin 7 | Joystick 2 - X axis)
  // -------------------------------------------------------------
  int joy2XVal = analogRead(JOY2_X);
  if (joy2XVal < DEADZONE_LOW) {
    rightAngle = max(30, rightAngle - STEP_SPEED);  // 30° safety limit
  } else if (joy2XVal > DEADZONE_HIGH) {
    rightAngle = min(150, rightAngle + STEP_SPEED); // 150° safety limit
  }
  rightServo.write(rightAngle);

  // -------------------------------------------------------------
  // 4. CLAW CONTROL (Pin 6 | Joystick 2 - Y axis)
  // -------------------------------------------------------------
  int joy2YVal = analogRead(JOY2_Y);
  if (joy2YVal < DEADZONE_LOW) {
    clawAngle = max(10, clawAngle - STEP_SPEED); // Fully closed
  } else if (joy2YVal > DEADZONE_HIGH) {
    clawAngle = min(90, clawAngle + STEP_SPEED); // Fully open
  }
  clawServo.write(clawAngle);

  // Smooth out movement speed
  delay(20);
}