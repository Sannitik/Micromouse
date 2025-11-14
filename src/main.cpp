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

void stop(){
  odom_reset();
  while (true)
  {
  static uint32_t timer = micros();
    while(micros() - timer < Ts_us)
    ;
    timer = micros();

    odom_ticks();

    float v_0 = 0;
    float theta_i0 = 0;

    mixer_tick(v_0, theta_i0);
  }
}

void left(){
  odom_reset();
  while (true)
  {
    static uint32_t timer = micros();
    while(micros() - timer < Ts_us)
    ;
    timer = micros();

    odom_ticks();

    float v_0 = 0;
    float theta_i0 = MAX_ANG_VEL;

    if(odom_get_theta() > M_PI / 2)
    {
      return;
    }

    mixer_tick(v_0, theta_i0);
  }
    
}

void fwd(){
  odom_reset();
  while (true)
  {
    static uint32_t timer = micros();
    while(micros() - timer < Ts_us)
    ;
    timer = micros();

    odom_ticks();

    float v_0 = MAX_VEL;
    float theta_i0 = 0;

    if(odom_get_S() > CELL_WIDTH){
      return;
    }

    mixer_tick(v_0, theta_i0);
  }
    
}

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
  // argviz_start();

  fwd();
  left();
  fwd();
  left();
  fwd();
  left();
  fwd();
  left();
  stop();
}

void loop() {
    static uint32_t timer = micros();
    while(micros() - timer < Ts_us)
    ;
    timer = micros();

    odom_ticks();

    mixer_tick(v_0, theta_i0);
    
}