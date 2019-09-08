#include "math_utils.h"

#define M_EPS 1e-6

float dot(vec2 u, vec2 v)
{
    return u.x*v.x + u.y*v.y;
}

vec3 cross(vec3 u, vec3 v)
{
    vec3 out;
    out.x = u.y*v.z - u.z*v.y;
    out.y = u.z*v.x - u.x*v.z;
    out.z = u.x*v.y - u.y*v.x;
    return out;
}

static inline float determinant(float* u, float* v)
{
    return u[0]*v[1] - v[0]*u[1];
}

vec2 intersect(line a, line b)
{
    vec2 out = { 0.f, 0.f };


    float x_diffs[] = { a.start.x - a.end.x, b.start.x - b.end.x };
    float y_diffs[] = { a.start.y - a.end.y, b.start.y - b.end.y };

    float denom = determinant(x_diffs, y_diffs);

    if(denom <= M_EPS) return out;

    float t = ((a.start.x - b.start.x)*(b.start.y - b.end.y) - (a.start.y - b.start.y)*(b.start.x - b.end.x)) / denom;
    float u = -((a.start.x - a.end.x)*(a.start.y - b.start.y) - (a.start.y - a.end.y)*(a.start.x - b.start.x)) / denom;

    if(t > 1.f || t < 0.f || u > 1.f || u < 0.f) return out;

    out.x = a.start.x + t*(a.end.x - a.start.x);
    out.y = a.start.y + t*(a.end.y - a.start.y);

    return out;
}

vec3 normalize(vec3 v)
{
    vec3 out;
    float normalizing_factor = 1.f / sqrtf(v.x*v.x* + v.y*v.y + v.z*v.z);

    out.x = v.x * normalizing_factor;
    out.y = v.y * normalizing_factor;
    out.z = v.z * normalizing_factor;

    return out;
}

int is_zero_vec(vec2 v)
{
    return v.x <= M_EPS && v.y <= M_EPS;
}

vec2 interpolate(float t, vec2 a, vec2 b)
{
    t = CLAMP(t, 0.f, 1.f);
    vec2 out = { a.x + t*(b.x - a.x), a.y + t*(b.y-a.y) };
    return out;
}
