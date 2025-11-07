#pragma once

#include "Config.h"
#include "Encoder.h"
#include "Motor.h"
#include "VelEstimator.h"

float servo_left_pi(float err)
{
    static float I = 0;

    float up = err * SERVO_KI;
    float ui = I;

    float u = up + ui;

    I += err * SERVO_KI * Ts_s;

    return u;
}

float servo_right_pi(float err)
{
    static float I = 0;

    float up = err * SERVO_KI;
    float ui = I;

    float u = up + ui;

    I += err * SERVO_KI * Ts_s;

    return u;
}

void servo_tick(float w0_left, float w0_right){
       enc_l_tick();
    enc_r_tick();

    ve_l_tick(enc_l_get_phi());
    ve_r_tick(enc_r_get_phi());

    float w_left = ve_l_get_w_est_f();
    float err_left = w0_left - w_left;
    float u_left = servo_left_pi(err_left);

    float w_right = ve_r_get_w_est_f();
    float err_right = w0_right - w_right;
    float u_right = servo_right_pi(err_right);

    m_driver(u_left, u_right);
}
