#include "parameters.h"
#include "render_utils.h"

#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL_ttf.h>


void render_wall(program_state* state, wall_line wl)
{

}

void render_walls(program_state* state)
{
    for(int i = 0; i < NUM_WALLS; ++i) {
        render_wall(state, WALL_LINES[i]);
    }
}

void spin_renderer_once(program_state* state)
{

    SDL_SetRenderDrawColor(state->renderer, CEILING_COLOR.r, CEILING_COLOR.g, CEILING_COLOR.b, 255);
    SDL_RenderClear(state->renderer);

    draw_floor(state->renderer);
    render_walls(state);

    SDL_RenderPresent(state->renderer);
}

int main(int argc, char** argv)
{
    program_state state = setup_renderer();

    while(state.running) {
        spin_renderer_once(&state);
    }

    return 0;
}
