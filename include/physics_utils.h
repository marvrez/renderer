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
    vec2 start, end;
    SDL_Color color;
} wall_line;

// Physics functions
int line_side(vec2 point, vec2 start, vec2 end);
int player_in_bounds(vec2 wall_start, vec2 end);
int is_player_colliding_with_wall();
