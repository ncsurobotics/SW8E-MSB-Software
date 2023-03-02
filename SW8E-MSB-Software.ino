#include <Wire.h>
#include <Servo.h>   
#include "pinmap.h"

#define DROP1_ID    1
#define DROP2_ID    2
#define SERVO1_ID   1
#define SERVO2_ID   2

#define QUEUE_SIZE  8
#define ADDRESS _______ // the 7-bit slave address

//-------------------------------
// GLOBAL VARIABLES
//-------------------------------
Servo torpedo_servo1, torpedo_servo2;
char message_queue[QUEUE_SIZE];
char* message_queue_write_ptr;
char* message_queue_read_ptr;

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
  
    torpedo_servo1.attatch(SERVO1_CTRL);
    torpedo_servo2.attatch(SERVO2_CTRL);
    
    Wire.begin(ADDRESS);
    
    message_queue_write_ptr = message_queue;
    message_queue_read_ptr = message_queue;
    
    for(int i = 0; i < QUEUE_SIZE; i++) {
        message_queue[i] = '\0'; 
    }
}

//-------------------------------
// TASK FUNCTIONS
//-------------------------------
void task_recieve_message(int bytes) {
    while(Wire.available) {
        
    }
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
    Wire.onRecieve(task_recieve_message(1));
}
