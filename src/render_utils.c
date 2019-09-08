#include "render_utils.h"

#include "parameters.h"

#include <string.h>

program_state setup_renderer()
{
    program_state state;
    state.running = 1;
    state.fps = 0;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow(
        "meme renderer",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );

    if(!window) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "failed to create a window: %s\n", SDL_GetError()
        );
        state.running = 0;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if(!renderer) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "failed to create a renderer: %s\n", SDL_GetError()
        );
        state.running = 0;
    }

    SDL_Surface* surface  = SDL_GetWindowSurface(window);

    TTF_Font* font = TTF_OpenFont(FONT_PATH, 16);
    if(!font) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "failed to load font: %s\n", SDL_GetError()
        );
        state.running = 0;
    }

    state.surface = surface;
    state.renderer = renderer;
    state.window = window;
    state.font = font;

    return state;
}

void draw_floor(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, FLOOR_COLOR.r, FLOOR_COLOR.g, FLOOR_COLOR.b, FLOOR_COLOR.a);
    SDL_Rect floor_rect = { 0, HALF_WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_RenderFillRect(renderer, &floor_rect);
}

void draw_debug_text(program_state state)
{
    static char message[1024];
    // Draw info text
    sprintf(
        message,
        "fps: %d \n"
        "player position: (%.2f, %.2f) \n"
        "angle is %.2f degrees. \n\n"
        "Move with arrow keys / WASD, r to reset position, e to turn 1 degree, t to turn 45 degrees, left ctrl to crouch\nPress q to quit.",
        state.fps,
        state.p.pos.x, state.p.pos.y,
        fmod(state.p.angle, 2*M_PI) * 180 / M_PI
    );

    // Create surfaces, texture & rect needed for text rendering
    SDL_Color red = { 255, 0, 0, 255 };
    SDL_Surface* info_text_surface = TTF_RenderText_Blended_Wrapped(state.font, message, red, WINDOW_WIDTH - 15);
    SDL_Texture* info_texture = SDL_CreateTextureFromSurface(state.renderer, info_text_surface);

    SDL_Rect text_render_quad = { 15, VIEW_HEIGHT + 30, info_text_surface->w, info_text_surface->h };
    SDL_RenderCopy(state.renderer, info_texture, NULL, &text_render_quad);

    // destroy data used to draw text
    SDL_FreeSurface(info_text_surface);
    SDL_DestroyTexture(info_texture);
}

void draw_views(SDL_Renderer* renderer)
{
    // draw absolute viewport
    // set color to black & draw viewport background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &ABSOLUTE_VIEW);
    // set color to red and draw border.
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &ABSOLUTE_VIEW);

    // draw transformed viewport
    // set color to black & draw viewport background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &TRANSFORMED_VIEW);
    // set color to red and draw border.
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &TRANSFORMED_VIEW);

    SDL_Point offset = { TRANSFORMED_VIEW.x, TRANSFORMED_VIEW.y };

    // draw player line
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    line l = { { HALF_VIEW_WIDTH, HALF_VIEW_HEIGHT }, { HALF_VIEW_WIDTH, HALF_VIEW_WIDTH - 5 } };
    draw_line_with_offset(renderer, l, offset);
}

void draw_line_with_offset(SDL_Renderer* renderer, line l, SDL_Point offset)
{
    SDL_RenderDrawLine(renderer, 
       l.start.x + offset.x, 
       l.start.y + offset.y, 
       l.end.x + offset.x, 
       l.end.y + offset.y
    );
}

void draw_line_with_scale_and_offset(SDL_Renderer* renderer, line l, float scale, SDL_Point offset)
{
    SDL_RenderDrawLine(renderer, 
        (int)(scale * l.start.x) + offset.x,
        (int)(scale * l.start.y) + offset.y, 
        (int)(scale * l.end.x) + offset.x, 
        (int)(scale * l.end.y) + offset.y
    );
}

void draw_pixel_with_offset(SDL_Renderer* renderer, vec2 p, SDL_Point offset)
{
    SDL_RenderDrawPoint(renderer, p.x + offset.x, p.y + offset.y);
}

void draw_pixel_with_scale_and_offset(SDL_Renderer* renderer, vec2 p, float scale, SDL_Point offset)
{
    SDL_RenderDrawPoint(renderer, 
        (int)(scale*p.x) + offset.x, 
        (int)(scale*p.y) + offset.y
    );
}
