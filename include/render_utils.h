#ifndef RENDER_UTILS_H
#define RENDER_UTILS_H

#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#include "math_utils.h"
#include "physics_utils.h"

typedef struct {
    player p;

    int running;
    int fps;

    // SDL states
    SDL_Surface* surface;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    TTF_Font* font;

} program_state;


program_state setup_renderer();

void draw_floor(SDL_Renderer* renderer);

void draw_views(SDL_Renderer* renderer);
void draw_debug_text(program_state state);

// Offset functions make it easy to have multiple viewports
void draw_line_with_offset(SDL_Renderer* renderer, line l, SDL_Point offset);
void draw_line_with_scale_and_offset(SDL_Renderer* renderer, line l, float scale, SDL_Point offset);

void draw_pixel_with_offset(SDL_Renderer* renderer, vec2 p, SDL_Point offset);
void draw_pixel_with_scale_and_offset(SDL_Renderer* renderer, vec2 p, float scale, SDL_Point offset);

#endif
