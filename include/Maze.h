#pragma once

#include <inttypes.h>
#include "Config.h"

/**
 * How much information is needed to represent all of the maze?
 *
 * One byte:
 *   cell A | cell B
 *      ssee|ssee
 *       | |  | ` east wall of cell B
 *       | |  `south wall of cell B
 *       | `east wall of cell A
 *       `south wall of cell A
 * Each wall is represented by a WallState enum
 *
 * So, 4 bits per cell, MAZE_WIDTH * MAZE_HEIGHT cells,
 * MAZE_MEM_SIZE = MAZE_WIDTH * MAZE_HEIGHT / 2 bytes
 */

#define MAZE_MEM_SIZE (MAZE_WIDTH * MAZE_HEIGHT / 2)

#define SET_WALL(stored_wall, wall) stored_wall = (wall == Maze::UNKNOWN ? stored_wall : wall)

struct Vec2
{
    int16_t x;
    int16_t y;
};

class Maze
{
public:
    enum WallState : uint8_t
    {
        UNKNOWN = 0,
        WALL,
        OPEN
        // TODO: add more wall types
    };
    struct CellWalls
    {
        WallState west : 2;
        WallState south : 2;
        WallState north : 2;
        WallState east : 2;
    };
private:

    struct CellStoreRaw
    {
        WallState hidown : 2;
        WallState hiright : 2;
        WallState lodown : 2;
        WallState loright : 2;
    };

    enum LoHi
    {
        LO = 1,
        HI = 0
    };

    CellStoreRaw map[MAZE_MEM_SIZE];

public:

    Maze()
    {
        for (int i = 0; i < MAZE_MEM_SIZE; i++)
        {
            map[i].lodown = UNKNOWN;
            map[i].loright = UNKNOWN;
            map[i].hidown = UNKNOWN;
            map[i].hiright = UNKNOWN;
        }
        for(int x = 0; x < MAZE_WIDTH; x++)
        {
            CellWalls cell = {.west = UNKNOWN, .south = WALL, .north = UNKNOWN, .east = UNKNOWN};
            Vec2 coord = {x, MAZE_HEIGHT - 1};
            setWall(coord, cell);
        }
        for(int y = 0; y < MAZE_HEIGHT; y++)
        {
            CellWalls cell = {.west = UNKNOWN, .south = UNKNOWN, .north = UNKNOWN, .east = WALL};
            Vec2 coord = {MAZE_WIDTH - 1, y};
            setWall(coord, cell);
        }
    }

    /**
     * @brief Установить стенку в клетке с данными координатами
     */
    void setWall(Vec2 coord, CellWalls cell_walls)
    {
        int cell_id = coord.y * MAZE_WIDTH / 2 + coord.x / 2;
        int cell_offset = coord.x % 2;

        if(cell_offset == LO)
        {
            SET_WALL(map[cell_id].lodown, cell_walls.south);
            SET_WALL(map[cell_id].loright, cell_walls.east);
            SET_WALL(map[cell_id].hiright, cell_walls.west);

            if(cell_id >= MAZE_WIDTH / 2)
            {
                SET_WALL(map[cell_id - MAZE_WIDTH / 2].lodown, cell_walls.north);
            }
        }
        else
        {
            SET_WALL(map[cell_id].hidown, cell_walls.south);
            SET_WALL(map[cell_id].hiright, cell_walls.east);

            if(cell_id % (MAZE_WIDTH / 2) != 0)
            {
                SET_WALL(map[cell_id - 1].loright, cell_walls.west);
            }

            if(cell_id >= MAZE_WIDTH / 2)
            {
                SET_WALL(map[cell_id - MAZE_WIDTH / 2].hidown, cell_walls.north);
            }
        }
    }

    WallState getUpWall(int cell_id, int cell_offset) const
    {
        if(cell_id >= MAZE_WIDTH / 2)
        {
            if(cell_offset == LO)
            {
                return map[cell_id - MAZE_WIDTH / 2].lodown;
            }
            return map[cell_id - MAZE_WIDTH / 2].hidown;
        }
        return WALL;
    }
    WallState getLeftWall(int cell_id, int cell_offset) const
    {
        if(cell_offset == LO)
        {
            return map[cell_id].hiright;
        }

        if(cell_id % (MAZE_WIDTH / 2) == 0)
        {
            return WALL;
        }

        return map[cell_id - 1].loright;
    }

    WallState getDownWall(int cell_id, int cell_offset) const
    {
        if(cell_offset == LO)
        {
            return map[cell_id].lodown;
        }
        return map[cell_id].hidown;
    }

    WallState getRightWall(int cell_id, int cell_offset) const
    {
        if(cell_offset == LO)
        {
            return map[cell_id].loright;
        }
        return map[cell_id].hiright;
    }

    /**
    * @brief Получить информацию о стенках в клетке с данными координатами
    */
    CellWalls getWalls(Vec2 coord) const
    {
        int cell_id = coord.y * MAZE_WIDTH / 2 + coord.x / 2;
        int cell_offset = coord.x % 2;

        CellWalls cell =
        {
            .west  = getLeftWall(cell_id, cell_offset),
            .south  = getDownWall(cell_id, cell_offset),
            .north    = getUpWall(cell_id, cell_offset),
            .east = getRightWall(cell_id, cell_offset)
        };

        return cell;
    }
};

Maze maze;

void maze_set_wall(Vec2 coord, Maze::CellWalls cell_walls)
{
    maze.setWall(coord, cell_walls);
}

void maze_init()
{
    maze = Maze();
    maze_set_wall(Vec2{0, 0}, Maze::CellWalls{Maze::WALL, Maze::WALL, Maze::WALL, Maze::OPEN});

}

Maze::CellWalls maze_get_walls(Vec2 coord)
{
    return maze.getWalls(coord);
}