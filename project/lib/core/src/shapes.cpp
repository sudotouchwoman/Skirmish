#include "shapes.hpp"

namespace core {
double AABB::getLeft() const {
    return center_.x - width_ / 2;
}

double AABB::getRight() const {
    return center_.x + width_ / 2;
}

double AABB::getTop() const {
    return center_.y + height_ / 2;
}

double AABB::getBottom() const {
    return center_.y - height_ / 2;
}

AABB::AABB(const vec2 & center, const double width, const double height) :
    center_(center),
    width_(width),
    height_(height) {}

AABB::AABB(
    const double center_x,
    double center_y,
    const double width,
    const double height) :
    AABB(
        vec2(center_x, center_y),
        width,
        height) {}

bool AABB::isInside(const vec2 & dot) const {
    const bool a = (getRight() >= dot.x);
    const bool b = (getLeft() <= dot.x);
    const bool c = (getTop() >= dot.y);
    const bool d = (getBottom() <= dot.y);

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
    const bool isLefter = getLeft() >= other.getRight();
    const bool isRighter = getRight() <= other.getLeft();
    const bool isAbove = getBottom() >= other.getTop();
    const bool isBelow = getTop() <= other.getBottom();

    // if at least one was true, there is no collision
    if (isLefter or isRighter or isAbove or isBelow)
        return ContactPoint();

    // it is what it is
    const vec2 this_halfdims(width_ / 2, height_ / 2);
    const vec2 other_halfdims(other.width_ / 2, other.height_ / 2);

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
    const double R = other.GetRadius();
    const vec2 half_dims(width_ / 2, height_ / 2);

    // circle's origin position relative to the AABB's origin
    const vec2 delta = other.getCenter() - center_;
    // clamped point: the point on AABB which is closest to the circle's origin
    const vec2 closest_point_on_box = vec2::clamp(delta, -half_dims, half_dims);
    // the direction of normal, i.e. inverse radius direction
    // to the closest point on box
    const vec2 normal_direction = delta - closest_point_on_box;
    // distance between closest point on AABB and the circle's origin
    // if it is greater than R, the collision is impossible!
    const double distance = normal_direction.len();

    if (distance >= R) return ContactPoint();

    // if we got that far, we must be colliding!
    const vec2 normal = normal_direction.normalized();
    // penetration is pretty self-explanatory,
    // if R and distance are aknowledged
    const double penetration = R - distance;
    // local collision point for AABB will always match its position,
    // i.e. the relative position is just zeros
    const vec2 localA;
    // local collision point for circle
    // is the point on circle in the inverse (!) direction of normal
    // as the normal is directed from AABB to circle
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
    return vec2(getLeft(), getTop());
}

vec2 AABB::TopRight() const {
    return vec2(getRight(), getTop());
}

vec2 AABB::BottomLeft() const {
    return vec2(getLeft(), getBottom());
}

vec2 AABB::BottomRight() const {
    return vec2(getRight(), getBottom());
}

AABB Circle::getBoundingRect() const {
    return AABB(center_, getTop() - getBottom(), getRight() - getLeft());
}

Circle::Circle(const vec2 & center, const double R) :
    center_(center),
    R(R) {}

Circle::Circle(const double center_x, const double center_y, const double R) :
    Circle(vec2(center_x, center_y), R) {}

bool Circle::isInside(const vec2 & dot) const {
    return center_.distance_to_squared(dot) <= R*R;
}

ContactPoint Circle::IntersectsWithAABB(const AABB & other) const {
    return other.IntersectsWithCircle(*this).inverse();
}

ContactPoint Circle::IntersectsWithCircle(const Circle & other) const {
    // circle to circle collision detection is quite straightforward,
    // just check whether the sum of radii is greater
    // than distance between circle origins
    if (&other == this) return ContactPoint();
    const double radii = R + other.R;

    const vec2 delta = other.center_ - center_;
    const double delta_len = delta.len();

    const bool intersect(delta_len <= radii);
    if (not intersect) return ContactPoint();
    
    vec2 normal = delta.normalized();
    vec2 localA = normal * R;
    vec2 localB = normal * (-other.R);
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

Point::Point(const vec2 & center):
    Circle(center, 0.0) {}

Point::Point(const double center_x, const double center_y) :
    Point(vec2(center_x, center_y)) {}

bool Point::isInside(const vec2 & dot) const {
    const bool same_x = allclose(center_.x, dot.x);
    const bool same_y = allclose(center_.y, dot.y);
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