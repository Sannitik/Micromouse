#pragma once

#include "Encoder.h"
#include "Config.h"


float ve_l_w_est = 0;
float ve_l_w_est_f = 0;
float ve_r_w_est = 0;
float ve_r_w_est_f = 0;
float dphi = 0;

void ve_init()
{
}

void ve_l_tick(float phi){
    static float phi_old = phi;
    float dphi = phi - phi_old;
    ve_l_w_est = dphi / Ts_s;
    phi_old = phi;
    static float I = 0;
    float e = ve_l_w_est - ve_l_w_est_f;
    float s = e / VE_LPT_T;
    I += s * Ts_s;
    ve_l_w_est_f = I;
}
float ve_l_get_w_est(){
    return ve_l_w_est;
}

float ve_l_get_w_est_f(){
    return ve_l_w_est_f;
}

void ve_r_tick(float phi){
    static float phi_old = phi;
    float dphi = phi - phi_old;
    ve_r_w_est = dphi / Ts_s;
    phi_old = phi;
    static float I = 0;
    float e = ve_r_w_est - ve_r_w_est_f;
    float s = e / VE_LPT_T;
    I += s * Ts_s;
    ve_r_w_est_f = I;
}
float ve_r_get_w_est(){
    return ve_r_w_est;
}

float ve_r_get_w_est_f(){
    return ve_r_w_est_f;
}