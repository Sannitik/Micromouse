#pragma once

#include "VoltageSensor.h"
#include "Motor.h"
#include "Encoder.h"
#include "Config.h"
#include "VelEstimator.h"
#include "ServoMotor.h"
#include "Screens.h"
#include "Mixer.h"
#include "Odometer.h"


int left_u = 0;
int right_u = 0;
int left_w0 = 0;
int right_w0 = 0;

SCREEN(servos, 
  {
    CLICK_ROW([](CLICK_STATE state){
        switch(state)
        {
        case CLICK_LEFT:
            left_w0--;
            break;
        case CLICK_RIGHT:
            left_w0++;
            break;
        case CLICK_DOWN:
            left_w0 = 0;
            break;
        default:
            break;
        }
    },
    "left_w0 = %s", String(left_w0).c_str());
    ROW("right_w: %s", String(ve_r_get_w_est_f()).c_str())

    CLICK_ROW([](CLICK_STATE state){
        switch(state)
        {
        case CLICK_LEFT:
            right_w0--;
            break;
        case CLICK_RIGHT:
            right_w0++;
            break;
        case CLICK_DOWN:
            right_w0 = 0;
            break;
        default:
            break;
        }
    }, 
    "right_w0 = %s", String(right_w0).c_str());
    ROW("left_w: %s", String(ve_l_get_w_est_f()).c_str())
})
    
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

float v_0 = 0;
float theta_i0 = 0;
SCREEN(mixer, 
  {
    CLICK_ROW([](CLICK_STATE state){
        switch(state)
        {
        case CLICK_LEFT:
            v_0-=0.1;
            break;
        case CLICK_RIGHT:
            v_0+=0.1;
            break;
        case CLICK_DOWN:
            v_0 = 0;
            break;
        default:
            break;
        }
    },
    "v_0 = %s", String(v_0).c_str());
    CLICK_ROW([](CLICK_STATE state){
        switch(state)
        {
        case CLICK_LEFT:
            theta_i0--;
            break;
        case CLICK_RIGHT:
            theta_i0++;
            break;
        case CLICK_DOWN:
            theta_i0 = 0;
            break;
        default:
            break;
        }
    }, 
    "theta_i0 = %s", String(theta_i0).c_str());
    ROW("odom_S: %s", String(odom_S).c_str());
    ROW("odom_theta: %s", String(odom_theta).c_str());
})