#ifndef MATH_UTILS_H
#define MATH_UTILS_H

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

typedef struct {
    vec2 start;
    vec2 end;
} line;

line make_line(vec2 start, vec2 end);

// generic math functions
float dot(vec2 u, vec2 v);
vec3 cross(vec3 u, vec3 v); 
vec2 intersect(line a, line b);
vec2 intersect_line_segments(line a, line b);
float determinant(vec2 u, vec2 v);
vec3 normalize(vec3 v);
int is_zero_vec(vec2 v);

vec2 interpolate(float t, vec2 a, vec2 b);

#endif
