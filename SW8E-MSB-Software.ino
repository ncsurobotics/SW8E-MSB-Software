#include <Wire.h>
#include <Servo.h>   
#include "pinmap.h"

#define DROP1_ID    1
#define DROP2_ID    2
#define SERVO1_ID   1
#define SERVO2_ID   2

// COMMAND STRUCTURE
#define RESET       0
#define DROP1_TRIG  1
#define DROP2_TRIG  2
#define SERVO1_TRIG 3
#define SERVO2_TRIG 4

#define QUEUE_SIZE  8
#define ADDRESS 0000000 // the 7-bit slave address

//-------------------------------
// GLOBAL VARIABLES
//-------------------------------
Servo torpedo_servo1, torpedo_servo2;
char message_queue[QUEUE_SIZE];
char* message_queue_write_ptr;
char* message_queue_read_ptr;

bool reset_received, drop1_received, drop2_received, servo1_received, servo2_received;

//-------------------------------
// SETUP FUNCTION
//-------------------------------
void setup() {
    pinMode(DROP1_CTRL, OUTPUT);
    pinMode(DROP2_CTRL, OUTPUT);
    
    digitalWrite(DROP1_CTRL, LOW);
    digitalWrite(DROP2_CTRL, LOW);
    digitalWrite(SERVO1_CTRL, LOW);
    digitalWrite(SERVO2_CTRL, LOW);
    digitalWrite(LED_GRN, LOW);     //currently used as a debug LED
  
    torpedo_servo1.attach(SERVO1_CTRL);
    torpedo_servo2.attach(SERVO2_CTRL);
    
    Wire.begin(ADDRESS);
    
    message_queue_write_ptr = message_queue;
    message_queue_read_ptr = message_queue;
    
    for(int i = 0; i < QUEUE_SIZE; i++) {
        message_queue[i] = '\0'; 
    }
    
    reset_received = false;
    drop1_received = false;
    drop2_received = false;
    servo1_received = false;
    servo2_received = false;
}

//-------------------------------
// TASK FUNCTIONS
//-------------------------------
void task_receive_message(int bytes) {
  while(Wire.available()) {
    switch(Wire.read()) {
        case 0:
            reset_received = true;
            break;
        case 1:
            drop1_received = true;
            break;
        case 2:
            drop2_received = true;
            break;
        case 3:
            servo1_received = true;
            break;
        case 4:
            
        default:
          digitalWrite(LED_GRN, HIGH);
          break;
    } 
  }
}

void task_reset() {
    digitalWrite(DROP1_CTRL, LOW);
    digitalWrite(DROP2_CTRL, LOW);
    digitalWrite(SERVO1_CTRL, LOW);
    digitalWrite(SERVO2_CTRL, LOW);
    digitalWrite(LED_GRN, LOW);     //currently used as a debug LED
    
    message_queue_write_ptr = message_queue;
    message_queue_read_ptr = message_queue;
    
    for(int i = 0; i < QUEUE_SIZE; i++) {
        message_queue[i] = '\0'; 
    }
    
    reset_received = false;
    drop1_received = false;
    drop2_received = false;
    servo1_received = false;
    servo2_received = false;
}

void task_torpedo_servo(char servo) {
    switch(servo) {
        case SERVO1_ID:
            torpedo_servo1.write(45);
        case SERVO2_ID:
            torpedo_servo2.write(45);
            break;
        default:
            digitalWrite(LED_GRN, HIGH);
            break; 
    }
}

void task_dropper_ctrl(char dropper) {
    switch(dropper) {
        case DROP1_ID:
            digitalWrite(DROP2_CTRL, HIGH);
            break;
        case DROP2_ID:
            digitalWrite(DROP2_CTRL, HIGH);
            break;
        default:
            digitalWrite(LED_GRN, HIGH);
            break;
    }
}

void loop() {
    Wire.onReceive(task_receive_message(1));
    
    if(reset_received) {
        task_reset();
    }
    
    if(drop1_received) {
        task_dropper_ctrl(DROP1_ID); 
    }
    
    if(drop2_received) {
        task_dropper_ctrl(DROP2_ID); 
    }
    
    if(servo1_received) {
        task_torpedo_servo(SERVO1_ID); 
    }
    
    if(servo2_received) {
        task_torpedo_servo(SERVO2_ID); 
    }
}
