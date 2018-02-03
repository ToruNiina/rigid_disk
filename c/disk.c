#include "disk.h"

bool overlaps(disk const* const lhs, disk const* const rhs)
{
    const double d2 = vec_distance_sq(&(lhs->p), &(rhs->p));
    const double rs = lhs->r + rhs->r;
    return d2 < rs * rs;
}
