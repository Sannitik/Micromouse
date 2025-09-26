#pragma once
#include <Arduino.h>

#define VS_BATTERY_VOLTS_PIN A7

void vs_init(){
    pinMode(VS_BATTERY_VOLTS_PIN, INPUT); 
}

float vs_get_v_batt()
{
    int BATTERY_VOLTS_RAW = analogRead(VS_BATTERY_VOLTS_PIN);
    float BATTERY_VOLTS_V = BATTERY_VOLTS_RAW * 5.0/1024;
    float V_BATT = 2 * BATTERY_VOLTS_V;
    return V_BATT;
}

