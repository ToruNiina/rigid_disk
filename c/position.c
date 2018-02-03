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

double vec_length_sq(position const* const vec)
{
    return vec->x * vec->x + vec->y * vec->y;
}

