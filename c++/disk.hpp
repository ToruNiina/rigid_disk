#ifndef MCMC_2D_DISK_DISK_H
#define MCMC_2D_DISK_DISK_H
#include "position.hpp"

namespace rigid_disk
{
struct disk
{
    double   r;
    position p;
};

inline bool overlaps(const disk& lhs, const disk& rhs)
{
    return length_sq(lhs.p - rhs.p) < (lhs.r + rhs.r) * (lhs.r + rhs.r);
}

}// rigid_disk
#endif// MCMC_2D_DISK_DISK_H
