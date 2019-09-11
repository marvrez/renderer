#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <SDL2/SDL.h>
#include "physics_utils.h"

#define EPS 1e-4

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define HALF_WINDOW_WIDTH WINDOW_WIDTH / 2
#define HALF_WINDOW_HEIGHT WINDOW_HEIGHT / 2

#define VIEW_WIDTH 256
#define VIEW_HEIGHT 256
#define HALF_VIEW_WIDTH VIEW_WIDTH / 2
#define HALF_VIEW_HEIGHT VIEW_HEIGHT / 2

#define ANGULAR_VELOCITY 0.05f
#define CROUCHING_HEIGHT 6.f*HALF_WINDOW_HEIGHT
#define STANDING_HEIGHT 10.f*HALF_WINDOW_HEIGHT

#define FONT_PATH "./font.ttf"

static const SDL_Color CEILING_COLOR = { 96, 96, 128, 255 };
static const SDL_Color FLOOR_COLOR = { 64, 64, 64, 255 };

static const SDL_Rect ABSOLUTE_VIEW = { 16, 16, VIEW_WIDTH, VIEW_HEIGHT };
static const SDL_Rect TRANSFORMED_VIEW = { WINDOW_WIDTH - VIEW_WIDTH - 16, 16, VIEW_WIDTH, VIEW_HEIGHT };
static const SDL_Rect PERSPECTIVE_VIEW = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

static const wall_line WALL_LINES[] = {
    { { { 0.0, 0.0 }, { 50.0, 0.0 } } , { 255, 192, 203, 255 } },
    { { { 50.0, 0.0 }, { 100.0, 25.0 } },{ 199, 199, 2, 255 } },
    { { { 100.0, 25.0 } , { 50.0, 50.0 } }, { 191, 255, 244, 255 } },
    { { { 50.0, 50.0}, { 0.0, 50.0 } }, { 160, 71, 235, 255 } },
    { { { 0.0, 50.0 } , { 0.0, 0.0 } }, { 110, 127, 6, 255 } }
};
static const int NUM_WALLS = sizeof(WALL_LINES)/sizeof(WALL_LINES[0]);

#endif
