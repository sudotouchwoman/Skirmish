// _LIB_CORE_INC_CORE_HPP
#pragma once  

// core namespace contains
// helper classes, the geometric primitives
// I will continue thinking on the architecture and
// build more complex entities on top of these
namespace core
{
    const double PI = 3.141592653589793238462643383;
    double degrees_to_rad(const double theta);
    bool allclose(const double a, const double b);
    struct vec2;
    vec2 rotate_vector(const vec2 & v, const double theta);
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
    vec2 operator+(const vec2 &);
    vec2 operator-(const vec2 &);
    void rotate(const double theta);
    double distance_to_squared(const vec2 & other) const;
    double distance_to(const vec2 & other) const;
    double len_squared() const;
    double len() const;
    vec2 normalized() const;
    bool operator==(const vec2 &) const;
    bool operator!=(const vec2 &) const;
};

// _LIB_CORE_INC_CORE_HPP
