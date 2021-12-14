// _LIB_CORE_INC_SHAPES_HPP
#pragma once
#include "geometry.hpp"

// this header expands the core namespace with
// shape classes. All shapes implement IShape
// interface and can check whether the two of them are intersecting
// or a given point lies within their area
// note that Point class is just a wrapper around core::vec2
// with IShape interface
namespace core
{
    class IShape;
    class AABB;
    class Circle;
    class Point;
    struct ContactPoint;
}  // namespace core

enum shape {
    AABB_shape = 1,
    Circle_shape,
    Point_shape
};

struct core::ContactPoint {
    bool has_contact = false;
    core::vec2 localA;
    core::vec2 localB;
    core::vec2 normal;
    double penetration = 0.0;
    ContactPoint() = default;
    ContactPoint(const ContactPoint &) = default;
    ~ContactPoint() = default;
    operator bool() const { return has_contact; }
    ContactPoint inverse() {
        return ContactPoint {
            has_contact,
            localB,
            localA,
            -normal,
            penetration
        };
    }
};

// interface for all shapes
// the shape should be able to run collision check,
// tell the caller its type and check whether
// a given point lies inside its area (or, perhaps, its volume
// in 3D space, but we will stick to 2D space)
class core::IShape {
public:
    virtual ~IShape() = 0;
    virtual bool LiesInside(const vec2 &) const = 0;
    virtual const vec2 & GetCenter() const = 0;

    virtual ContactPoint IntersectsWithAABB(const AABB &) const = 0;
    virtual ContactPoint IntersectsWithCircle(const Circle &) const = 0;
    virtual ContactPoint IntersectsWith(const IShape &) const = 0;
    
    virtual shape Type() const = 0;
    virtual void shift(const vec2 &) = 0;
protected:
    virtual double _getRight() const = 0;
    virtual double _getLeft() const = 0;
    virtual double _getTop() const = 0;
    virtual double _getBottom() const = 0;
};

// AABB (axis-aligned bounding box) class,
// which is basically a rectangle
class core::AABB : public IShape {
protected:
    vec2 center;
    double width = 0.0;
    double height = 0.0;
protected:
    double _getRight() const override;
    double _getLeft() const override;
    double _getTop() const override;
    double _getBottom() const override;
public:
    AABB() = default;
    ~AABB() = default;
    AABB(const vec2 & center, const double w, const double h);
    AABB(const double center_x, const double center_y, const double w, const double h);

    shape Type() const override { return shape::AABB_shape; }
    void shift(const vec2 & delta) override { center += delta; }

    bool LiesInside(const core::vec2 &) const override;

    ContactPoint IntersectsWithAABB(const AABB &) const override;
    ContactPoint IntersectsWithCircle(const Circle &) const override;
    ContactPoint IntersectsWith(const IShape &) const override;

    const vec2 & GetCenter() const override { return center; }

    vec2 TopLeft() const;
    vec2 TopRight() const;
    vec2 BottomLeft() const;
    vec2 BottomRight() const;
};

// Circular shape class, pretty self-explanatory
class core::Circle : public IShape {
protected:
    vec2 center;
    double R = 0.0;
protected:
    double _getRight() const override { return center.x + R; }
    double _getLeft() const override { return center.x - R; }
    double _getTop() const override { return center.y + R; }
    double _getBottom() const override { return center.y - R; }
public:
    Circle() = default;
    ~Circle() = default;
    Circle(const double center_x, const double center_y, const double R);
    Circle(const vec2 & center, const double R);

    shape Type() const override { return shape::Circle_shape; }
    void shift(const vec2 & delta) override { center += delta; }

    bool LiesInside(const vec2 &) const override;

    ContactPoint IntersectsWithAABB(const AABB &) const override;
    ContactPoint IntersectsWithCircle(const Circle &) const override;
    ContactPoint IntersectsWith(const IShape &) const override;

    double GetRadius() const { return R; }
    const vec2 & GetCenter() const override { return center; }
};

// Point shape class, is merely a wrapper over vec2
// as vec2 itself is not actually a shape
class core::Point : public Circle {
public:
    Point() = default;
    ~Point() = default;
    Point(const vec2 & center);
    Point(const double center_x, const double center_y);

    shape Type() const { return shape::Point_shape; }

    bool LiesInside(const vec2 &) const;

    ContactPoint IntersectsWithAABB(const AABB &) const;
    ContactPoint IntersectsWithCircle(const Circle &) const;
    ContactPoint IntersectsWith(const IShape &) const;
};

// _LIB_CORE_INC_SHAPES_HPP
