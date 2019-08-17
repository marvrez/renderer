#ifndef PHYSICS_UTILS_H
#define PHYSICS_UTILS_H

#include "math_utils.h"

#include <SDL2/SDL.h>

typedef struct {
    vec2 pos;
    float height;
    float angle;
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
int is_player_colliding_with_wall(player p, int num_wall_lines, wall_line* wall_lines);

#endif
