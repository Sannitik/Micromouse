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
#include "DistSens.h"
#include "WallFollowing.h"
#include "Maze.h"
#include "MazeDrawer.h"
#include "Tests.h"
#include "Navigator.h"

void setup()
{
    Serial.begin(115200);
    
    m_init();
    vs_init();
    enc_l_init();
    enc_r_init();
    asmr_init();
    dist_init();
    maze_init();
    solver_init();
    nav_init();

    interrupts();

    argviz_init(Serial);
    argviz_registerScreen(3, volts);
    argviz_registerScreen(1, encoders);
    argviz_registerScreen(0, mixer);
    argviz_registerScreen(2, asmr);
    argviz_registerScreen(4, dist_sens);
    //argviz_start();

    // test_router();
}

void loop()
{
    // Timer
    static uint32_t timer = micros();
    while (micros() - timer < Ts_us)
        ;
    timer = micros();

    asmr_tick();
}