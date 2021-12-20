#include "geometry.hpp"

#include <limits>
#include <algorithm>

namespace core {

bool allclose(const double a, const double b) {
    return
        a == b or
        std::abs(a-b) < 
        std::abs(
            std::min(a, b)*std::numeric_limits<double>::epsilon());
}

vec2 vec2::rotate_r(const vec2 & v, const double theta_r) {
    vec2 tmp(v);
    tmp.rotate_r(theta_r);
    return tmp;
}

vec2 vec2::rotate_d(const vec2 & v, const double theta_d) {
    vec2 tmp(v);
    tmp.rotate_d(theta_d);
    return tmp;
}

vec2 vec2::clamp(const vec2 & v, const vec2 & lb, const vec2 & ub) {
    const double x_clamped = std::min(std::max(v.x, lb.x), ub.x);
    const double y_clamped = std::min(std::max(v.y, lb.y), ub.y);
    return vec2(x_clamped, y_clamped);
}

vec2 vec2::operator+(const vec2 & other) const {
    vec2 tmp;
    tmp.x = x + other.x;
    tmp.y = y + other.y;
    return tmp;
}

vec2 vec2::operator-(const vec2 & other) const {
    vec2 tmp;
    tmp.x = x - other.x;
    tmp.y = y - other.y;
    return tmp;
}

vec2 vec2::operator*(const double a) const {
    vec2 tmp;
    tmp.x = a * x;
    tmp.y = a * y;
    return tmp;
}

vec2 vec2::operator-() const {
    vec2 tmp;
    tmp.x = -x;
    tmp.y = -y;
    return tmp;
}

void vec2::rotate_d(const double theta_d) {
    const double theta_r = degrees_to_rad(theta_d);
    const double x_ = x * std::cos(theta_r) - y * std::sin(theta_r);
    const double y_ = x * std::sin(theta_r) + y * std::cos(theta_r);
    x = std::round(x_);
    y = std::round(y_);
}

void vec2::rotate_r(const double theta_r) {
    const double x_ = x * std::cos(theta_r) - y * std::sin(theta_r);
    const double y_ = x * std::sin(theta_r) + y * std::cos(theta_r);
    x = std::round(x_);
    y = std::round(y_);
}

double vec2::angle_r() const {
    return std::atan2(y, x);
}

double vec2::angle_d() const {   
    return rad_to_degrees(std::atan2(y, x));
}

double vec2::len_squared() const {
    return x * x + y * y;
}

double vec2::len() const {
    return sqrt(len_squared());
}

vec2 vec2::normalized() const {
    const double length = len();
    if (allclose(length, 0.0)) return vec2();
    return vec2(std::round(x / length), std::round(y / length));
}

double vec2::distance_to_squared(const vec2 & other) const {
    const double dx = (x - other.x)*(x - other.x);
    const double dy = (y - other.y)*(y - other.y);
    return dx + dy;
}

double vec2::distance_to(const vec2 & other) const {
    return sqrt(distance_to_squared(other));
}

bool vec2::operator==(const vec2 & other) const {
    return allclose(x, other.x) and allclose(y, other.y);
}

bool vec2::operator!=(const vec2 & other) const {
    return not (*this == other);
}

}  // namespace core