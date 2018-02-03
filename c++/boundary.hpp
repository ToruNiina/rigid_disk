#ifndef MCMC_2D_DISK_BOUNDARY_H
#define MCMC_2D_DISK_BOUNDARY_H
#include "position.hpp"

namespace rigid_disk
{
class boundary
{
  public:
    boundary(const position& low, const position& up) noexcept
      : lower_(low), upper_(up), width_(up - low), half_width_((up - low) / 2)
    {}

    position const& lower()      const noexcept {return this->lower_;}
    position const& upper()      const noexcept {return this->upper_;}
    position const& width()      const noexcept {return this->width_;}
    position const& half_width() const noexcept {return this->half_width_;}

  private:

    position lower_, upper_;
    position width_, half_width_;
};

inline position restrict_position(position pos, const boundary& bc) noexcept
{
    if     (pos.x      < bc.lower().x) {pos.x += bc.width().x;}
    else if(bc.upper().x < pos.x)      {pos.x -= bc.width().x;}
    if     (pos.y      < bc.lower().y) {pos.y += bc.width().y;}
    else if(bc.upper().y < pos.y)      {pos.y -= bc.width().y;}
    return pos;
}
inline position restrict_direction(position vec, const boundary& bc) noexcept
{
    if     (vec.x            < -bc.half_width().x) {vec.x += bc.width().x;}
    else if(bc.half_width().x < vec.x)             {vec.x -= bc.width().x;}
    if     (vec.y            < -bc.half_width().y) {vec.y += bc.width().y;}
    else if(bc.half_width().y < vec.y)             {vec.y -= bc.width().y;}
    return vec;
}

}// rigid_disk
#endif// MCMC_2D_DISK_BOUNDARY_H
