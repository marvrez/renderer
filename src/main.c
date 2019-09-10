#include "parameters.h"
#include "render_utils.h"
#include "physics_utils.h"

#include <math.h>
#include <SDL2/SDL.h>

static inline int done()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    return event.type == SDL_QUIT
        || event.key.keysym.sym == SDLK_END
        || event.key.keysym.sym == SDLK_ESCAPE;
}

static inline void handle_input(program_state* state)
{
    player* p = &state->p;
    vec2 prev_pos = p->pos;

    const uint8_t* key = SDL_GetKeyboardState(NULL);

    if(key[SDL_SCANCODE_LEFT])  p->angle += 0.1f;
    if(key[SDL_SCANCODE_RIGHT]) p->angle -= 0.1f;
    float pcos = cosf(p->angle), psin = sinf(p->angle);
    if(key[SDL_SCANCODE_UP] || key[SDL_SCANCODE_W]) {
        p->pos.x += pcos;
        p->pos.y -= psin;
    }
    if(key[SDL_SCANCODE_DOWN] || key[SDL_SCANCODE_S]) {
        p->pos.x -= pcos;
        p->pos.y += psin;
    }
    if(key[SDL_SCANCODE_A]) {
        p->pos.x -= psin;
        p->pos.y -= pcos;
    }
    if(key[SDL_SCANCODE_D]) {
        p->pos.x += psin;
        p->pos.y += pcos;
    }
    if(is_player_colliding_with_wall(*p)) p->pos = prev_pos;
    if(key[SDL_SCANCODE_Q]) state->running = 0;

    p->height = key[SDL_SCANCODE_LCTRL] ? CROUCHING_HEIGHT : STANDING_HEIGHT;
}

int main(int argc, char** argv)
{
    program_state state = setup_renderer();
    while(state.running && !done()) {
        handle_input(&state);

        int t0 = SDL_GetTicks();

        draw_floor_and_ceiling(state.renderer);

        // draw all walls on the whole screen
        for(int i = 0; i < NUM_WALLS; ++i) {
            draw_wall(&state, WALL_LINES[i]);
        }

        // draw debug viewports
        draw_views(state.renderer);
        draw_debug_text(state);
        for(int i = 0; i < NUM_WALLS; ++i) {
            draw_debug_wall(&state, WALL_LINES[i]);
        }

        // render all of it to the screen
        SDL_RenderPresent(state.renderer);

        // manually caps frame rate to ~60 fps if the vertical sync (VSYNC) init failed.
        int t1 = SDL_GetTicks();
        int ms = 16 - (t1 - t0);
        SDL_Delay(ms < 0 ? 0 : ms);
        state.fps = 1000.f / ((t1-t0) + ms);
    }
    return 0;
}
