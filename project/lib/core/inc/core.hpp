// _LIB_CORE_INC_CORE_HPP
#pragma once  
#include <cmath>

// core namespace contains
// helper classes, the geometric primitives
// 3D and 2D versions were implemented, however,
// the 3D versions are considered odd
// and are not likely to be utilized
// I will continue thinking on the architecture and
// build more complex entities on top of these
namespace core
{
    class dot3;
    class vec3;

    class dot2;
    class vec2;
};  // namespace core

// pretty self-explanatory
class core::dot3 {
    double _x = 0.0;
    double _y = 0.0;
    double _z = 0.0;
public:
    dot3(const double x = 0.0, const double y = 0.0, const double z = 0.0) : _x(x), _y(y), _z(z) {}
    dot3 & operator+=(const dot3 & other) { _x += other._x; _y += other._y; _z += other._z; return *this; }
    dot3 & operator-=(const dot3 & other) { _x -= other._x; _y -= other._y; _z -= other._z; return *this; }
    dot3 operator+(const dot3 &);
    dot3 operator-(const dot3 &);
    double const & x() const { return _x; }
    double const & y() const { return _y; }
    double const & z() const { return _z; }
};

core::dot3 core::dot3::operator+(const dot3 & other) {
    dot3 tmp;
    tmp._x = _x + other._x;
    tmp._y = _y + other._y;
    tmp._z = _z + other._z;
    return tmp;
}

core::dot3 core::dot3::operator-(const dot3 & other) {
    dot3 tmp;
    tmp._x = _x - other._x;
    tmp._y = _y - other._y;
    tmp._z = _z - other._z;
    return tmp;
}

// 3-dimensional vector, basically just wraps 2 3D dots
// maybe addition of angle attribute is a good idea
class core::vec3 {
    dot3 _src;
    dot3 _dst;
public:
    vec3() = default;
    vec3(const dot3 & _src, const dot3 & _dst) : _src(_src), _dst(_dst) {}
    vec3 & operator+=(const vec3 & other) { _src += other._src; _dst += other._dst; return *this; }
    vec3 & operator-=(const vec3 & other) { _src -= other._src; _dst -= other._dst; return *this; }
    vec3 operator+(const vec3 &);
    vec3 operator-(const vec3 &);
};

core::vec3 core::vec3::operator+(const vec3 & other) {
    vec3 tmp;
    tmp._src = _src + other._src;
    tmp._dst = _dst + other._dst;
    return tmp;
}

core::vec3 core::vec3::operator-(const vec3 & other) {
    vec3 tmp;
    tmp._src = _src - other._src;
    tmp._dst = _dst - other._dst;
    return tmp;
}

// 2-dimensional versions
// the only major difference is the rotation interface as
// only a single angle in 2D case is possible (around z-axis)
// I wanted to create a generic option but did not succeed
class core::dot2 {
    double _x = 0.0;
    double _y = 0.0;
public:
    dot2() = default;
    dot2(const double x, const double y) : _x(x), _y(y) {}
    dot2 & operator+=(const dot2 & other) { _x += other._x; _y += other._y; return *this; }
    dot2 & operator-=(const dot2 & other) { _x -= other._x; _y -= other._y; return *this; }
    dot2 operator+(const dot2 &);
    dot2 operator-(const dot2 &);
    double const & x() const { return _x; }
    double const & y() const { return _y; }
};

core::dot2 core::dot2::operator+(const dot2 & other) {
    dot2 tmp;
    tmp._x = _x + other._x;
    tmp._y = _y + other._y;
    return tmp;
}

core::dot2 core::dot2::operator-(const dot2 & other) {
    dot2 tmp;
    tmp._x = _x - other._x;
    tmp._y = _y - other._y;
    return tmp;
}

// 2D vector
// the angle attribute still sounds like a good idea
// however, this one can be easily obtained from
// the dots via, well, class method or smth
// keeping it up to date after each transformation can
// be odd
class core::vec2 {
    dot2 _src;
    dot2 _dst;
public:
    vec2() = default;
    vec2(const dot2 & _src, const dot2 & _dst) : _src(_src), _dst(_dst) {}
    vec2 & operator+=(const vec2 & other) { _src += other._src; _dst += other._dst; return *this; }
    vec2 & operator-=(const vec2 & other) { _src -= other._src; _dst -= other._dst; return *this; }
    vec2 operator+(const vec2 &);
    vec2 operator-(const vec2 &);
    constexpr dot2 const & src() { return _src; }
    constexpr dot2 const & dst() { return _dst; }
    double norm() const;
    void rotate(const double);
    void inverse();
};

core::vec2 core::vec2::operator+(const vec2 & other) {
    vec2 tmp;
    tmp._src = _src + other._src;
    tmp._dst = _dst + other._dst;
    return tmp;
}

core::vec2 core::vec2::operator-(const vec2 & other) {
    vec2 tmp;
    tmp._src = _src - other._src;
    tmp._dst = _dst - other._dst;
    return tmp;
}

double core::vec2::norm() const {
    const double dx = _dst.x() - _src.x();
    const double dy = _dst.y() - _src.y();
    return dx * dx + dy * dy;
}

void core::vec2::rotate(const double angle) {
    const double dx = _dst.x() - _src.x();
    const double dy = _dst.y() - _src.y();

    const double new_x = dx * cos(angle) - dy * sin(angle);
    const double new_y = dx * sin(angle) + dy * cos(angle);

    _dst = _src + core::dot2(new_x, new_y);
}

void core::vec2::inverse() {
    std::swap(_src, _dst);
}

// _LIB_CORE_INC_CORE_HPP
