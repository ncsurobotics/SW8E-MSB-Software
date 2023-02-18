//------------------------------------------------
// MACROS
//------------------------------------------------

// Pinmap for FR2355 launchpad

#define DROP1_CTRL P2_0
#define DROP2_CTRL  P2_2

#define SERVO1_CTRL P6_0
#define SERVO2_CTRL P6_1

#define LED_GRN P6_2

// Note: Reserved P1_2 & P1_3 for I2C ("Wire"); P4_6 & P4_7 shared with SPI ("Wire1")
// Note: Serial P4_2 & P4_3 are connected to backchannel UART (debug header), Serial1 P1_6 & P1_7
// Note: Reserved 4_5, 4_6, & 4_7 for SPI (Not used in this board)

/* Layout of the 2 - 10 pin headers.

   All pins support digitalWrite() and attachInterrupt()
   Pins marked with PWM support anaglogWrite()
   Pins marked with Ax support analogRead()

                                          +---\/---+
                                   VCC0  1|        |21  5.0V
                            (A5)   P1.5  2|        |22  GND
(PWM) (UCA0RXD -> Serial1)         P1.6  3|        |23  P1.4            (A4) 
(PWM) (UCA0TXD -> Serial1)         P1.7  4|        |24  P5.3            (A11)
                                   P3.6  5|        |25  P5.1            (A9)
(PWM)                       (A10)  P5.2  6|        |26  P5.0            (A8)
(PWM) (SCLK)                       P4.5  7|        |27  P5.4
                                   P3.4  8|        |28  P1.1            (A1)
(PWM) (SCL)                        P1.3  9|        |29  P3.5            (OA3O)
(PWM) (SDA)                        P1.2 10|        |30  P3.1            (OA2O)
                                          +--------+

                      
                      
                                          +---\/---+
(PWM)                              P2.1 40|        |20  GND
(PWM)                              P6.0 39|        |19  P2.0                 (PWM)
(PWM)                              P6.1 38|        |18  P2.2                 (PWM)
(PWM)                              P6.2 37|        |17  P4.0
(PWM)                              P6.3 36|        |16  RST
(PWM)                              P6.4 35|        |15  P4.6 (SDA) (MOSI)
                                   P3.7 34|        |14  P4.7 (SCL) (MISO)
                                   P2.4 33|        |13  P4.4
                     (OA2+)        P3.3 32|        |12  P2.5
                     (OA2-)        P3.2 31|        |11  P3.0
                                          +--------+
                      
                      

                                               ----+
                                                   |41  DebugUART TX  (P4.3)
                     J13 eZ-FET Connector          |42  DebugUART RX  (P4.2)
                                               ----+
                                               ----+
                                                   |43  LED1       (P1.0)
                     Extra LED's and Switches      |44  LED2       (P6.6)
                                                   |45  PUSH1      (P4.1)
                                                   |46  PUSH2      (P2.3)
                                               ----+
*/
