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

void draw_wall(program_state* state, wall_line wl)
{
    player p = state->p;
    SDL_Point offset;

    // used for drawing the current line, start by assigning the line points of the current 'wall' to AbsoluteLine
    line absolute_line = wl.l;
    // transform line to be relative to the player
    line relative_line = {{ absolute_line.start.x - p.pos.x, p.pos.y - absolute_line.start.y },
                          { absolute_line.end.x - p.pos.x,   p.pos.y - absolute_line.end.y }};

    float pcos = cosf(p.angle), psin = sinf(p.angle);
    // rotate them to be around the player's view (90-player.angle) degrees
    // multiply x coordinates by -1 because we want to view the inverse change of X in the transformed view
    line transformed_line = {{ -(relative_line.start.x*psin - relative_line.start.y*pcos), relative_line.start.x*pcos + relative_line.start.y*psin },
                             { -(relative_line.end.x*psin - relative_line.end.y*pcos),     relative_line.end.x*pcos + relative_line.end.y*psin }};
    float *tz1 = &transformed_line.start.y, *tz2 = &transformed_line.end.y;

    /*  PERSPECTIVE VIEW / PROJECTION */
    offset.x = PERSPECTIVE_VIEW.x, offset.y = PERSPECTIVE_VIEW.y;

    // If either point of the wall line is behind the player, ignore the line
    if(*tz1 <= 0.0 && *tz2 <= 0.0) return;

    // small epsilon to avoid dividing by zero in intersection calculations
    line l1 = { {-EPS, EPS}, {-HALF_VIEW_WIDTH/2, EPS} };
    line l2 = { {EPS, EPS}, {HALF_VIEW_WIDTH/2, EPS} };
    vec2 intersect1 = intersect(transformed_line, l1), intersect2 = intersect(transformed_line, l2);
    // If the line is partially behind the player (crosses the viewplane, clip it)
    // If intersect1 is behind the player
    if(*tz1 <= 0.0) {
        transformed_line.start.x = intersect1.y > 0.0 ? intersect1.x : intersect2.x;
        transformed_line.start.y = intersect1.y > 0.0 ? intersect1.y : intersect2.y;
    }
    // If intersect2 is behind the player
    if(*tz2 <= 0.0) {
        transformed_line.end.x = intersect1.y > 0.0 ? intersect1.x : intersect2.x;
        transformed_line.end.y = intersect1.y > 0.0 ? intersect1.y : intersect2.y;
    }

    /* PERSPECTIVE DIVIDE */
    // HFOV = 53.13, x and y scalars are set depending on window height and width
    float xscale = PERSPECTIVE_VIEW.w, yscale = 8*PERSPECTIVE_VIEW.h;
    if(p.height == CROUCHING_HEIGHT) yscale += STANDING_HEIGHT - CROUCHING_HEIGHT;

    float x1 = -(transformed_line.start.x * xscale) / *tz1; // perspective divides to get verticies co-ords of wall to draw.
    float y1_top = -yscale / *tz1; // negative value for yscale is used as y coords on screen decrement as we get closer to the top of the window
    float y1_bot = p.height / *tz1; // player height is used as this is how much wall you want to draw under the centre line of the screen giving the experience of height.
    float x2 = -(transformed_line.end.x * xscale) / *tz2;
    float y2_top = -yscale / *tz2;
    float y2_bot = p.height / *tz2;

    // Data used to draw the wall. This structure holds the values of the '3d wall' projected on to the 2d 'camera'.
    wall w = {{x1, y1_top}, {x2, y2_top}, {x2, y2_bot}, {x1, y1_bot}}; 
    line top_wall_line = { w.top_left, w.top_right };
    line bot_wall_line = { w.bottom_left, w.bottom_right };

    line left_window_line = { {-HALF_WINDOW_WIDTH + 1, -HALF_WINDOW_HEIGHT}, {-HALF_WINDOW_WIDTH + 1, HALF_WINDOW_HEIGHT} };
    line right_window_line = { {HALF_WINDOW_WIDTH - 2, -HALF_WINDOW_HEIGHT}, {HALF_WINDOW_WIDTH - 2, HALF_WINDOW_HEIGHT} };

    // clip if out of view on left side
    vec2 clip_top = intersect(top_wall_line, left_window_line), clip_bot = intersect(bot_wall_line, left_window_line);
    if(w.top_left.x <= -HALF_WINDOW_WIDTH + 1) {
        w.top_left = clip_top;
        w.bottom_left.x = clip_top.x, w.bottom_left.y = clip_bot.y;
    }
    if(w.top_right.x <= -HALF_WINDOW_WIDTH + 1) {
        w.top_right = clip_top;
        w.bottom_right.x = clip_top.x, w.bottom_right.y = clip_bot.y;
    }
    // clip if out of view on right side
    clip_top = intersect(top_wall_line, right_window_line), clip_bot = intersect(bot_wall_line, right_window_line);
    if(w.top_left.x >= HALF_WINDOW_WIDTH - 1) {
        w.top_left = clip_top;
        w.bottom_left.x = clip_top.x, w.bottom_left.y = clip_bot.y;
    }
    if(w.top_right.x >= HALF_WINDOW_WIDTH - 1) {
        w.top_right = clip_top;
        w.bottom_right.x = clip_top.x, w.bottom_right.y = clip_bot.y;
    }

    // Calculations for drawing the vertical lines of the wall, floor and ceiling.
    float left_most_wall = MIN(w.top_left.x, w.top_right.x), right_most_wall = MAX(w.top_left.x, w.top_right.x);
    float wall_width = right_most_wall - left_most_wall;
    if(wall_width <= 1) return;

    float dh_top = w.top_right.y - w.top_left.y, dh_bot = w.bottom_left.y - w.bottom_right.y; // height deltas
    for(int cl = left_most_wall; cl <= right_most_wall; ++cl) { // Loop over each x position of the wall. cl = current vertical line
        float dy_top = (cl - left_most_wall) * (dh_top / wall_width); // Calculate the change in Y position for the current vertical line.  Used for the ceiling vertical line.
        float dy_bot = (cl - left_most_wall) * (dh_bot / wall_width); // Calculate the change in Y position for the current vertical line.  Used for the floor vertical line.

        float wall_top = w.top_left.y + dy_top; // Calculate the Y position to draw the vertical ceiling line to.
        if(wall_top < -HALF_WINDOW_HEIGHT) wall_top = -HALF_WINDOW_HEIGHT; // Set a minimum for this value to ensure vertical ceiling line at least starts at the top of the viewport and is not drawn above that (non-visible area);
        float wall_bot = w.bottom_left.y - dy_bot; // Calculate the Y position to draw the vertical floor line from.
        if(wall_bot > HALF_WINDOW_HEIGHT) wall_bot = HALF_WINDOW_HEIGHT;

        // Change render colour to the ceiling color. Lines are drawn relative to the centre of the player's view.
        SDL_SetRenderDrawColor(state->renderer, CEILING_COLOR.r, CEILING_COLOR.g, CEILING_COLOR.b, CEILING_COLOR.a);
        line ceiling_line = { {HALF_WINDOW_WIDTH + cl, HALF_WINDOW_HEIGHT + wall_top}, {HALF_WINDOW_WIDTH + cl, 1} };
        draw_line_with_offset(state->renderer, ceiling_line, offset);

        // Change render colour to the floor color. Lines are drawn relative to the centre of the player's view.
        SDL_SetRenderDrawColor(state->renderer, FLOOR_COLOR.r, FLOOR_COLOR.g, FLOOR_COLOR.b, FLOOR_COLOR.a);
        line floor_line = { {HALF_WINDOW_WIDTH + cl, HALF_WINDOW_HEIGHT + wall_bot}, {HALF_WINDOW_WIDTH + cl, WINDOW_HEIGHT - 2} };
        draw_line_with_offset(state->renderer, floor_line, offset);

        // Draw Wall
        // Change render colour to the wall color & apply lighting. Lines are drawn relative to the centre of the player's view.
        SDL_SetRenderDrawColor(state->renderer, wl.color.r, wl.color.g, wl.color.b, wl.color.a);
        line final_wall_line = { {HALF_WINDOW_WIDTH + cl, HALF_WINDOW_HEIGHT + wall_top}, {HALF_WINDOW_WIDTH + cl, HALF_WINDOW_HEIGHT + wall_bot} };
        draw_line_with_offset(state->renderer, final_wall_line, offset);
    }
    // Change render colour to green
    SDL_SetRenderDrawColor(state->renderer, 0, 255, 0, 255);
    // Draw top line of wall
    line shifted_top_wall_line = { {HALF_WINDOW_WIDTH + w.top_left.x, HALF_WINDOW_HEIGHT + w.top_left.y}, {HALF_WINDOW_WIDTH + w.top_right.x, HALF_WINDOW_HEIGHT + w.top_right.y} };
    draw_line_with_offset(state->renderer, shifted_top_wall_line, offset);
    // Draw bottom line of wall
    line shifted_bottom_wall_line = { {HALF_WINDOW_WIDTH + w.bottom_left.x, HALF_WINDOW_HEIGHT + w.bottom_left.y}, {HALF_WINDOW_WIDTH + w.bottom_right.x, HALF_WINDOW_HEIGHT + w.bottom_right.y} };
    draw_line_with_offset(state->renderer, shifted_bottom_wall_line, offset);
    // Draw left line of wall
    line shifted_left_wall_line = { {HALF_WINDOW_WIDTH + w.top_left.x, HALF_WINDOW_HEIGHT + w.top_left.y}, {HALF_WINDOW_WIDTH + w.bottom_left.x, HALF_WINDOW_HEIGHT + w.bottom_left.y} };
    draw_line_with_offset(state->renderer, shifted_left_wall_line, offset);
    // Draw right line of wall
    line shifted_right_wall_line = { {HALF_WINDOW_WIDTH + w.top_right.x, HALF_WINDOW_HEIGHT + w.top_right.y}, {HALF_WINDOW_WIDTH + w.bottom_right.x, HALF_WINDOW_HEIGHT + w.bottom_right.y}};
    draw_line_with_offset(state->renderer, shifted_right_wall_line, offset);
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
    line relative_line = {{ absolute_line.start.x - p.pos.x, p.pos.y - absolute_line.start.y },
                          { absolute_line.end.x - p.pos.x,   p.pos.y - absolute_line.end.y }};

    // rotate line to be around the player's view (90-player.angle) degrees
    // multiply X-coordinates by -1 because we want to view the inverse change of X in the transformed view
    line transformed_line = {{ -(relative_line.start.x*psin - relative_line.start.y*pcos), relative_line.start.x*pcos + relative_line.start.y*psin },
                             { -(relative_line.end.x*psin - relative_line.end.y*pcos),     relative_line.end.x*pcos + relative_line.end.y*psin }};

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
    line transformed_line_centered = {
        { HALF_VIEW_WIDTH - transformed_line.start.x, HALF_VIEW_WIDTH - transformed_line.start.y },
        { HALF_VIEW_WIDTH - transformed_line.end.x,   HALF_VIEW_WIDTH - transformed_line.end.y }
    };
    draw_line_with_offset(state->renderer, transformed_line_centered, offset);
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
