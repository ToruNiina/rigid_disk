#include "position.h"
#include "boundary.h"
#include "disk.h"
#include "dsfmt/dSFMT.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288419
#endif // M_PI

typedef struct {
    boundary bc;
    disk*    disks;
    size_t   num_disks;
} system_t;

void create_system(system_t* sys,
        position const* const low, position const* const up, size_t num_disks)
{
    sys->num_disks = 100;
    sys->disks = (disk*)malloc(sizeof(disk) * sys->num_disks);
    init_boundary(&sys->bc, low, up);
    return ;
}

void destroy_system(system_t* sys)
{
    free(sys->disks);
    return ;
}

void print_system(system_t const* sys)
{
    printf("%zu\n\n", sys->num_disks);
    for(size_t i=0; i<sys->num_disks; ++i)
    {
        printf("H     %8.3f %8.3f 0.0\n", sys->disks[i].p.x, sys->disks[i].p.y);
    }
    return;
}

// Fisher-Yates-Durstenfeld shuffle algorithm
void shuffle(size_t* array, size_t N, dsfmt_t* rng)
{
    for(size_t i=0; i<N-1; ++i)
    {
        size_t j = dsfmt_genrand_close_open(rng) * (N - i) + i;

        const size_t tmp = array[j];
        array[j] = array[i];
        array[i] = tmp;
    }
    return;
}

int main(int argc, char **argv)
{
    dsfmt_t rng;
    dsfmt_init_gen_rand(&rng, 123456789);

    const double radius    = 1.0;
    const size_t num_disks = 100;
    const double delta     = 0.3;

    //------------------------ initialize system ------------------------
    const position lower = { 0.0,  0.0};
    const position upper = {30.0, 30.0};
    system_t sys;
    create_system(&sys, &lower, &upper, num_disks);

    for(size_t n=0; n<num_disks; ++n)
    {
        while(true)
        {
            const position new_p = {
                dsfmt_genrand_close_open(&rng) * (upper.x - lower.x) + lower.x,
                dsfmt_genrand_close_open(&rng) * (upper.y - lower.y) + lower.y
            };

            const disk new_disk = {
                new_p, radius
            };

            bool collides=false;
            for(size_t i=0; i<n; ++i)
            {
                if(overlaps(&new_disk, &(sys.disks[i])))
                {
                    collides = true;
                    break;
                }
            }
            if(!collides)
            {
                sys.disks[n] = new_disk;
                break;
            }
        }
    }

    //------------------------------ MCMC ------------------------------

    size_t idx_list[num_disks]; // C99 VLA
    for(size_t i=0; i<num_disks; ++i)
    {
        idx_list[i] = i;
    }

    for(size_t t=0; t<10000; ++t)
    {
        print_system(&sys);
        shuffle(idx_list, num_disks, &rng);
        for(size_t i=0; i<num_disks; ++i)
        {
            const position dv = {
                dsfmt_genrand_close_open(&rng) * 2 * delta - delta,
                dsfmt_genrand_close_open(&rng) * 2 * delta - delta
            };

            const size_t idx = idx_list[i];
            disk target = sys.disks[idx];
            vec_add(&target.p, &dv);
            restrict_position(&target.p, &sys.bc);

            bool collides = false;
            for(size_t j=0; j<num_disks; ++j)
            {
                if(j == idx){continue;}

                if(overlaps(&target, &sys.disks[j]))
                {
                    collides = true;
                    break;
                }
            }
            if(!collides)
            {
                sys.disks[idx] = target;
            }
        }
    }
    print_system(&sys);

    destroy_system(&sys);
    return 0;
}
