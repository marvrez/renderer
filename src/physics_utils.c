#include "physics_utils.h"
#include "parameters.h"

static inline int player_in_bounds(player p, wall_line wl)
{
    line l = wl.l;
    float det = (l.start.x - l.end.x)*(l.start.y - p.pos.y) 
               - (l.start.x - p.pos.x)*(l.start.y - l.end.y);
    return det > 0.f;
}

int is_player_colliding_with_wall(player p)
{
    for(int i = 0; i < NUM_WALLS; ++i) {
        wall_line wl = WALL_LINES[i];
        if(!player_in_bounds(p, wl)) {
            return 1;
        }
    }
    return 0;
}
