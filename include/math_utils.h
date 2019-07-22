#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define CLAMP(val, min_val, max_val) MIN(MAX(val, min_val), max_val)

typedef struct {
    float x;
    float y;
} vec2;

typedef struct {
    float x;
    float y;
    float z;
} vec3;

// generic math functions
float dot(vec2 u, vec2 v);
vec3 cross(vec3 u, vec3 v); 
vec2 intersect(vec2 u_start, vec2 u_end,
               vec2 v_start, vec2 v_end);
vec2 intersect_line_segments(vec2 u_start, vec2 u_end,
                             vec2 v_start, vec2 v_end);
float determinant(vec2 u, vec2 v);
vec3 normalize(vec3 v);
int is_zero_vec(vec2 v);

vec2 interpolate(float t, vec2 a, vec2 b);
