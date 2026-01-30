#include <Arduino.h>
#include <argviz.h>

#include "VoltageSensor.h"
#include "Motor.h"
#include "Encoder.h"
#include "Config.h"
#include "VelEstimator.h"
#include "ServoMotor.h"
#include "Screens.h"
#include "Mixer.h"
#include "Odometer.h"
#include "ASMR.h"
#include "WallSens"

void setup()
{
    Serial.begin(115200);
    
    m_init();
    vs_init();
    enc_l_init();
    enc_r_init();
    asmr_init();
    analogueSetup();
    setupSystick();

    interrupts();

    argviz_init(Serial);
    argviz_registerScreen(3, volts);
    argviz_registerScreen(1, encoders);
    argviz_registerScreen(0, mixer);
    argviz_registerScreen(3, asmr);
    argviz_registerScreen(4, walls);
    argviz_start();
}

void loop()
{
    // Timer
    static uint32_t timer = micros();
    while (micros() - timer < Ts_us)
        ;
    timer = micros();

    // Sense
    // odom_tick();
    // Plan
    // Act
    asmr_tick();
}