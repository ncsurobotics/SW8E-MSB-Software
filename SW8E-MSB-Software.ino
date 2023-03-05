#include <Wire.h>
#include <Servo.h>   
#include "pinmap.h"

// IDs for Identifying which tool to select
#define DROP1_ID    1
#define DROP2_ID    2
#define SERVO1_ID   1
#define SERVO2_ID   2

//------------------
// COMMAND STRUCTURE
//------------------
#define RESET             0
#define DROP_ENG_TRIG     1
#define DROP_DISENG_TRIG  2
#define SERVO1_TRIG       3
#define SERVO2_TRIG       4

#define ADDRESS 0x20 // the 7-bit slave address
#define OFF false
#define ON  true

// Don't try to enable LXFT1.
// This oscillator isn't connected on these boards
// and on the FR2433 cuases thrusters to arm during the 2 seconds
// energia tries to do so.
extern volatile uint16_t vlo_freq;
void enableXtal(void)
{
#if (!defined(__MSP430FR2XX_4XX_FAMILY__) && (defined(__MSP430_HAS_CS__) || defined(__MSP430_HAS_CS_A__))) 
    /* ACLK = VLO = ~ 12 KHz */
    vlo_freq = 8000;
    /* Source ACLK from VLO */
    CSCTL2 |= SELA__VLOCLK;
  }
#endif
#if (defined(__MSP430FR2XX_4XX_FAMILY__) && (defined(__MSP430_HAS_CS__) || defined(__MSP430_HAS_CS_A__)))
  /* ACLK = REFO = ~ 32 KHz */
  vlo_freq = 32768;
  /* Source ACLK from REFO */
  CSCTL4 |= SELA__REFOCLK;
  CSCTL3 |= SELREF__REFOCLK;


  /* Clear the Oscillator fault interrupt flag */
  CSCTL7 &= ~(DCOFFG|XT1OFFG|FLLULIFG);
  SFRIFG1 &= ~OFIFG;
#endif

#ifdef __MSP430_HAS_BC2__
  /* ACLK = VLO = ~ 12 KHz */
  vlo_freq = 12000;
  /* No XTAL present. Default P2.6/7 to GPIO */
  P2SEL &= ~(BIT6|BIT7);
  /* Source ACLK from VLO */
  BCSCTL3 |= LFXT1S_2;
#endif

#if defined(__MSP430_HAS_UCS__)
  UCSCTL6 &= ~(XT1DRIVE_3);
#endif
}

//-------------------------------
// GLOBAL VARIABLES
//-------------------------------
Servo torpedo_servo1, torpedo_servo2;
bool reset_received, drop_eng_received, drop_diseng_received, servo1_received, servo2_received;

//-------------------------------
// SETUP FUNCTION
//-------------------------------
void setup() {
    pinMode(DROP1_CTRL, OUTPUT);
    pinMode(DROP2_CTRL, OUTPUT);
    pinMode(LED_GRN, OUTPUT);
    pinMode(SERVO1_CTRL, OUTPUT);
    pinMode(SERVO2_CTRL, OUTPUT);
    
    digitalWrite(DROP1_CTRL, LOW);
    digitalWrite(DROP2_CTRL, LOW);
    digitalWrite(SERVO1_CTRL, LOW);
    digitalWrite(SERVO2_CTRL, LOW);
    digitalWrite(LED_GRN, LOW);     //currently used as a debug LED
  
    torpedo_servo1.attach(SERVO1_CTRL);
    torpedo_servo2.attach(SERVO2_CTRL);
    
    Wire.begin(ADDRESS);
    Wire.onReceive(task_receive_message);
    
    reset_received = false;
    drop_eng_received = false;
    drop_diseng_received = false;
    servo1_received = false;
    servo2_received = false;
}

//-------------------------------
// TASK FUNCTIONS
//-------------------------------

// TODO change this to a bitmap for flags
void task_receive_message(int bytes) {
  digitalWrite(LED_GRN, HIGH);
  while(Wire.available()) {
    switch(Wire.read()) {
        case RESET:
            reset_received = true;
            break;
        case DROP_ENG_TRIG:
            drop_eng_received = true;
            break;
        case DROP_DISENG_TRIG:
            drop_diseng_received = true;
            break;
        case SERVO1_TRIG:
            servo1_received = true;
            break;
        case SERVO2_TRIG:
            servo2_received = true;
            break;
        default:
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
    
    reset_received = false;
    drop_eng_received = false;
    drop_diseng_received = false;
    servo1_received = false;
    servo2_received = false;
}

void task_torpedo_servo(char servo) {
    switch(servo) {
        case SERVO1_ID:
            torpedo_servo1.write(45);
            digitalWrite(LED_GRN, HIGH);
        case SERVO2_ID:
            torpedo_servo2.write(45);
            digitalWrite(LED_GRN, HIGH);
            break;
        default:
            break; 
    }
}

void task_dropper_ctrl(char direction) {
  if (direction) {
    digitalWrite(DROP1_CTRL, HIGH);
    digitalWrite(DROP2_CTRL, HIGH);
    digitalWrite(LED_GRN, HIGH);
  } else {
    digitalWrite(DROP1_CTRL, LOW);
    digitalWrite(DROP2_CTRL, LOW);
    digitalWrite(LED_GRN, OFF);
  }

}

void loop() {
    
    if(reset_received) {
        task_reset();
        reset_received = false;
    }
    
    if(drop_eng_received) {
        task_dropper_ctrl(ON);
        drop_eng_received = false;
    }
    
    if(drop_diseng_received) {
        task_dropper_ctrl(OFF); 
        drop_diseng_received = false;
    }
    
    if(servo1_received) {
        task_torpedo_servo(SERVO1_ID); 
        servo1_received = false;        
    }
    
    if(servo2_received) {
        task_torpedo_servo(SERVO2_ID); 
        servo2_received = false;
    }
}
