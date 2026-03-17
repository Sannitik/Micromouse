#pragma once

#define Ts_us 2000
#define Ts_s (Ts_us / 1000000.0)

#define VE_LPT_T 0.005

#define SERVO_KP 3.0
#define SERVO_KI 20.0

#define ROBOT_WIDTH 0.075
#define ROBOT_WHEEL_RADIUS 0.0165

#define MAX_VEL 0.2
#define MAX_ANG_VEL 1.0

#define TURN_RADIUS_SHORTEST 0.09
#define TURN_RADIUS_EXPLORE 0.05

#define CELL_WIDTH 0.18

//Asmr parameters
#define ASMR_PROG_BUFFER 64

//Wall following
// right small distance - 56
// left small distance - 48
// right long distance - 20
// left long distance - 14

//DistSens
#define MF_LEFT_REFERENCE 110 //48 75
#define MF_RIGHT_REFERENCE 150 //56 115

#define MF_LEFT_TRESHOLD 36 //14 25
#define MF_RIGHT_TRESHOLD 30 //20 50

#define MF_FLEFT_TRESHOLD 19
#define MF_FRIGHT_TRESHOLD 15

//Maze
#define MAZE_WIDTH 6
#define MAZE_HEIGHT 4

//Solve
#define MAX_SOLVE_TIME 500

//Navigator

#define GOAL_X 2
#define GOAL_Y 2
#define NAV_X_START 1
#define NAV_Y_START 1
#define NAV_SIGMA_START 0 //Стартуем вправо
