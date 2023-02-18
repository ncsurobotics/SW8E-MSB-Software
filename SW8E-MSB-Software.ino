#include <Wire.h>   
#include "pinmap.h"

void setup() {

}

void loop() {
  digitalWrite(LED_GRN, HIGH);

  digitalWrite(DROP1_CTRL, HIGH);
  digitalWrite(DROP2_CTRL, HIGH);
  
  digitalWrite(SERVO1_CTRL, HIGH);
  digitalWrite(SERVO2_CTRL, HIGH);

  delay(500);

  digitalWrite(LED_GRN, LOW);

  digitalWrite(DROP1_CTRL, LOW);
  digitalWrite(DROP2_CTRL, LOW);
  
  digitalWrite(SERVO1_CTRL, LOW);
  digitalWrite(SERVO2_CTRL, LOW);

  delay(500);
}
