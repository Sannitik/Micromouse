#pragma once

#include "Config.h"
#include "VelEstimator.h"

float odom_S = 0;
float odom_theta = 0;

void odom_reset(){
    odom_S = 0;
    odom_theta = 0;
}

void odom_ticks(){
    float w_l = ve_l_get_w_est_f();
    float w_r = ve_r_get_w_est_f();

    float v_l = w_l * ROBOT_WHEEL_RADIUS;
    float v_r = w_r * ROBOT_WHEEL_RADIUS;
    
    float v = (v_l + v_r)/2;
    float theta_i0 = (v_l - v_r) / ROBOT_WIDTH;

    odom_S += v * Ts_s;
    odom_theta += theta_i0 * Ts_s;
}