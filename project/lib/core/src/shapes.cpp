#include <memory>
#include <array>
#include <algorithm>

#include "shapes.hpp"

namespace core {
    using IShapeUPtr = std::unique_ptr<IShape>;

    core::IShape::~IShape() {}

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

    ContactPoint AABB::IntersectsWithAABB(const AABB & other) const {
        if (&other == this) return ContactPoint();

        // first check if the boxes are actually intersecting
        // the expressions are unintuitive, however
        // the idea is simple: boxes DEFINETELY DO NOT overlap
        // (it is actually easier to check)
        // if one of them has, e.g., left border
        // with greater value then the right border of the other
        const bool isLefter = _getLeft() >= other._getRight();
        const bool isRighter = _getRight() <= other._getLeft();
        const bool isAbove = _getBottom() >= other._getTop();
        const bool isBelow = _getTop() <= other._getBottom();

        // if at least one was true, there is no collision
        if (isLefter or isRighter or isAbove or isBelow)
            return ContactPoint();

        // it is what it is
        const vec2 this_halfdims(width / 2, height / 2);
        const vec2 other_halfdims(other.width / 2, other.height / 2);

        // uncanny flex with lambdas
        double penetration;  // pen distance
        const vec2 normal = [&]() {
            // as we are checking AABBs,
            // the intersection normal must be
            // aligned along axis, what a surprize!
            const std::array<double, 4> distances = {
                other.TopRight().x - BottomLeft().x,
                TopRight().x - other.BottomLeft().x,
                other.TopRight().y - BottomLeft().y,
                TopRight().y - other.BottomLeft().y,
            };

            const std::array<vec2, 4> axis = {
                vec2(-1.0, 0.0), vec2(1.0, 0.0),
                vec2(0.0, -1.0), vec2(0.0, 1.0)
            };

            // we are looking for the least distance
            const auto min_iter = std::min_element(
                distances.begin(),
                distances.end()
            );

            const auto min_idx = std::distance(
                distances.begin(),
                min_iter
            );

            penetration = *min_iter;
            return axis[min_idx];
        }();

        return ContactPoint {
            true,
            vec2(),  // the local vectors are empty
            vec2(),
            normal,
            penetration
        };
    }

    ContactPoint AABB::IntersectsWithCircle(const Circle & other) const {
        const auto R = other.GetRadius();
        const vec2 & other_center = other.GetCenter();
        const vec2 half_dims(width / 2, height / 2);

        const vec2 delta = other_center - center;
        const vec2 closest_point_on_box = clamp_vector(delta, -half_dims, half_dims);

        const vec2 local_point = delta - closest_point_on_box;
        const auto distance = local_point.len();

        if (distance >= R) return ContactPoint();

        const vec2 normal = local_point.normalized();
        const double penetration = R - distance;
        const vec2 localA;
        const vec2 localB = normal * (-R);

        return ContactPoint {
            true,
            localA,
            localB,
            normal,
            penetration
        };
    }

    ContactPoint AABB::IntersectsWith(const IShape & other) const {
        return other.IntersectsWithAABB(*this).inverse();
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

    ContactPoint Circle::IntersectsWithAABB(const AABB & other) const {
        return other.IntersectsWithCircle(*this).inverse();
    }

    ContactPoint Circle::IntersectsWithCircle(const Circle & other) const {
        if (&other == this) return ContactPoint();
        const double radii = R + other.R;

        const auto delta = other.center - center;
        const auto delta_len = delta.len();

        const bool intersect(delta_len <= radii);
        if (not intersect) return ContactPoint();
        
        auto normal = delta.normalized();
        auto localA = normal * R;
        auto localB = normal * (-other.R);
        double penetration = radii - delta_len;

        return ContactPoint {
            true,
            localA,
            localB,
            normal,
            penetration
        };
    }

    ContactPoint Circle::IntersectsWith(const IShape & other) const {
        return other.IntersectsWithCircle(*this).inverse();
    }

    Point::Point(const vec2 & center) : Circle(center, 0.0) {}

    Point::Point(const double center_x, const double center_y) :
    Point(vec2(center_x, center_y)) {}

    bool Point::LiesInside(const vec2 & dot) const {
        const bool same_x = allclose(center.x, dot.x);
        const bool same_y = allclose(center.y, dot.y);
        return same_x and same_y;
    }

    ContactPoint Point::IntersectsWithAABB(const AABB & other) const {
        return other.IntersectsWithCircle(*this).inverse();
    }

    ContactPoint Point::IntersectsWithCircle(const Circle & other) const {
        return other.IntersectsWithCircle(*this).inverse();
    }

    ContactPoint Point::IntersectsWith(const IShape & other) const {
        return other.IntersectsWithCircle(*this).inverse();
    }
}