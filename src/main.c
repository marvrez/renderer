#include "parameters.h"
#include "render_utils.h"

#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL_ttf.h>

void spin_renderer_once(program_state* state, TTF_Font* font)
{

}

int main(int argc, char** argv)
{
    program_state state;
    state.running = 1;

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

    SDL_Surface* surface  = SDL_GetWindowSurface(window);

    SDL_Renderer* renderer = SDL_GetRenderer(window); //SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if(!renderer) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "failed to create a renderer: %s\n", SDL_GetError()
        );
        state.running = 0;
    }
    TTF_Font* font = TTF_OpenFont("font.ttf", 16);
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

    while(state.running) {
        spin_renderer_once(&state, font);
    }
    return 0;
}
