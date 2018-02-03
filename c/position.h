#ifndef MCMC_2D_DISK_POSITION_H
#define MCMC_2D_DISK_POSITION_H

typedef struct {
    double x, y;
} position;

void   vec_add(position* const lhs, position const* const rhs);
void   vec_sub(position* const lhs, position const* const rhs);
void   vec_mul(position* const lhs, double const rhs);
void   vec_div(position* const lhs, double const rhs);
double vec_length_sq(position const* const vec);

#endif// MCMC_2D_DISK_POSITION_H
