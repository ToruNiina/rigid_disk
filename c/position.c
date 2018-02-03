#include "position.h"
#include <math.h>

void vec_add(position* const lhs, position const* const rhs)
{
    lhs->x += rhs->x;
    lhs->y += rhs->y;
    return;
}

void vec_sub(position* const lhs, position const* const rhs)
{
    lhs->x -= rhs->x;
    lhs->y -= rhs->y;
    return;
}

void vec_mul(position* const lhs, double const rhs)
{
    lhs->x *= rhs;
    lhs->y *= rhs;
    return;
}

void vec_div(position* const lhs, double const rhs)
{
    lhs->x /= rhs;
    lhs->y /= rhs;
    return;
}

double vec_distance_sq(position const* const lhs, position const* const rhs)
{
    position dr = *lhs;
    vec_sub(&dr, rhs);
    return dr.x * dr.x + dr.y * dr.y;
}

double vec_distance(position const* const lhs, position const* const rhs)
{
    return sqrt(vec_distance_sq(lhs, rhs));
}
