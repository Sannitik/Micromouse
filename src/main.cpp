#include <Arduino.h>
#include <argviz.h>

#include "VoltageSensor.h"
#include "Motor.h"
#include "Encoder.h"
#include "Config.h"
#include "VelEstimator.h"

int left_u = 0;
int right_u = 0;


SCREEN(volts, 
  {
    ROW("Vbatt [mV]: %d", int(vs_get_v_batt() * 1000));
    CLICK_ROW([](CLICK_STATE state){
        switch(state)
        {
        case CLICK_LEFT:
            left_u--;
            break;
        case CLICK_RIGHT:
            left_u++;
            break;
        case CLICK_DOWN:
            left_u = 0;
            break;
        default:
            break;
        }
    }, 
    "left_u = %d", left_u);

    CLICK_ROW([](CLICK_STATE state){
        switch(state)
        {
        case CLICK_LEFT:
            right_u--;
            break;
        case CLICK_RIGHT:
            right_u++;
            break;
        case CLICK_DOWN:
            right_u = 0;
            break;
        default:
            break;
        }
    }, 
    "right_u = %d", right_u);
})

SCREEN(encoders, 
  {
    ROW("Left phi[mrad]: %d", (int)(enc_l_get_phi() * 1000));
    ROW("Left w[mrad/s]: %d", (int)(ve_l_get_w_est() * 1000));
    ROW("Left wf[mrad/s]: %d", (int)(ve_l_get_w_est_f() * 1000));
    ROW("Right phi[mrad]]: %d", (int)(enc_r_get_phi() * 1000));
    ROW("Right w[mrad/s]: %d", (int)(ve_r_get_w_est() * 1000));
    ROW("Right wf[mrad/s]: %d", (int)(ve_r_get_w_est_f() * 1000));
})

void setup() {
  Serial.begin(115200);

  m_init();
  vs_init();
  enc_l_init();
  enc_r_init();

  interrupts();

  argviz_init(Serial);
  argviz_registerScreen(0, volts);
  argviz_registerScreen(1, encoders);
  argviz_start();
}

void loop() {
    static uint32_t timer = micros();
    while(micros() - timer < Ts_us)
    ;
    timer = micros();
    enc_l_tick();
    enc_r_tick();

    ve_l_tick(enc_l_get_phi());
    ve_r_tick(enc_r_get_phi());

    m_driver(left_u, right_u);
}