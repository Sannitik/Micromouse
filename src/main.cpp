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

    interrupts();

    argviz_init(Serial);
    argviz_registerScreen(3, volts);
    argviz_registerScreen(1, encoders);
    argviz_registerScreen(0, mixer);
    argviz_registerScreen(2, asmr);
    argviz_registerScreen(4, dist_sens);
    // argviz_start();

    maze_set_wall(Vec2{1, 2}, Maze::CellWalls{Maze::WALL, Maze::WALL, Maze::WALL, Maze::OPEN});
    maze_set_wall(Vec2{3, 0}, Maze::CellWalls{Maze::OPEN, Maze::WALL, Maze::OPEN, Maze::OPEN});
    maze_set_wall(Vec2{4, 1}, Maze::CellWalls{Maze::WALL, Maze::WALL, Maze::WALL, Maze::OPEN});
    maze_set_wall(Vec2{0, 1}, Maze::CellWalls{Maze::WALL, Maze::WALL, Maze::WALL, Maze::OPEN});
    maze_set_wall(Vec2{2, 2}, Maze::CellWalls{Maze::OPEN, Maze::OPEN, Maze::OPEN, Maze::WALL});
    maze_set_wall(Vec2{2, 0}, Maze::CellWalls{Maze::OPEN, Maze::WALL, Maze::WALL, Maze::OPEN});
    maze_set_wall(Vec2{3, 2}, Maze::CellWalls{Maze::WALL, Maze::WALL, Maze::OPEN, Maze::OPEN});
    maze_set_wall(Vec2{4, 3}, Maze::CellWalls{Maze::WALL, Maze::WALL, Maze::WALL, Maze::OPEN});
    maze_set_wall(Vec2{5, 1}, Maze::CellWalls{Maze::OPEN, Maze::WALL, Maze::OPEN, Maze::WALL});
    maze_set_wall(Vec2{5, 0}, Maze::CellWalls{Maze::OPEN, Maze::OPEN, Maze::WALL, Maze::WALL});
    maze_set_wall(Vec2{1, 0}, Maze::CellWalls{Maze::OPEN, Maze::OPEN, Maze::WALL, Maze::OPEN});
    maze_set_wall(Vec2{2, 1}, Maze::CellWalls{Maze::OPEN, Maze::OPEN, Maze::WALL, Maze::OPEN});
    maze_set_wall(Vec2{0, 3}, Maze::CellWalls{Maze::WALL, Maze::WALL, Maze::OPEN, Maze::OPEN});
    maze_set_wall(Vec2{2, 3}, Maze::CellWalls{Maze::OPEN, Maze::WALL, Maze::OPEN, Maze::OPEN});
    maze_set_wall(Vec2{5, 2}, Maze::CellWalls{Maze::OPEN, Maze::OPEN, Maze::WALL, Maze::WALL});
    


    //maze_set_wall(Vec2{0, 1}, Maze::CellWalls{Maze::WALL, Maze::WALL, Maze::WALL, Maze::OPEN});
    
    Serial.println();
    draw_maze(MAZE_WIDTH, MAZE_HEIGHT);

    solver_solve(Vec2{0, 0}, Vec2{4, 3});

    draw_maze_with_solver(MAZE_WIDTH, MAZE_HEIGHT);

    while(1);
}

void loop()
{
    // Timer
    static uint32_t timer = micros();
    while (micros() - timer < Ts_us)
        ;
    timer = micros();

    // asmr_tick();
}