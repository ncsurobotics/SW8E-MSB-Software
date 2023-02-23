#include <Wire.h>
#include <Servo.h>   
#include "pinmap.h"

//-------------------------------
// GLOBAL VARIABLES
Servo torpedo_servo1, torpedo_servo2;
//-------------------------------

// SETUP FUNCTIONS
//-------------------------------
//-------------------------------
// DROPPER SETUP FUNCTION
void dropper_setup() {
  digitalWrite(DROP1_CTRL, HIGH);
  digitalWrite(DROP2_CTRL, HIGH);
}
//-------------------------------

//-------------------------------
// TORPEDO SERVO SETUP FUNCTION
void torpedo_servo_setup() {
  torpedo_servo1.attatch(SERVO1_CTRL);
  torpedo_servo2.attatch(SERVO2_CTRL);
}
//-------------------------------

//-------------------------------
// FULL BOARD SETUP FUNCTION
void setup() {
  dropper_setup();
  torpedo_servo_setup();
}
//-------------------------------
//-------------------------------

// GENERAL RUNTIME TASK FUNCTIONS
//-------------------------------
//-------------------------------
// 
void task_torpedo_servo() {
  
}
//-------------------------------

//-------------------------------
//
void loop() {
  
}
//-------------------------------
//-------------------------------