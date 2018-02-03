#include "disk.h"

bool overlaps(disk const* const lhs, disk const* const rhs,
              boundary const* const bc)
{
    position vec = lhs->p;
    vec_sub(&vec, &rhs->p);
    restrict_direction(&vec, bc);

    const double d2 = vec_length_sq(&vec);
    const double rs = lhs->r + rhs->r;
    return d2 < rs * rs;
}
