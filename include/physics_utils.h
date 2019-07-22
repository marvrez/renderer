#ifndef PHYSICS_UTILS_H
#define PHYSICS_UTILS_H

#include "math_utils.h"

#include <SDL/SDL2.h>

typedef struct {
    vec2 pos;
} player;

typedef struct {
    vec2 top_left, top_right;
    vec2 bottom_right, bottom_left;
} wall;

typedef struct {
    line l;
    SDL_Color color;
} wall_line;

// Physics functions
int point_in_line(vec2 p, line l);
int player_in_bounds(player p, wall_line l);
int is_player_colliding_with_wall(player p, int num_walls, wall* walls);

#endif
