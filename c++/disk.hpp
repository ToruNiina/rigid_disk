#ifndef MCMC_2D_DISK_DISK_H
#define MCMC_2D_DISK_DISK_H
#include "position.hpp"
#include "boundary.hpp"

namespace rigid_disk
{
struct disk
{
    double   r;
    position p;
};

inline bool overlaps(const disk& lhs, const disk& rhs, const boundary& bc)
{
    return length_sq(restrict_direction(lhs.p - rhs.p, bc)) <
           (lhs.r + rhs.r) * (lhs.r + rhs.r);
}

}// rigid_disk
#endif// MCMC_2D_DISK_DISK_H
