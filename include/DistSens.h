#pragma once
#include <Arduino.h>

#define BOARD UKMARSBOT_V1
const int ENCODER_LEFT_CLK = 2;
const int ENCODER_RIGHT_CLK = 3;
const int ENCODER_LEFT_B = 4;
const int ENCODER_RIGHT_B = 5;
const int MOTOR_LEFT_DIR = 7;
const int MOTOR_RIGHT_DIR = 8;
const int MOTOR_LEFT_PWM = 9;
const int MOTOR_RIGHT_PWM = 10;
const int EMITTER_F = 11;
const int EMITTER_S = 12;
const int SENSOR_0 = A0;
const int SENSOR_1 = A1;
const int SENSOR_2 = A2;
const int SENSOR_3 = A3;
const int SENSOR_4 = A4;
const int SENSOR_5 = A5;
const int FUNCTION_PIN = A6;
const int BATTERY_VOLTS = A7;

uint32_t updateTime;
uint32_t updateInterval = 40; // (ms) do not make this smaller than 25ms for performance reasons

// the default values for the front sensor when the robot is backed up to a wall
const int FRONT_REFERENCE = 44;
// the default values for the side sensors when the robot is centred in a cell
const int LEFT_REFERENCE = 38;
const int RIGHT_REFERENCE = 49;

// the values above which, a wall is seen
const int FRONT_WALL_THRESHOLD = FRONT_REFERENCE / 20; // minimum value to register a wall
const int LEFT_WALL_THRESHOLD = LEFT_REFERENCE / 2;    // minimum value to register a wall
const int RIGHT_WALL_THRESHOLD = RIGHT_REFERENCE / 2;  // minimum value to register a wall

// working copies of the reference values
int gFrontReference = FRONT_REFERENCE;
int gLeftReference = LEFT_REFERENCE;
int gRightReference = RIGHT_REFERENCE;
int gFLeftReference = LEFT_REFERENCE;
int gFRightReference = RIGHT_REFERENCE;
// the current value of the sensors
volatile int gSensorFront;
volatile int gSensorLeft;
volatile int gSensorRight;
volatile int gFSensorLeft;
volatile int gFSensorRight;
// true f a wall is present
volatile bool gFrontWall;
volatile bool gLeftWall;
volatile bool gRightWall;
volatile bool gFLeftWall;
volatile bool gFRightWall;
// steering and turn position errors
volatile int gSensorFrontError; // zero when robot in cell centre
volatile float gSensorCTE;      // zero when robot in cell centre

int counter = 1;

void analogueSetup()
{
  // increase speed of ADC conversions to 28us each
  // by changing the clock prescaler from 128 to 16
  bitClear(ADCSRA, ADPS0);
  bitClear(ADCSRA, ADPS1);
  bitSet(ADCSRA, ADPS2);
}

/***
 * If you are interested in what all this does, the ATMega328P datasheet
 * has all the answers but it is not easy to follow until you have some
 * experience. For now just use the code as it is.
 */
void setupSystick()
{
  // set the mode for timer 2
  bitClear(TCCR2A, WGM20);
  bitSet(TCCR2A, WGM21);
  bitClear(TCCR2B, WGM22);
  // set divisor to 128 => timer clock = 125kHz
  bitSet(TCCR2B, CS22);
  bitClear(TCCR2B, CS21);
  bitSet(TCCR2B, CS20);
  // set the timer frequency
  OCR2A = 249; // (16000000/128/500)-1 = 249
  // enable the timer interrupt
  bitSet(TIMSK2, OCIE2A);
}

void updateSideWallSensor()
{
  // first read them dark
  int right = analogRead(A1);
  int left = analogRead(A2);
  // light them up
  digitalWrite(EMITTER_S, 1);
  // wait until all the detectors are stable
  delayMicroseconds(50);
  // now find the differences
  right = analogRead(A1) - right;
  left = analogRead(A2) - left;
  // and go dark again.
  digitalWrite(EMITTER_S, 0);

  gLeftWall = left > gLeftReference / 2;
  gRightWall = right > gRightReference / 2;
  // make the results available to the rest of the program
  gSensorLeft = left;
  gSensorRight = right;
}

void updateFrontWallSensors()
{
  // first read them dark
  int f_right = analogRead(A0);
  int f_left = analogRead(A3);
  // light them up
  digitalWrite(EMITTER_F, 1);
  // wait until all the detectors are stable
  delayMicroseconds(50);
  // now find the differences
  f_right = analogRead(A0) - f_right;
  f_left = analogRead(A3) - f_left;
  // and go dark again.
  digitalWrite(EMITTER_F, 0);

  gFLeftWall = f_left > gFLeftReference / 2;
  gFRightWall = f_right > gFRightReference / 2;
  // make the results available to the rest of the program
  gFSensorLeft = f_left;
  gFSensorRight = f_right;
}

// the systick event is an ISR attached to Timer 2
ISR(TIMER2_COMPA_vect)
{
  if(counter%2 == 0)
  {
    updateFrontWallSensors();
  }
  if(counter)
  {
    updateSideWallSensor();
  }
  counter++;
}

void dist_init()
{
  pinMode(EMITTER_F, OUTPUT);
  pinMode(EMITTER_S, OUTPUT);
  digitalWrite(EMITTER_F, 0); // be sure the emitter is off
  digitalWrite(EMITTER_S, 0); // be sure the emitter is off
  analogueSetup();          // increase the ADC conversion speed
  setupSystick();
  updateTime = millis() + updateInterval;
}

int dist_get_left()
{
    noInterrupts();
    int result = gSensorLeft;
    interrupts();
    return result;
}

int dist_get_right()
{
   noInterrupts();
    int result = gSensorRight;
    interrupts();
    return result;
}

int dist_get_fleft()
{
    noInterrupts();
    int result = gFSensorLeft;
    interrupts();
    return result;
}

int dist_get_fright()
{
    noInterrupts();
    int result = gFSensorRight;
    interrupts();
    return result;
}