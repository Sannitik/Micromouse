#pragma once

#include "Config.h"

int nav_x;
int nav_y;
int nav_sigma;

int nav_si[] = {0, 1, 1, 1, 0, -1, -1, -1};
int nav_co[] = {1, 1, 0, -1, -1, -1, 0, 1};

void nav_init()
{
    nav_x = NAV_X_START;
    nav_y = NAV_Y_START;
    nav_sigma = NAV_SIGMA_START;
}

int nav_get_x()
{
    return nav_x;
}
int nav_get_y()
{
    return nav_y;
}
int nav_get_sigma()
{
    return nav_sigma & 0b111;
}

void nav_tick(int dx, int dy, int dsigma)
{
    int x_g = dx * nav_co[nav_get_sigma()] + dy * nav_si[nav_get_sigma()];
    int y_g = -dx * nav_si[nav_get_sigma()] + dy * nav_co[nav_get_sigma()];

    nav_x += x_g;
    nav_y += y_g;
    nav_sigma += dsigma;
}