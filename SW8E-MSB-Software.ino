#include <Wire.h>
#include <Servo.h>   
#include "pinmap.h"


//------------------
// COMMAND STRUCTURE
//------------------
#define RESET             0         // Reset both droppers and torpedos
#define D1_TRIG           1         // Trigger dropper 1 (drop)
#define D2_TRIG           2         // Trigger dropper 2 (drop)
#define SERVO1_TRIG       3         // Trigger torpedo 1 (fire)
#define SERVO2_TRIG       4         // Trigger torpedo 2 (fire)


// Servo positions for torpedos
#define SERVO_RESET_POS 50
#define SERVO_TRIG_POS 20

// Dropper states
#define DROPPER_RESET_STATE HIGH
#define DROPPER_TRIG_STATE LOW


// I2C slave address for MSB
#define ADDRESS 0x20 // the 7-bit slave address


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
bool reset_received, drop1_received, drop2_received, servo1_received, servo2_received;
unsigned long led_off_time = 0;


void on_i2c_receive(int bytes) {
  digitalWrite(LED_GRN, HIGH);
  led_off_time = millis() + 100;
  if(led_off_time == 0)
    led_off_time = 1;
  while(Wire.available()) {
    switch(Wire.read()) {
        case RESET:
            reset_received = true;
            break;
        case D1_TRIG:
            drop1_received = true;
            break;
        case D2_TRIG:
            drop2_received = true;
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
    Wire.onReceive(on_i2c_receive);
    
    reset_received = true;          // Reset on first run of loop()
    drop1_received = false;
    drop2_received = false;
    servo1_received = false;
    servo2_received = false;
}

void loop() {
    
    // Turn LED off after blinking to indicate command byte received
    if(millis() > led_off_time && led_off_time != 0){
      led_off_time = 0;
      digitalWrite(LED_GRN, LOW);
    }

    if(reset_received) {
        torpedo_servo1.write(SERVO_RESET_POS);
        torpedo_servo2.write(SERVO_RESET_POS);
        digitalWrite(DROP1_CTRL, DROPPER_RESET_STATE);
        digitalWrite(DROP2_CTRL, DROPPER_RESET_STATE);
        reset_received = false;
    }
    
    if(drop1_received) {
      digitalWrite(DROP1_CTRL, DROPPER_TRIG_STATE);
      drop1_received = false;
    }
    
    if(drop2_received) {
      digitalWrite(DROP2_CTRL, DROPPER_TRIG_STATE);
      drop2_received = false;
    }
    
    if(servo1_received) {
      torpedo_servo1.write(SERVO_TRIG_POS);
      servo1_received = false;        
    }
    
    if(servo2_received) {
      torpedo_servo2.write(SERVO_TRIG_POS); 
      servo2_received = false;
    }
}
