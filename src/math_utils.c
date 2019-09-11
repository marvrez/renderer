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

static inline float determinant(float x1, float y1, float x2, float y2)
{
    return x1*y2 - x2*y1;
}

vec2 intersect(line a, line b)
{
    float x = determinant(a.start.x, a.start.y, a.end.x, a.end.y);
    float y = determinant(b.start.x, b.start.y, b.end.x, b.end.y);

    float det = determinant(a.start.x - a.end.x, a.start.y - a.end.y,
                            b.start.x - b.end.x, b.start.y - b.end.y);

    vec2 out;
    out.x = determinant(x, a.start.x - a.end.x, y, b.start.x - b.end.x) / det;
    out.y = determinant(x, a.start.y - a.end.y, y, b.start.y - b.end.y) / det;

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
