#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <SDL2/SDL.h>
#include "physics_utils.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define HALF_WINDOW_WIDTH WINDOW_WIDTH / 2
#define HALF_WINDOW_HEIGHT WINDOW_HEIGHT / 2

#define VIEW_WIDTH 256
#define VIEW_HEIGHT 256
#define HALF_VIEW_WIDTH VIEW_WIDTH / 2
#define HALF_VIEW_HEIGHT VIEW_HEIGHT / 2

#define CROUCHING_HEIGHT 6*HALF_WINDOW_HEIGHT
#define STANDING_HEIGHT 10*HALF_WINDOW_HEIGHT

static const SDL_Color CEILING_COLOR = { 96, 96, 128, 255 };
static const SDL_Color FLOOR_COLOR = { 64, 64, 64, 255 };

static const SDL_Rect ABSOLUTE_VIEW = { 16, 16, VIEW_WIDTH, VIEW_HEIGHT };
static const SDL_Rect TRANSFORMED_VIEW = { WINDOW_WIDTH - VIEW_WIDTH - 16, 16, VIEW_WIDTH, VIEW_HEIGHT };
static const SDL_Rect PERSPECTIVE_VIEW = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

static const wall_line wall_lines[] = {
    { { { 0.0, 0.0 }, { 50.0, 0.0 } } , { 255, 0, 255, 255 } },
    { { { 50.0, 0.0 }, { 100.0, 25.0 } },{ 255, 0, 255, 255 } },
    { { { 100.0, 25.0 } , { 50.0, 50.0 } }, { 0, 255, 255, 255 } },
    { { { 50.0, 50.0}, { 0.0, 50.0 } }, { 255, 0, 255, 255 } },
    { { { 0.0, 50.0 } , { 0.0, 0.0 } }, { 255, 0, 255, 255 } }
};

#endif
