/**
 * MMSX21-VT24-12
 * Systems design of an insectoid propulsion system
 * ESP32
 * WEMOS LOLIN32
 * 
 * Elias Hedberg 
 */

#include "Arduino.h"

// Pins for the L293D
#define IN1 12
#define IN2 14
#define ENA 13

// Constants
const float pi = 3.1415926535897932384626433832795;

// PWM settings
const int PWM_CH = 0; // ESP32 pwm channel 0-15
const int PWM_FREQ = 40000; // PWM Freq. Keep over human hearing freq to avoid noise
const int PWM_RES = 8; // 8 bits, 0-255, ESP32 max 16 bits

// Variables to build the sinus wave approximation using PWM
const int PERIOD = 400 ; // ms
const int MAX_DUTY_CYCLE = (int)(pow(2, PWM_RES) - 1);
const int DELAY = PERIOD / MAX_DUTY_CYCLE;
const float PRECISE_DELAY = PERIOD / MAX_DUTY_CYCLE;

int tick = 0;

void setup()
{
  // Assign pins.
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  ledcSetup(PWM_CH, PWM_FREQ, PWM_RES);
  ledcAttachPin(ENA, PWM_CH);
}



void loop()
{
  // Select direction to actuate the VCA
  if (tick > MAX_DUTY_CYCLE/2) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  }
  else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }
  // Actuate with PWM (!TODO this formula is probably totally wrong)
  ledcWrite(PWM_CH, abs(sin(2 * 3.14 * tick * PRECISE_DELAY / PERIOD) * MAX_DUTY_CYCLE));
  
  // PWM increment
  if (tick <= MAX_DUTY_CYCLE) {
    tick++;
  }
  else {
    tick = 0;
  }

  // Delay to achive desired period of the sine wave
  delay(2);
}