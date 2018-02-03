#ifndef MCMC_2D_DISK_POSITION_HPP
#define MCMC_2D_DISK_POSITION_HPP
#include <cmath>

namespace rigid_disk
{

struct position
{
    double x, y;

    position& operator+=(const position& rhs) noexcept
    {
        x += rhs.x;
        y += rhs.y;
    }
    position& operator-=(const position& rhs) noexcept
    {
        x -= rhs.x;
        y -= rhs.y;
    }
    position& operator*=(const double rhs) noexcept
    {
        x *= rhs;
        y *= rhs;
    }
    position& operator/=(const double rhs) noexcept
    {
        x /= rhs;
        y /= rhs;
    }
};

inline position operator+(const position& lhs, const position& rhs) noexcept
{
    return position{lhs.x + rhs.x, lhs.y + rhs.y};
}
inline position operator-(const position& lhs, const position& rhs) noexcept
{
    return position{lhs.x - rhs.x, lhs.y - rhs.y};
}
inline position operator*(const position& lhs, const double rhs) noexcept
{
    return position{lhs.x * rhs, lhs.y * rhs};
}
inline position operator*(const double lhs, const position& rhs) noexcept
{
    return position{lhs * rhs.x, lhs * rhs.y};
}
inline position operator/(const position& lhs, const double rhs) noexcept
{
    return position{lhs.x / rhs, lhs.y / rhs};
}

inline double length_sq(const position& v)
{
    return v.x * v.x + v.y * v.y;
}
inline double length(const position& v)
{
    return std::sqrt(length_sq(v));
}

}// rigid_disk
#endif// MCMC_2D_DISK_POSITION_H
