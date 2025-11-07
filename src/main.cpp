#include <Arduino.h>
#include <argviz.h>

#include "VoltageSensor.h"
#include "Motor.h"
#include "Encoder.h"
#include "Config.h"
#include "VelEstimator.h"
#include "ServoMotor.h"
#include "Screens.h"
#include "Mixer.h"
#include "Odometer.h"


void setup() {
  Serial.begin(115200);

  m_init();
  vs_init();
  enc_l_init();
  enc_r_init();

  interrupts();

  argviz_init(Serial);
  argviz_registerScreen(0, volts);
  argviz_registerScreen(1, servos);
  argviz_registerScreen(2, mixer);
  argviz_start();
}

void loop() {
    static uint32_t timer = micros();
    while(micros() - timer < Ts_us)
    ;
    timer = micros();

    odom_ticks();

    mixer_tick(v_0, theta_i0);
    
}