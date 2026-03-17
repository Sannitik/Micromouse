#pragma once
#include "Tipes.h"
#include "Config.h"
#include "Navigator.h"
#include "Maze.h"

void wall_explorer_update(SensorData data)
{
    int sigma = nav_get_sigma();
    Vec2 pos = nav_get_pos();

    Maze::CellWalls walls = {Maze::UNKNOWN, Maze::UNKNOWN, Maze::UNKNOWN, Maze::UNKNOWN};

    bool forw_wall = data.is_wall_fleft || data.is_wall_fright;
    bool left_wall = data.is_wall_left;
    bool right_wall = data.is_wall_right;

    // Serial.print("L: ");
    // Serial.print(left_wall);
    // Serial.print(" F: ");
    // Serial.print(forw_wall);
    // Serial.print(" R: ");
    // Serial.println(right_wall);

    if(sigma == 0)
    {
        walls.east = forw_wall ? Maze::WALL : Maze::OPEN;
        walls.north = left_wall ? Maze::WALL : Maze::OPEN;
        walls.south = right_wall ? Maze::WALL : Maze::OPEN;
    }
    else if(sigma == 2)
    {
        walls.north = forw_wall ? Maze::WALL : Maze::OPEN;
        walls.west = left_wall ? Maze::WALL : Maze::OPEN;
        walls.east = right_wall ? Maze::WALL : Maze::OPEN;
    }
    else if(sigma == 4)
    {
        walls.west = forw_wall ? Maze::WALL : Maze::OPEN;
        walls.south = left_wall ? Maze::WALL : Maze::OPEN;
        walls.north = right_wall ? Maze::WALL : Maze::OPEN;
    }
    else if(sigma == 6)
    {
        walls.south = forw_wall ? Maze::WALL : Maze::OPEN;
        walls.east = left_wall ? Maze::WALL : Maze::OPEN;
        walls.west = right_wall ? Maze::WALL : Maze::OPEN;
    }

    maze_set_wall(pos, walls);
}