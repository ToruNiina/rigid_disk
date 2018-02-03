#include "boundary.h"

void init_boundary(boundary* const bc,
                   position const* const low, position const* const up)
{
    bc->lower = *low;
    bc->upper = *up;

    bc->width = *up;
    vec_sub(&(bc->width), low);

    bc->half_width = bc->width;
    vec_mul(&(bc->half_width), 0.5);
    return;
}

void restrict_position(position* const pos, boundary const* const bc)
{
    if     (pos->x      < bc->lower.x) {pos->x += bc->width.x;}
    else if(bc->upper.x < pos->x)      {pos->x -= bc->width.x;}
    if     (pos->y      < bc->lower.y) {pos->y += bc->width.y;}
    else if(bc->upper.y < pos->y)      {pos->y -= bc->width.y;}
    return;
}

void restrict_direction(position* const dir, boundary const* const bc)
{
    if     (dir->x            < -bc->half_width.x) {dir->x += bc->width.x;}
    else if(bc->half_width.x < dir->x)             {dir->x -= bc->width.x;}
    if     (dir->y            < -bc->half_width.y) {dir->y += bc->width.y;}
    else if(bc->half_width.y < dir->y)             {dir->y -= bc->width.y;}
    return;
}
