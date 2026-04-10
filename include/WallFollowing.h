#pragma once
#include "Arduino.h"
#include "ASMR.h"
#include "Tipes.h"
#include "Config.h"

float wf_kp_left = 0.08;
float wf_kp_right = -wf_kp_left;
float wf_kp_angle = 4.7;

float wf_straight_tick(SensorData data)
{
    float left = data.dist_left;
    float right = data.dist_right;

    float err_left = MF_LEFT_REFERENCE - left;
    float err_right = MF_RIGHT_REFERENCE - right;
    float err_angle = 0 - data.odom_theta;

    float theta_i0_left = err_left * wf_kp_left;
    float theta_i0_right = err_right * wf_kp_right;
    float theta_i0_angle = err_angle * wf_kp_angle;

    float theta_i0 = 0;
    size_t counter = 0;
    if(data.is_wall_left){
        theta_i0 += theta_i0_left;
        counter++;
    }
    if(data.is_wall_right){
        theta_i0 += theta_i0_right;
        counter++;
    }

    theta_i0 += 5*theta_i0_angle;
    counter+=5;
    
    if(counter != 0){
        theta_i0 /= counter;
    }

    return theta_i0;
}


