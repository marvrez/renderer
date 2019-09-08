#include "parameters.h"
#include "render_utils.h"

#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL_ttf.h>


void render_wall(program_state* state, wall_line wl)
{

}

void render_debug_wall(program_state* state, wall_line wl)
{

}

void handle_input(program_state* state)
{

}

int main(int argc, char** argv)
{
    program_state state = setup_renderer();
    while(state.running) {
        handle_input(&state);

        // set ceiling color
        SDL_SetRenderDrawColor(state.renderer, CEILING_COLOR.r, CEILING_COLOR.g, CEILING_COLOR.b, CEILING_COLOR.a);
        SDL_RenderClear(state.renderer);

        draw_floor(state.renderer);

        // render all walls on the whole screen
        for(int i = 0; i < NUM_WALLS; ++i) {
            render_wall(&state, WALL_LINES[i]);
        }

        // render debug viewports
        draw_views(state.renderer);
        draw_debug_text(state);
        for(int i = 0; i < NUM_WALLS; ++i) {
            render_debug_wall(&state, WALL_LINES[i]);
        }

        SDL_RenderPresent(state.renderer);
    }
    return 0;
}
