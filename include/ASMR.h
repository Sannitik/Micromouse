#pragma once

#include <stdint.h>
#include <Arduino.h>
#include "Config.h"
#include "Odometer.h"
#include "Mixer.h"
#include "DistSens.h"
#include "Navigator.h"
#include "WallFollowing.h"
#include "Tipes.h"
#include "WallExploler.h"
#include "MazeDrawer.h"
#include "Router.h"


#define FROM_STRAIGHT 0
#define FROM_DIAG 0b00001000

#define SHORTEST 0
#define EXPLORE 0b00010000
#define IN_PLACE 0b00100000

#define T45 0
#define T90 0b00000010
#define T135 0b00000100
#define T180 0b00000110

#define TURN_LEFT 0b00000000
#define TURN_RIGHT 0b00000001

ASMR_Entry asmr_prog_buffer[ASMR_PROG_BUFFER] = {
    SWD05,
    SWD05,
    // TURN_CYC + EXPLORE + FROM_STRAIGHT + T90 + TURN_RIGHT,
    // TURN_CYC + EXPLORE + FROM_STRAIGHT + T90 + TURN_RIGHT,
    // TURN_CYC + EXPLORE + FROM_STRAIGHT + T90 + TURN_LEFT,
    IDLE,
    STOP
};

enum ASMR_CYC_Type : uint8_t
{
    STIDLE = 0b00,
    FORW = 0b01,
    TURN = 0b10
};

size_t asmr_prog_counter = 0;

void asmr_init()
{
    asmr_prog_counter = 0;
}

void asmr_cyc_stidle(CyclogramOutput *output, SensorData data, ASMR_Entry cyc)
{
    output->v_0 = 0;
    output->theta_i0 = 0;

    if(cyc.raw == STOP)
    {
        output->is_completed = false;
    }
    else
    {
        output->is_completed = true;
    }
}

void asmr_cyc_forw(CyclogramOutput *output, SensorData data, ASMR_Entry cyc)
{
    output->v_0 = MAX_VEL;
    output->theta_i0 = wf_straight_tick(data);

    // int dist_half_int = cyc.forw.forw_dist;
    int16_t dist_half_int = cyc.raw & 0b00011111;

    float dist_mul = 1.0;

    if((cyc.raw & 0b00100000) != 0) //Diag
    {
        dist_mul = M_SQRT2;
        output->theta_i0 = 0;

    }
    else{
        output->theta_i0 = wf_straight_tick(data);
    }

    float dist = dist_half_int*0.5*CELL_WIDTH*dist_mul;

    output->is_completed = data.odom_S > dist;
}

const float turn_smooth_distances[][2] = {
    [0] = {CELL_WIDTH /2 - TURN_RADIUS_SHORTEST * tan(M_PI /8), CELL_WIDTH * M_SQRT1_2 - TURN_RADIUS_SHORTEST*tan(M_PI /8)}, // 45deg
    [1] = {CELL_WIDTH - TURN_RADIUS_SHORTEST,
           CELL_WIDTH * M_SQRT1_2 - TURN_RADIUS_SHORTEST}, // 90deg
    [2] = {3.0/2 * CELL_WIDTH - TURN_RADIUS_SHORTEST * tan(M_PI * 3.0/8), CELL_WIDTH * M_SQRT1_2 - TURN_RADIUS_SHORTEST * tan(3.0/8 * M_PI)}, // 135deg
    [3] = {CELL_WIDTH /2, 0}, //180
};

void asmr_cyc_turn(CyclogramOutput *output, SensorData data, ASMR_Entry cyc)
{
    uint8_t turn_type = (cyc.raw & 0b00110000) >> 4;
    uint8_t turn_source = (cyc.raw & 0b00001000) >> 3; // откуда приходим
    uint8_t turn_angle = (cyc.raw & 0b00000110) >> 1;
    uint8_t turn_dir = cyc.raw & 0b00000001; // направление поворота

    uint8_t turn_dest = (~turn_angle & 0b1) ^ turn_source; // куда приходим

    float turn_delta_theta = (45 + 45*turn_angle)* DEG_TO_RAD;
    float turn_radius = 0;

    float first_dist = 0;
    float second_dist = 0;
    float turn_dist = 0;

    float turn_vel_f = 0;
    float turn_vel_w = 0;

    if(turn_type == 0)
    {
        if(turn_angle == 3)
        {
            turn_radius = CELL_WIDTH /2 ;
        }
        else
        {
            turn_radius = TURN_RADIUS_SHORTEST;
        }
        first_dist = turn_smooth_distances[turn_angle][turn_source];
        turn_dist = turn_delta_theta * turn_radius;
        second_dist = turn_smooth_distances[turn_angle][turn_dest];

        turn_vel_f = MAX_VEL;
        float turn_vel = MAX_VEL / turn_radius;
        turn_vel_w = turn_dir ? -turn_vel : turn_vel;
    }
    else if(turn_type == 1) // EXPLORE
    {
        turn_radius = TURN_RADIUS_EXPLORE;

        if (turn_angle != 1)
        {
            Serial.println("EXPLORE turn_angle != 90");    
            return;
        }

        first_dist = CELL_WIDTH / 2 - turn_radius - 0.01;
        turn_dist = M_PI_2 * turn_radius;
        second_dist = first_dist;

        turn_vel_f = MAX_VEL;
        float turn_vel = MAX_VEL / turn_radius;
        turn_vel_w = turn_dir ? -turn_vel : turn_vel;
    }
    else if(turn_type == 2) //IN PLACE
    {
        turn_radius = 0;

        first_dist = CELL_WIDTH /2;
        turn_dist = 0;
        second_dist = CELL_WIDTH /2;

        turn_vel_f = 0;
        turn_vel_w = turn_dir ? -MAX_ANG_VEL : MAX_ANG_VEL;
    }
    if(data.odom_S < first_dist)
    {
        asmr_cyc_forw(output, data, ASMR_Entry{(FORW << 6) | (turn_source << 5)});
    }
    else if(fabs(data.odom_theta) < turn_delta_theta)
    {
        // output->v_0 = MAX_VEL;
        // float turn_vel = MAX_VEL / turn_radius;
        // output->theta_i0 = turn_dir ? -turn_vel : turn_vel;
        output->v_0 = turn_vel_f;
        output->theta_i0 = turn_vel_w;
    }
    else if (data.odom_S < first_dist + turn_dist + second_dist)
    {
        data.odom_theta -= turn_dir ? -turn_delta_theta : turn_delta_theta;
        asmr_cyc_forw(output, data, ASMR_Entry{FORW << 6 | turn_dest << 5});
    }
    output->is_completed  = data.odom_S > first_dist + turn_dist + second_dist;
}

