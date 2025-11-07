#pragma once

#include "Config.h"
#include "ServoMotor.h"

void mixer_init()
{

}

void mixer_tick(float v_0, float theta_i0)
{
    float v_l0 = v_0 - theta_i0 * ROBOT_WIDTH / 2;
    float v_r0 = v_0 + theta_i0 * ROBOT_WIDTH / 2;

    float w_l0 = v_l0 / ROBOT_WHEEL_RADIUS;
    float w_r0 = v_r0 / ROBOT_WHEEL_RADIUS;

    servo_tick(w_l0, w_r0);
}