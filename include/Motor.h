#pragma once
#include <Arduino.h>
#include <VoltageSensor.h>

#define M_LEFT_DIR_PIN 7
#define M_LEFT_PWM_PIN 9
#define M_RIGHT_DIR_PIN 8
#define M_RIGHT_PWM_PIN 10

void m_init(){
    pinMode(M_LEFT_DIR_PIN, OUTPUT);
    pinMode(M_LEFT_PWM_PIN, OUTPUT);
    pinMode(M_RIGHT_DIR_PIN, OUTPUT);
    pinMode(M_RIGHT_PWM_PIN, OUTPUT);
}

void m_driver(float u_l, float u_r)
{
    int left_dir = u_l > 0;
    int left_pwm = constrain(255 * fabs(u_l) / vs_get_v_batt(), 0, 255);

    digitalWrite(M_LEFT_DIR_PIN, left_dir);
    analogWrite(M_LEFT_PWM_PIN, left_pwm);

    int right_dir = u_r < 0;
    int right_pwm = constrain(255 * fabs(u_r) / vs_get_v_batt(), 0, 255);

    digitalWrite(M_RIGHT_DIR_PIN, right_dir);
    analogWrite(M_RIGHT_PWM_PIN, right_pwm);

}


