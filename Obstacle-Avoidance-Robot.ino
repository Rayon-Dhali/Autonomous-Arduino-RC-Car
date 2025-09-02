/*
   Autonomous Arduino RC Car
   Components:
   - Arduino Uno/Nano
   - L298N Motor Driver
   - Ultrasonic Sensor (HC-SR04)
   - Servo Motor (to rotate ultrasonic sensor)
   - IR sensors optional for line detection 
  

   Functionality:
   The car moves forward until an obstacle is detected (< 20 cm).
   Then it stops, reverses slightly, scans right and left distances,
   compares them, and turns in the direction with more free space.
*/

#include <Servo.h>        // Servo library for rotating ultrasonic sensor
#include <NewPing.h>      // Ultrasonic sensor library for accurate distance measurement

// ---------------- MOTOR DRIVER PINS (L298N) ----------------
const int LeftMotorForward = 4;
const int LeftMotorBackward = 5;
const int RightMotorForward = 6;
const int RightMotorBackward = 7;

// ---------------- ULTRASONIC SENSOR PINS ----------------
#define TRIGGER_PIN  A1  // Trigger pin connected to A1
#define ECHO_PIN     A2  // Echo pin connected to A2
#define MAX_DISTANCE 250 // Max distance limit for ultrasonic sensor (in cm)

// ---------------- OBJECTS ----------------
Servo servo_motor;  // Servo object to rotate the ultrasonic sensor
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// ---------------- GLOBAL VARIABLES ----------------
int distance = 100;  // Stores the last measured distance

// ===================================================
// SETUP: Initialize components
// ===================================================
void setup() {
  // Set motor driver pins as outputs
  pinMode(RightMotorForward, OUTPUT);
  pinMode(LeftMotorForward, OUTPUT);
  pinMode(LeftMotorBackward, OUTPUT);
  pinMode(RightMotorBackward, OUTPUT);

  // Attach servo motor to pin 10 and center it at 115 degrees
  servo_motor.attach(10);
  servo_motor.write(115);
  delay(2000);  // Wait 2 seconds for servo to stabilize

  // Take initial distance readings
  distance = readPing();
  delay(100);
}

// ===================================================
// LOOP: Main decision-making process (Flowchart logic)
// ===================================================
void loop() {
  int distanceRight = 0;
  int distanceLeft = 0;
  delay(50);

  // --------- OBSTACLE DETECTED ---------
  if (distance <= 20) {   // If obstacle is closer than 20 cm
    moveStop();
    delay(300);

    moveBackward();       // Step back slightly
    delay(400);

    moveStop();
    delay(300);

    // Scan right distance
    distanceRight = lookRight();
    delay(300);

    // Scan left distance
    distanceLeft = lookLeft();
    delay(300);

    // Compare right and left distances
    if (distanceRight >= distanceLeft) {
      turnRight();
      delay(300);
      moveStop();
    } else {
      turnLeft();
      delay(300);
      moveStop();
    }
  }
  // --------- NO OBSTACLE: KEEP MOVING FORWARD ---------
  else {
    moveForward();
  }

  // Update distance reading
  distance = readPing();
}

// ===================================================
// HELPER FUNCTIONS
// ===================================================

// Rotate servo to the right, measure distance, then reset
int lookRight() {
  servo_motor.write(50);      // Rotate sensor right
  delay(500);
  int distance = readPing();  // Get distance
  delay(100);
  servo_motor.write(115);     // Reset to center
  return distance;
}

// Rotate servo to the left, measure distance, then reset
int lookLeft() {
  servo_motor.write(180);     // Rotate sensor left
  delay(500);
  int distance = readPing();  // Get distance
  delay(100);
  servo_motor.write(115);     // Reset to center
  return distance;
}

// Get distance from ultrasonic sensor
int readPing() {
  delay(100);                 // Short delay between pings
  int cm = sonar.ping_cm();   // Send ping and return distance in cm
  if (cm == 0) {              // If no obstacle detected, set max range
    cm = 250;
  }
  return cm;
}

// -------- MOTOR CONTROL FUNCTIONS --------

// Stop all motors
void moveStop() {
  digitalWrite(RightMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(LeftMotorBackward, LOW);
}

// Move forward
void moveForward() {
  digitalWrite(RightMotorForward, HIGH);
  digitalWrite(RightMotorBackward, LOW);
  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(LeftMotorBackward, LOW);
}

// Move backward
void moveBackward() {
  digitalWrite(RightMotorForward, LOW);
  digitalWrite(RightMotorBackward, HIGH);
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(LeftMotorBackward, HIGH);
}

// Turn right
void turnRight() {
  digitalWrite(RightMotorForward, LOW);
  digitalWrite(RightMotorBackward, HIGH);
  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(LeftMotorBackward, LOW);
}

// Turn left
void turnLeft() {
  digitalWrite(RightMotorForward, HIGH);
  digitalWrite(RightMotorBackward, LOW);
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(LeftMotorBackward, HIGH);
}
