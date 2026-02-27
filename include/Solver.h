#pragma once

#include "Maze.h"
#include "Config.h"
const int MAX_SIZE = 64; // Maximum size of the queue

template<typename T>
class Queue {
private:
    T arr[MAX_SIZE];
    int begin;
    int end;

public:
    Queue() {
        begin = 0;
        end = 0;
    }

    bool isEmpty()
    {
        return begin == end;
    }

    bool isFull()
    {
        return isEmpty();
    }

    bool push_back(T x)
    {
        arr[end] = x;
        end = (end + 1) % MAX_SIZE;
        return isFull();
    }

    T pop_front()
    {
        T x = arr[begin];
        begin = (begin + 1) % MAX_SIZE;
        return x;
    }

    void clear()
    {
        begin = 0;
        end = 0;
    }
};

Queue<Vec2> solver_queue;

enum class WhereFrom
{
    UNKNOWN,
    WEST,
    SOUTH,
    NOATH,
    EAST,
    GOAL
};

WhereFrom solver_where_from_storage[MAZE_WIDTH][MAZE_HEIGHT] = {WhereFrom::UNKNOWN};

    Vec2 solver_start;
    Vec2 solver_goal;

void solver_init()
{
    solver_queue.clear();
    for(int x = 0; x < MAZE_WIDTH; x++){
        for(int y = 0; y < MAZE_WIDTH; y++){
            solver_where_from_storage[x][y] = WhereFrom::UNKNOWN;
        }
    }
}

void solver_set_start_goal(Vec2 start, Vec2 goal)
{
    solver_start = start;
    solver_goal = goal;

    solver_queue.push_back(goal);
    solver_where_from_storage[goal.x][goal.y] = WhereFrom::GOAL;
}

bool solver_solve()
{
    uint32_t time0 = micros();
    while(!solver_queue.isEmpty() && micros() - time0 < MAX_SOLVE_TIME)
    {
        Vec2 current = solver_queue.pop_front();

        if(current.x == solver_start.x && current.y == solver_start.y)
        {
            return true;
        }

        Maze::CellWalls current_walls = maze_get_walls(current);

        if (current_walls.left != Maze::WALL)
        {
            Vec2 new_sosed = Vec2{current.x - 1, current.y};
            if(solver_where_from_storage[new_sosed.x][new_sosed.y] == WhereFrom::UNKNOWN)
            {
                solver_where_from_storage[new_sosed.x][new_sosed.y] = WhereFrom::EAST;
                solver_queue.push_back(new_sosed);
            }
        }
        if (current_walls.down != Maze::WALL)
        {
            Vec2 new_sosed = Vec2{current.x, current.y + 1};
            if(solver_where_from_storage[new_sosed.x][new_sosed.y] == WhereFrom::UNKNOWN)
            {
                solver_where_from_storage[new_sosed.x][new_sosed.y] = WhereFrom::NOATH;
                solver_queue.push_back(new_sosed);
            }
        }
        if (current_walls.up != Maze::WALL)
        {
            Vec2 new_sosed = Vec2{current.x, current.y - 1};
            if(solver_where_from_storage[new_sosed.x][new_sosed.y] == WhereFrom::UNKNOWN)
            {
                solver_where_from_storage[new_sosed.x][new_sosed.y] = WhereFrom::SOUTH;
                solver_queue.push_back(new_sosed);
            }
        }
        if (current_walls.right != Maze::WALL)
        {
            Vec2 new_sosed = Vec2{current.x + 1, current.y};
            if(solver_where_from_storage[new_sosed.x][new_sosed.y] == WhereFrom::UNKNOWN)
            {
                solver_where_from_storage[new_sosed.x][new_sosed.y] = WhereFrom::WEST;
                solver_queue.push_back(new_sosed);
            }
        }
    }

    return false;
}

char solver_get_where_from(Vec2 coord)
{
    char symbs[] = {'x', 'L', 'D', 'U', 'R', 'G'};
    return symbs[(int)solver_where_from_storage[coord.x][coord.y]];
}