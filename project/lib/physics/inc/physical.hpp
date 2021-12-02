// _LIB_PHYSICS_INC_PHYSICAL_HPP
#pragma once
#include <memory>
#include <functional>

#include "core.hpp"

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

    // using IShapeUPtr = std::unique_ptr<core::IShape>;

    // container for all properties
    // of a physical object, such as
    // its kinematical state (velocity,
    // acceleration) and mass
    // the geometry is stored separately
    struct State {
        core::vec2 velocity;
        core::vec2 acceleration;
        double mass = 0.0;
        State() = default;
        State(const core::vec2 & velocity, const core::vec2 & acceleration, const double mass);
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
    public:
        virtual ~IPhysicalObject() = 0;

        // update the object properties with dt time step
        // according to the current acceleration and velocity values
        virtual void update(const double dt) = 0;
        // apply force which appears due to collision
        virtual void interact(IPhysicalObject & another) = 0;
        // save the new geometry state
        virtual void commit() = 0;
        // rollback to the previous geometry state
        virtual void rollback() = 0;

        // getters and setters for geometry and state
        virtual void setGeometry(const core::IShape & new_geom) = 0;
        virtual void setState(const State & new_state) = 0;
        virtual core::IShape & getGeometry() = 0;
        virtual State & getState() = 0;
        // tell whether two objects are colliding
        // in the current timestep
        // resolve the collision in some way...
        // the process has to depend on the underlying geometry, thus
        // the resolution would make use of IShape's overloaded methods
        virtual bool ResolveCollision(const IPhysicalObject &) = 0;
        virtual bool CollidesWith(const IPhysicalObject &) = 0;
        // ???
        virtual void onCollision() = 0;

        // conceptually, tell whether the current object is
        // interacting in the simulation
        virtual bool isMovable() const = 0;
        virtual bool isInteractive() const = 0;
        // set the activity flag (if object
        // is inactive, it is not taken into account during
        // collision detection)
        virtual void setInteractivity(const bool active) = 0;
    };

    // Static object implements IPhysicalObject
    // interface and can thus be updated, moved and
    // polled for state
    // however, the move() methods do nothing
    // as the entity itself remains static
    // I also marked geometry and state as
    // `const`, but this is arguable
    // class StaticObject : public IPhysicalObject {
    // protected:
    //     // const because fixed
    //     // the state can only have non-zero mass,
    //     // the kinematics are zerofilled and constant
    //     IShapeUPtr geometry;
    //     State state;
    //     bool is_interactive = false;
    // public:
    //     // imply that one should provide geometry to
    //     // create a valid physical object
    //     StaticObject() = default;
    //     StaticObject(const core::IShape & geometry) :
    //         geometry(std::make_unique<core::IShape>(geometry)),
    //         state(State()) {}
    //     StaticObject(
    //         const core::IShape & geometry,
    //         const double mass,
    //         const bool active) :
    //         geometry(std::make_unique<core::IShape>(geometry)),
    //         state(State(mass)),
    //         is_interactive(active) {}

    //     // do nothing because fixed
    //     // apply force, however, should
    //     // take another object into account
    //     void update(const double dt) override {}
    //     void interact(IPhysicalObject & another) override;
    //     void commit() override {}
    //     void rollback() override {}

    //     core::IShape & getGeometry() override;
    //     State & getState() override;
    //     // do nothing because fixed
    //     void setGeometry(const core::IShape & new_geom) override {}
    //     void setState(const State & new_state) override {}

    //     bool isMovable() const override { return false; }
    //     bool isInteractive() const override { return is_interactive; }
    //     void setInteractivity(const bool active) override { is_interactive = active; }
    // };

    // Movable object implements IPhysicalObject
    // interface and can thus be updated, moved and
    // polled for state and underlying geometry
    // class MovableObject : public IPhysicalObject {
    // protected:
    //     IShapeUPtr geometry;
    //     // store geometry copy to have somewhere to rollback
    //     IShapeUPtr geometry_shadow_copy;
    //     State state;
    //     bool is_interactive = true;
    // public:
    //     // imply that one should provide geometry to
    //     // create a valid physical object
    //     MovableObject() = default;
    //     MovableObject(const core::IShape & geometry, const State & state);
    //     MovableObject(const core::IShape & geometry);
    //     MovableObject & operator=(MovableObject &&);

    //     void update(const double dt) override;
    //     void interact(IPhysicalObject & another) override;
    //     void commit() override;
    //     void rollback() override;

    //     core::IShape & getGeometry() override;
    //     State & getState() override;
    //     void setGeometry(const core::IShape & new_geom) override;
    //     void setState(const State & new_state) override;

    //     bool isMovable() const override { return true; }
    //     bool isInteractive() const override { return is_interactive; }
    //     void setInteractivity(const bool active) override { is_interactive = active; }
    // };

}  // namespace physical

// _LIB_PHYSICS_INC_PHYSICAL_HPP
