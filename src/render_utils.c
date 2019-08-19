#include "render_utils.h"

#include "parameters.h"

#include <string.h>

void draw_debug_text(SDL_Renderer* renderer, program_state state)
{
    static char message[1024];
}

void draw_views(SDL_Renderer* renderer, SDL_Point* offset)
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

    offset->x = TRANSFORMED_VIEW.x;
    offset->y = TRANSFORMED_VIEW.y;

    // draw player line
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    line l = { { HALF_VIEW_WIDTH, HALF_VIEW_HEIGHT }, { HALF_VIEW_WIDTH, HALF_VIEW_WIDTH - 5 } };
    draw_line_with_offset(renderer, l, *offset);

    // change render colour to a light blue for viewplane trim
    SDL_SetRenderDrawColor(renderer, 128, 128, 255, 128);

    // Draw intersecting line
    const float eps = 1e-4;
    line l1 = { { HALF_VIEW_WIDTH - eps, HALF_VIEW_WIDTH - eps }, { HALF_VIEW_WIDTH - (HALF_VIEW_WIDTH / 2), 0 } };
    line l2 = { { HALF_VIEW_WIDTH + eps, HALF_VIEW_WIDTH - eps }, { HALF_VIEW_WIDTH + (HALF_VIEW_WIDTH / 2), 0 } };
    draw_line_with_offset(renderer, l1, *offset);
    draw_line_with_offset(renderer, l2, *offset);
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
