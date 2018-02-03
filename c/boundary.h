#ifndef MCMC_2D_DISK_BOUNDARY_H
#define MCMC_2D_DISK_BOUNDARY_H
#include "position.h"

typedef struct {
    position lower, upper;
    position width, half_width;
} boundary;

void init_boundary(boundary* const bc,
                   position const* const low, position const* const up);

void restrict_position (position* const pos, boundary const* const bc);
void restrict_direction(position* const pos, boundary const* const bc);

#endif// MCMC_2D_DISK_BOUNDARY_H
