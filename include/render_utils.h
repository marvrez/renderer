#ifndef RENDER_UTILS_H
#define RENDER_UTILS_H

#include <SDL2/SDL.h>

#include "math_utils.h"
#include "physics_utils.h"

void render_wall(wall_line l);

// Offset functions make it easy to have multiple viewports
void draw_line_with_offset(SDL_Renderer* renderer, line l, SDL_Point offset);
void draw_line_with_scale_and_offset(SDL_Renderer* renderer, line l, float scale, SDL_Point offset);

void draw_pixel_with_offset(SDL_Renderer* renderer, vec2 p, SDL_Point offset);
void draw_pixel_with_scale_and_offset(SDL_Renderer* renderer, vec2 p, float scale, SDL_Point offset);

#endif