void asmr_nav_update(ASMR_Entry cyc)
{
    uint8_t cyc_type = cyc.raw >> 6;

    switch (cyc_type)
    {
        case STIDLE:
        nav_tick(0, 0, 0);
        break;
        case FORW:
        {
            uint8_t dx = cyc.raw & 0b00011111;
            nav_tick(dx, 0, 0);
            break;
        }
        case TURN:
        {
            uint8_t delta_angle = (cyc.raw & 0b00000110) >> 1;
            uint8_t turn_dir = cyc.raw & 0b00000001;

            int8_t dsigma = (delta_angle + 1) * (turn_dir ? -1 : 1);

            int dx = 0;
            int dy = 0;

            switch((cyc.raw & 0b00110000) >> 4)
            {
            case 0b00: //Shortest
                switch(delta_angle)
                {
                    case 0: // 45deg
                        dx = 2;
                        dy = -1;
                        break;
                    case 1: // 90 deg
                        if(cyc.raw & 0b00001000)
                        {
                            dx = 1;
                            dy = -1;
                        }
                        else
                        {
                            dx = 2;
                            dy = -2;
                        }
                        break;
                    case 2: // 135 deg
                        dx = 1;
                        dy = -2;
                        break;
                    case 3: // 180 deg
                        dx = 0;
                        dy = -2;
                        break;
                }
                break;
            case 0b01: //Explore
                dx = 1;
                dy = -1;
                break;
            case 0b10: //In-place
                dx = 0;
                dy = 0;
                break;
            }

            if (turn_dir)
            {
                dy = -dy;
            }

            nav_tick(dx, dy, dsigma);
            break;
        }
        default:
            break;
    }
}

void asmr_tick()
{
    //Read sensers
    odom_ticks();
    //Run cyclogram

    SensorData data = {
        .odom_S = odom_get_S(),
        .odom_theta = odom_get_theta(),
        .time = micros(), //!!!
        .dist_left = dist_get_left(),
        .dist_right = dist_get_right(),
        .dist_fleft = dist_get_fleft(),
        .dist_fright = dist_get_fright(),
    };

    data.is_wall_left  = data.dist_left > MF_LEFT_TRESHOLD;
    data.is_wall_right = data.dist_right > MF_RIGHT_TRESHOLD;
    data.is_wall_fleft  = data.dist_fleft > MF_FLEFT_TRESHOLD;
    data.is_wall_fright  = data.dist_fright > MF_FRIGHT_TRESHOLD;

    CyclogramOutput output;

    ASMR_Entry current_cyc = asmr_prog_buffer[asmr_prog_counter];

    uint8_t cyc_type = current_cyc.raw >> 6;

    switch (cyc_type)
    {
    case STIDLE:
        asmr_cyc_stidle(&output, data, current_cyc);
        break;
    case FORW:
        asmr_cyc_forw(&output, data, current_cyc);
        break;
    case TURN:
        asmr_cyc_turn(&output, data, current_cyc);
        break;
    default:
        break;
    }

    if(output.is_completed)
    {
        Serial.println(current_cyc.raw, BIN);
        asmr_prog_counter++;
        odom_reset();
        asmr_nav_update(current_cyc);
        wall_explorer_update(data);
        //router_init();
        solver_init();
        solver_set_start_goal(nav_get_pos(), Vec2{GOAL_X, GOAL_Y});
    }
    bool is_solved = solver_solve();

    if(is_solved)
    {
        router_init();
        router_tick();
        router_path_to_cyc(router_path_buffer);
        Serial.println(router_path_buffer);
        Serial.println(nav_get_x());
        Serial.println(nav_get_y());
        Serial.println(nav_get_sigma());
        draw_maze_with_solver(MAZE_WIDTH, MAZE_HEIGHT);


        asmr_prog_buffer[0] = router_cyc_buffer[0];
        asmr_prog_counter = 0;

        solver_init();
    }
    if (current_cyc.raw == IDLE)
    {
        draw_maze(MAZE_WIDTH, MAZE_HEIGHT);
    }
    //Write motors
    mixer_tick(output.v_0, output.theta_i0);
}

size_t asmr_get_prog_counter()
{
    return asmr_prog_counter;
}

ASMR_Entry *asmr_get_prog_buffer()
{
    return asmr_prog_buffer;
}