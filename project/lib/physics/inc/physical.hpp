// _LIB_PHYSICS_INC_PHYSICAL_HPP
#pragma once
#include <memory>
#include <functional>

#include "core.hpp"

using namespace core;

typedef std::shared_ptr<IShape> IShapeSPtr;
typedef std::weak_ptr<IShape> IShapeWPtr;

// namespace covers all physics-related
// routines like (discrete) collision checks,
// simulation stepping
namespace physical
{
    struct State;
    class IPhysicalObject;
    class MovableObject;
    class StaticObject;
    class Collision;

    typedef std::shared_ptr<IPhysicalObject> IPObjectSPtr;

    struct State {
        vec2 velocity;
        vec2 acceleration;
        double mass;
        State() = default;
        State(const vec2 & velocity, const vec2 & acceleration, const double mass);
        State(const double mass);
        ~State() = default;
    };

    // Interface for Physical objects
    // these should implement update() method
    // which takes one argument (timedelta for simulation)
    // other interface methods include move(),
    // which conceptually changes the geometry
    // and CollidesWith(), which should evaluate to true
    // if this object collides with another, passed via argument
    class IPhysicalObject {
    protected:
        virtual void move(const double x, const double y) = 0;
        virtual void move(const vec2 & delta) = 0;
    public:
        virtual ~IPhysicalObject() = 0;

        virtual void update(const double dt) = 0;
        virtual const IShapeWPtr & getGeometry() const = 0;
        virtual const State & getState() const = 0;
        virtual bool isActive() const = 0;

        virtual bool CollidesWith(const IPhysicalObject &) = 0;
        virtual void setOnCollisionCallback(std::function<void(IPhysicalObject &)> & callback) = 0;
    };

    // Static object implements IPhysicalObject
    // interface and can thus be updated, moved and
    // polled for state
    // however, the move() methods do nothing
    // as the entity itself remains static
    // I also marked geometry and state as
    // `const`, but this is arguable
    class StaticObject : public IPhysicalObject {
    private:
        const IShapeSPtr geometry;
        const State state;
        void move(const double x, const double y) override {}
        void move(const vec2 & delta) override {}
    public:
        StaticObject() = default;
        StaticObject(const IShapeSPtr & geometry, const State & state) : geometry(geometry), state(state) {}
        StaticObject(const IShapeSPtr & geometry) : geometry(geometry), state(State()) {}

        void update(const double dt) override {}
        const IShapeWPtr & getGeometry() const override;
        const State & getState() const override;
    };

    // Movable object implements IPhysicalObject
    // interface and can thus be updated, moved and
    // polled for state and underlying geometry
    class MovableObject : public IPhysicalObject {
    private:
        IShapeSPtr geometry;
        State state;
        void move(const double x, const double y) override;
        void move(const vec2 & delta) override;
    public:
        MovableObject() = default;
        MovableObject(const IShapeSPtr & geometry, const State & state) : geometry(geometry), state(state) {}
        MovableObject(const IShapeSPtr & geometry) : geometry(geometry), state(State()) {}

        void update(const double dt) override;
        const IShapeWPtr & getGeometry() const override;
        const State & getState() const override;
    };

    // collision class, wrapper around two colliding
    // objects
    class Collision {
        std::array<IPObjectSPtr, 2> colliding;
        Collision() = delete;
        Collision(const IPObjectSPtr & a, const IPObjectSPtr & b) :
            colliding { a, b } {}
        IPObjectSPtr & getFirst() { return colliding[0]; }
        IPObjectSPtr & getSecond() { return colliding[1]; }
    };

}  // namespace physical

// _LIB_PHYSICS_INC_PHYSICAL_HPP
