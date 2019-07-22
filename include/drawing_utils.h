#ifndef DRAWING_UTILS_H
#define DRAWING_UTILS_H

#include <SDL/SDL2.h>

#include "math_utils.h"

void render_wall(wall_line l);

// Offset functions make it easy to have multiple viewports
void draw_line_with_offset(SDL_Renderer* renderer, line l, SDL_Point offset);
void draw_line_with_scale_and_offset(SDL_Renderer* renderer, line l, float scale, SDL_Point offset);

void draw_pixel_with_offset(SDL_Renderer* renderer, vec2 p, SDL_Point offset);
void draw_pixel_with_scale_and_offset(SDL_Renderer* renderer, vec2 p, float scale, SDL_Point offset);

#endif
