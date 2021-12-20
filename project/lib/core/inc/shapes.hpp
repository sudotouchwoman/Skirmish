// _LIB_CORE_INC_SHAPES_HPP
#pragma once
#include <memory>
#include <array>
#include <algorithm>

#include "geometry.hpp"

// this header expands the core namespace with
// shape classes. All shapes implement IShape
// interface and can check whether the two of them are intersecting
// or a given point lies within their area
// note that Point class is just a wrapper around core::vec2
// with IShape interface

enum shape {
    AABB_SHAPE,
    CIRCLE_SHAPE,
    POINT_SHAPE
};

namespace core
{

class IShape;
class AABB;
class Circle;
class Point;
struct ContactPoint;

using IShapeUPtr = std::unique_ptr<IShape>;

struct ContactPoint {
    bool has_contact = false;
    vec2 localA;
    vec2 localB;
    vec2 normal;
    double penetration = 0.0;
    operator bool() const { return has_contact; }
    ContactPoint inverse() {
        std::swap(localB, localA);
        normal.inverse();
        return *this;
    }
};

// interface for all shapes
// the shape should be able to run collision check,
// tell the caller its type and check whether
// a given point lies inside its area (or, perhaps, its volume
// in 3D space, but we will stick to 2D space)
class IShape {
public:
    virtual ~IShape() = default;
    virtual bool isInside(const vec2 &) const = 0;
    virtual const vec2 & getCenter() const = 0;
    virtual void setCenter(const vec2 &) = 0;

    virtual ContactPoint IntersectsWithAABB(const AABB &) const = 0;
    virtual ContactPoint IntersectsWithCircle(const Circle &) const = 0;
    virtual ContactPoint IntersectsWith(const IShape &) const = 0;
    
    virtual shape type() const = 0;
    virtual void shift(const vec2 &) = 0;
    virtual AABB getBoundingRect() const = 0;
protected:
    virtual double getRight() const = 0;
    virtual double getLeft() const = 0;
    virtual double getTop() const = 0;
    virtual double getBottom() const = 0;
};

// AABB (axis-aligned bounding box) class,
// which is basically a rectangle
class AABB : public IShape {
public:
    AABB() = default;
    ~AABB() = default;
    AABB(const vec2 & center, const double width, const double height);
    AABB(const double center_x, const double center_y, const double w, const double h);

    shape type() const override { return shape::AABB_SHAPE; }
    void shift(const vec2 & delta) override { center_ += delta; }

    bool isInside(const core::vec2 &) const override;

    ContactPoint IntersectsWithAABB(const AABB &) const override;
    ContactPoint IntersectsWithCircle(const Circle &) const override;
    ContactPoint IntersectsWith(const IShape &) const override;

    const vec2 & getCenter() const override { return center_; }
    void setCenter(const vec2 & new_center) { center_ = new_center; }

    AABB getBoundingRect() const override { return *this; }
    double getWidth() const { return width_; }
    double getHeight() const { return height_; }

    vec2 TopLeft() const;
    vec2 TopRight() const;
    vec2 BottomLeft() const;
    vec2 BottomRight() const;
protected:
    vec2 center_;
    double width_ = 0.0;
    double height_ = 0.0;
protected:
    double getRight() const override;
    double getLeft() const override;
    double getTop() const override;
    double getBottom() const override;
};

// Circular shape class, pretty self-explanatory
class Circle : public IShape {
public:
    Circle() = default;
    ~Circle() = default;
    Circle(const double center_x, const double center_y, const double R);
    Circle(const vec2 & center, const double R);

    shape type() const override { return shape::CIRCLE_SHAPE; }
    void shift(const vec2 & delta) override { center_ += delta; }

    bool isInside(const vec2 &) const override;

    ContactPoint IntersectsWithAABB(const AABB &) const override;
    ContactPoint IntersectsWithCircle(const Circle &) const override;
    ContactPoint IntersectsWith(const IShape &) const override;

    double GetRadius() const { return R; }
    const vec2 & getCenter() const override { return center_; }
    void setCenter(const vec2 & new_center) { center_ = new_center; }
    AABB getBoundingRect() const override;
protected:
    vec2 center_;
    double R = 0.0;
protected:
    double getRight() const override { return center_.x + R; }
    double getLeft() const override { return center_.x - R; }
    double getTop() const override { return center_.y + R; }
    double getBottom() const override { return center_.y - R; }
};

// Point shape class, is merely a wrapper over vec2
// as vec2 itself is not actually a shape
class Point : public Circle {
public:
    Point() = default;
    ~Point() = default;
    Point(const vec2 & center);
    Point(const double center_x, const double center_y);

    shape type() const { return shape::POINT_SHAPE; }

    bool isInside(const vec2 &) const;

    ContactPoint IntersectsWithAABB(const AABB &) const;
    ContactPoint IntersectsWithCircle(const Circle &) const;
    ContactPoint IntersectsWith(const IShape &) const;
};

}  // namespace core

// _LIB_CORE_INC_SHAPES_HPP
