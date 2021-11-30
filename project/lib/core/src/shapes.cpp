#include "shapes.hpp"
#include <memory>
#include <array>
#include <algorithm>

namespace core {
    using IShapeUPtr = std::unique_ptr<IShape>;

    double AABB::_getLeft() const {
        return center.x - width / 2;
    }

    double AABB::_getRight() const {
        return center.x + width / 2;
    }

    double AABB::_getTop() const {
        return center.y + height / 2;
    }

    double AABB::_getBottom() const {
        return center.y - height / 2;
    }

    AABB::AABB(const vec2 & center, const double w, const double h) :
    center(center), width(w), height(h) {}

    AABB::AABB(
        const double center_x,
        double center_y,
        const double w,
        const double h) :
        AABB(vec2(center_x, center_y), w, h) {}

    bool AABB::LiesInside(const vec2 & dot) const {
        const bool a = (_getRight() >= dot.x);
        const bool b = (_getLeft() <= dot.x);
        const bool c = (_getTop() >= dot.y);
        const bool d = (_getBottom() <= dot.y);

        return a and b and c and d;
    }

    bool AABB::IntersectsWith(const AABB & other) const {
        if (&other == this) return true;

        const bool isLefter = _getLeft() >= other._getRight();
        const bool isRighter = _getRight() <= other._getLeft();
        const bool isAbove = _getBottom() >= other._getTop();
        const bool isBelow = _getTop() <= other._getBottom();

        return not (
            isLefter or isRighter or isAbove or isBelow
        );
    }

    bool AABB::IntersectsWith(const Circle & other) const {
        const double R = other.GetRadius();
        const vec2 & other_center = other.GetCenter();

        const std::array<IShapeUPtr, 6> shapes = {
                std::make_unique<Circle>(TopLeft(), R),
                std::make_unique<Circle>(TopRight(), R),
                std::make_unique<Circle>(BottomLeft(), R),
                std::make_unique<Circle>(BottomRight(), R),
                std::make_unique<AABB>(center, width, height + 2*R),
                std::make_unique<AABB>(center, width + 2*R, height)
                };

        return std::any_of(
            shapes.begin(),
            shapes.end(),
            [&other_center](const IShapeUPtr & p) {
                return p->LiesInside(other_center);
                }
            );
    }

    bool AABB::IntersectsWith(const Point & other) const {
        return other.IntersectsWith(*this);
    }

    vec2 AABB::TopLeft() const {
        return vec2(_getLeft(), _getTop());
    }

    vec2 AABB::TopRight() const {
        return vec2(_getRight(), _getTop());
    }

    vec2 AABB::BottomLeft() const {
        return vec2(_getLeft(), _getBottom());
    }

    vec2 AABB::BottomRight() const {
        return vec2(_getRight(), _getBottom());
    }

    Circle::Circle(const vec2 & center, const double R) :
    center(center), R(R) {}

    Circle::Circle(const double center_x, const double center_y, const double R) :
    Circle(vec2(center_x, center_y), R) {}

    bool Circle::LiesInside(const vec2 & dot) const {
        return center.distance_to_squared(dot) <= R*R;
    }

    bool Circle::IntersectsWith(const AABB & other) const {
        return other.IntersectsWith(*this);
    }

    bool Circle::IntersectsWith(const Circle & other) const {
        const double R_sum = R + other.R;
        const double squared_centers_distance = 
            center.distance_to_squared(other.center);

        return squared_centers_distance <= R_sum*R_sum;
    }

    bool Circle::IntersectsWith(const Point & other) const {
        return other.IntersectsWith(*this);
    }

    Point::Point(const vec2 & center) : center(center) {}

    Point::Point(const double center_x, const double center_y) :
    Point(vec2(center_x, center_y)) {}

    bool Point::LiesInside(const vec2 & dot) const {
        const bool same_x = allclose(center.x, dot.x);
        const bool same_y = allclose(center.y, dot.y);
        return same_x and same_y;
    }

    bool Point::IntersectsWith(const Point & other) const {
        return LiesInside(other.center);
    }

    bool Point::IntersectsWith(const AABB & other) const {
        return other.LiesInside(center);
    }

    bool Point::IntersectsWith(const Circle & other) const {
        return other.LiesInside(center);
    }
}