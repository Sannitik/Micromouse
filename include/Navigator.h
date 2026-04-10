#pragma once

#include "Config.h"
#include "Maze.h"

struct NavPos
{
    int x;
    int y;
    int sigma;
};
NavPos nav_pos;

int nav_si[] = {0, 1, 1, 1, 0, -1, -1, -1};
int nav_co[] = {1, 1, 0, -1, -1, -1, 0, 1};

void nav_init()
{
    nav_pos.x = NAV_X_START;
    nav_pos.y = NAV_Y_START;
    nav_pos.sigma = NAV_SIGMA_START;
}

int nav_get_x()
{
    return nav_pos.x;
}
int nav_get_y()
{
    return nav_pos.y;
}
int nav_get_sigma()
{
    return nav_pos.sigma;
}

void nav_tick(int dx, int dy, int dsigma)
{
    int x_g = dx * nav_co[nav_get_sigma()] + dy * nav_si[nav_get_sigma()];
    int y_g = -dx * nav_si[nav_get_sigma()] + dy * nav_co[nav_get_sigma()];

    nav_pos.x += x_g;
    nav_pos.y += y_g;
    nav_pos.sigma += dsigma;
    nav_pos.sigma &= 0b111;
}
Vec2 nav_get_pos()
{
    Vec2 pos;
    if(nav_pos.x % 2 == 1 && nav_pos.y % 2 == 2)
    {
        pos.x = (nav_pos.x - 1)/2;
        pos.y = (nav_pos.y - 1)/2;
    }
    else
    {
        nav_tick(1, 0, 0);

        pos.x = (nav_pos.x - 1)/2;
        pos.y = (nav_pos.y - 1)/2;

        nav_tick(-1, 0, 0);
    }

    return pos;
}