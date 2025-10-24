#pragma once

#include "Config.h"
#include "Encoder.h"
#include "Motor.h"
#include "VelEstimator.h"

float servo_pi(float err){
    static float I = 0;

    float up = err * SERVO_KP;
    float ui = I;

    float u = up + ui;

    I += err * SERVO_KI * Ts_s;

    return u;
}

void servo_tick_left(float w0_left){
    enc_l_tick();

    ve_l_tick(enc_l_get_phi());
    float w_left = ve_l_get_w_est();
    float err_left = w0_left - w_left;
    float u_left = servo_pi(err_left);

    m_driver(u_left, 0);
}

void servo_tick_right(float w0_right){
    enc_r_tick();

    ve_r_tick(enc_r_get_phi());
    float w_right = ve_r_get_w_est();
    float err_right = w0_right - w_right;
    float u_right = servo_pi(err_right);

    m_driver(u_right, 0);
}