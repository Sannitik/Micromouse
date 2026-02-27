#pragma once 

#include "config.h"
#include "Maze.h"
#include "MazeDrawer.h"
#include "Solver.h"
#include "Navigator.h"

test_maze()
{
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

    uint32_t time0 = micros();
    solver_set_start_goal(Vec2{0, 0}, Vec2{4, 3});
    uint32_t time1 = micros();

    Serial.print("Solver_init: ");
    Serial.print(time1 - time0);
    Serial.println(" us");

    bool solved = false;
    do
    {
        uint32_t time0s = micros();
        solved = solver_solve();
        uint32_t time1s = micros();
        Serial.print("Solver solve: ");
        Serial.print(time1s - time0s);
        Serial.println(" us");
    } while(!solved);

    draw_maze_with_solver(MAZE_WIDTH, MAZE_HEIGHT);

    while(1);
}

void test_nav_print_coords()
{
    Serial.print("X: ");
    Serial.print(nav_get_x());
    Serial.print(" Y: ");
    Serial.print(nav_get_y());
    Serial.print(" Sigma: ");
    Serial.println(nav_get_sigma());
}

void test_navigator()
{
    nav_init();

    test_nav_print_coords();
    nav_tick(0, 0, 0);
    test_nav_print_coords();

    nav_tick(2, -2, 2);
    test_nav_print_coords();
    nav_tick(2, -2, 2);
    test_nav_print_coords();
    nav_tick(2, -2, 2);
    test_nav_print_coords();
    nav_tick(2, -2, 2);
    test_nav_print_coords();
}