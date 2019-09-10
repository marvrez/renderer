#include "render_utils.h"

#include "parameters.h"
#include "math_utils.h"

#include <string.h>
#include <math.h>

program_state setup_renderer()
{
    program_state state = {{{25.f, 25.f}, STANDING_HEIGHT, 0.f}, 1, 0.f};

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

void draw_floor_and_ceiling(SDL_Renderer* renderer)
{
    // set ceiling color
    SDL_SetRenderDrawColor(renderer, CEILING_COLOR.r, CEILING_COLOR.g, CEILING_COLOR.b, CEILING_COLOR.a);
    SDL_RenderClear(renderer);

    // set floor color
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
        "fps: %.2f \n"
        "player position: (%.2f, %.2f)\n"
        "angle: %.2f deg. \n\n"
        "Move with arrow keys / WASD, left ctrl to crouch\nPress esc or q to exit.",
        state.fps,
        state.p.pos.x, state.p.pos.y,
        RAD2DEG(fmodf(state.p.angle, 2*M_PI))
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

void draw_debug_wall(program_state* state, wall_line wl)
{
    SDL_Point offset;
    player p = state->p;

    // used for drawing the current line
    line absolute_line = wl.l;

    float pcos = cosf(p.angle), psin = sinf(p.angle);

    line player_line = { p.pos, { p.pos.x + 5*pcos, p.pos.y - 5*psin} };

    // transform line to be relative to the player
    vec3 transformed_line_start = { absolute_line.start.x - p.pos.x, p.pos.y - absolute_line.start.y };
    vec3 transformed_line_end = { absolute_line.end.x - p.pos.x, p.pos.y - absolute_line.end.y };

    // rotate them to be around the player's view (90-player.angle) degrees
    transformed_line_start.z = transformed_line_start.x*pcos + transformed_line_start.y*psin;
    transformed_line_end.z = transformed_line_end.x*pcos + transformed_line_end.y*psin;
    // multiplied by -1 because we want to view the inverse x-change in the transformed view
    transformed_line_start.x = -(transformed_line_start.x*psin - transformed_line_start.y*pcos);
    transformed_line_end.x = -(transformed_line_end.x*psin - transformed_line_end.y*pcos);

    /* ABSOLUTE VIEW */
    offset.x = ABSOLUTE_VIEW.x, offset.y = ABSOLUTE_VIEW.y;
    // draw wall lines in the absolute view
    SDL_SetRenderDrawColor(state->renderer, wl.color.r, wl.color.g, wl.color.b, wl.color.a);
    draw_line_with_offset(state->renderer, absolute_line, offset);

    // draw absolute red player line
    SDL_SetRenderDrawColor(state->renderer, 255, 0, 0, 255);
    draw_line_with_offset(state->renderer, player_line, offset);

    /* TRANSFORMED VIEW */
    offset.x = TRANSFORMED_VIEW.x, offset.y = TRANSFORMED_VIEW.y;
    // draw the wall lines with transformed view
    SDL_SetRenderDrawColor(state->renderer, wl.color.r, wl.color.g, wl.color.b, wl.color.a);
    line transformed_line = {
        { HALF_VIEW_WIDTH - transformed_line_start.x, HALF_VIEW_WIDTH - transformed_line_start.z },
        { HALF_VIEW_WIDTH - transformed_line_end.x, HALF_VIEW_WIDTH - transformed_line_end.z }
    };
    draw_line_with_offset(state->renderer, transformed_line, offset);
}

void draw_views(SDL_Renderer* renderer)
{
    // set color to black & fill viewport background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &ABSOLUTE_VIEW);
    // set color to red and draw border.
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &ABSOLUTE_VIEW);

    // set color to black & fill viewport background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &TRANSFORMED_VIEW);
    // set color to red and draw border.
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &TRANSFORMED_VIEW);

    // draw static red player line in transformed view
    SDL_Point offset = { TRANSFORMED_VIEW.x, TRANSFORMED_VIEW.y };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    line l = { { HALF_VIEW_WIDTH, HALF_VIEW_HEIGHT }, { HALF_VIEW_WIDTH, HALF_VIEW_WIDTH - 7 } };
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
