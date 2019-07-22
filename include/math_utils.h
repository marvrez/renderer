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
vec2 intersect(float x1, float y1, float x2, float y2,
               float x3, float y3, float x4, float y4);
vec2 intersect_line_segments(float x1, float y1, float x2, float y2, 
                             float x3, float y3, float x4, float y4);
float determinant(float x1, float y1, float x2, float y2);
vec3 normalize(vec3 v);
int is_zero_vec(vec2 v);

vec2 interpolate(float t, vec2 a, vec2 b);
