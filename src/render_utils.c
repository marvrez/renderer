#include "render_utils.h"

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
