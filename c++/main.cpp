#include "position.hpp"
#include "boundary.hpp"
#include "disk.hpp"
#include <algorithm>
#include <numeric>
#include <vector>
#include <array>
#include <random>
#include <iostream>

namespace rigid_disk
{

struct system
{
    boundary          bc;
    std::vector<disk> disks;
};

void print_system(const system& sys)
{
    std::cout << sys.disks.size() << "\n\n";
    for(const auto& dsk : sys.disks)
    {
        std::cout << "H     " << dsk.p.x << ' ' << dsk.p.y << " 0.0\n";
    }
    return;
}
} // rigid_disk

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);

    std::mt19937 rng(123456789);

    constexpr double      radius    = 1.0;
    constexpr std::size_t num_disks = 100;
    constexpr double      delta     = 0.3;

    //------------------------ initialize system ------------------------
    const rigid_disk::position lower = { 0.0,  0.0};
    const rigid_disk::position upper = {30.0, 30.0};
    rigid_disk::system sys{rigid_disk::boundary{lower, upper}, {}};

    std::uniform_real_distribution<double> uni_x(lower.x, upper.x);
    std::uniform_real_distribution<double> uni_y(lower.y, upper.y);
    for(std::size_t n=0; n<num_disks; ++n)
    {
        while(true)
        {
            const rigid_disk::disk new_disk = {
                radius, rigid_disk::position{uni_x(rng), uni_y(rng)}
            };
            const auto overlap_with = [&](const rigid_disk::disk& dsk){
                return rigid_disk::overlaps(new_disk, dsk);
            };
            if(sys.disks.cend() == std::find_if(
                    sys.disks.cbegin(), sys.disks.cend(), overlap_with))
            {
                sys.disks.push_back(new_disk);
                break;
            }
        }
    }

    //------------------------------ MCMC ------------------------------

    std::uniform_real_distribution<double> displacement(-delta, delta);

    std::array<std::size_t, num_disks> idx_list;
    std::iota(idx_list.begin(), idx_list.end(), 0ul);

    for(std::size_t t=0; t<10000; ++t)
    {
        rigid_disk::print_system(sys);
        std::shuffle(idx_list.begin(), idx_list.end(), rng);

        for(std::size_t idx : idx_list)
        {
            rigid_disk::disk target = sys.disks[idx];

            const rigid_disk::position dv = {
                displacement(rng), displacement(rng)
            };
            target.p = restrict_position(target.p + dv, sys.bc);

            bool collides = false;
            for(size_t j=0; j<num_disks; ++j)
            {
                if(j == idx){continue;}

                if(rigid_disk::overlaps(target, sys.disks[j]))
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
    rigid_disk::print_system(sys);
    return 0;
}
