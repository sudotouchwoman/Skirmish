// _LIB_CORE_INC_GEOMETRY_HPP
#pragma once  
#include <cmath>
#include <iostream>

// core namespace contains
// helper classes, the geometric primitives
// I will continue thinking on the architecture and
// build more complex entities on top of these
namespace core
{
    const double PI = M_PI;
    inline double degrees_to_rad(const double theta) { return theta * PI / 180.0; }
    inline double rad_to_degrees(const double theta) { return 180.0 / PI * theta; }

    bool allclose(const double a, const double b);
    struct vec2;
}  // namespace core

// 2-dimensional vector/dot/point
// I wanted to create a generic vector entity but did not succeeded
struct core::vec2 {
    double x = 0.0;
    double y = 0.0;
public:
    vec2() = default;
    vec2(const double x, const double y) : x(x), y(y) {}

    vec2 & operator+=(const vec2 & other) { x += other.x; y += other.y; return *this; }
    vec2 & operator-=(const vec2 & other) { x -= other.x; y -= other.y; return *this; }
    vec2 operator+(const vec2 &) const;
    vec2 operator-(const vec2 &) const;
    vec2 operator*(const double) const;
    vec2 operator-() const;

    void inverse() { x = -x; y = -y; }
    void rotate_r(const double theta_r);
    void rotate_d(const double theta_d);
    double angle_r() const;
    double angle_d() const;

    double distance_to_squared(const vec2 & other) const;
    double distance_to(const vec2 & other) const;
    double len_squared() const;
    double len() const;

    vec2 normalized() const;
    bool operator==(const vec2 &) const;
    bool operator!=(const vec2 &) const;

    static vec2 rotate_r(const vec2 & v, const double theta_r);
    static vec2 rotate_d(const vec2 & v, const double theta_d);
    static vec2 clamp(const vec2 & v, const vec2 & lb, const vec2 & ub);

    friend std::ostream & operator<<(std::ostream & out, const vec2 & v) {
        out << "core::vec2 object: X=" << v.x << " Y=" << v.y << std::endl;
        return out;
    }

    friend std::istream & operator>>(std::istream & in, vec2 & v) {
        in >> v.x >> v.y;
        return in;
    }
};

// _LIB_CORE_INC_GEOMETRY_HPP
