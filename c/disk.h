#ifndef MCMC_2D_DISK_DISK_H
#define MCMC_2D_DISK_DISK_H
#include "position.h"
#include <stdbool.h>

typedef struct {
    position p;
    double   r;
} disk;

bool overlaps(disk const* const lhs, disk const* const rhs);

#endif// MCMC_2D_DISK_DISK_H
