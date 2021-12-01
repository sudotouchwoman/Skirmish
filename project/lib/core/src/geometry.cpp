#include <cmath>
#include <limits>
#include <algorithm>

#include "core.hpp"

namespace core {
    double degrees_to_rad(const double theta) {
    return theta * PI / 180.0;
    }

    bool allclose(const double a, const double b) {
        return
            a == b or
            std::abs(a-b) < 
            std::abs(
                std::min(a, b)*std::numeric_limits<double>::epsilon());
    }

    vec2 rotate_vector(const vec2 & v, const double theta) {
        vec2 tmp(v);
        tmp.rotate(theta);
        return tmp;
    }

    vec2 vec2::operator+(const vec2 & other) {
        vec2 tmp;
        tmp.x = x + other.x;
        tmp.y = y + other.y;
        return tmp;
    }

    vec2 vec2::operator-(const vec2 & other) {
        vec2 tmp;
        tmp.x = x - other.x;
        tmp.y = y - other.y;
        return tmp;
    }

    void vec2::rotate(const double theta) {
        const double rad_theta = degrees_to_rad(theta);
        const double x_ = x * std::cos(rad_theta) - y * std::sin(rad_theta);
        const double y_ = x * std::sin(rad_theta) + y * std::cos(rad_theta);
        x = std::round(x_);
        y = std::round(y_);
    }

    double vec2::len_squared() const {
        return x * x + y * y;
    }

    double vec2::len() const {
        return sqrt(len_squared());
    }

    vec2 vec2::normalized() const {
        const double length = len();
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
}