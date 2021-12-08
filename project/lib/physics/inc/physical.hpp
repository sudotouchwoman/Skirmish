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
    class PhysicalObject;
    
    using IShapeUPtr = std::unique_ptr<core::IShape>;

    // container for all properties
    // of a physical object, such as
    // its kinematical state (velocity,
    // acceleration) and mass
    // the geometry is stored separately
    struct State {
        core::vec2 velocity;
        core::vec2 acceleration;
        double inverse_mass = 0.0;
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
    class PhysicalObject {
    private:
        bool interactive = false;
        State state;
        IShapeUPtr geometry;
    public:
        PhysicalObject() = default;
        PhysicalObject(const double inverse_mass);
        PhysicalObject(const State &, IShapeUPtr);
        ~PhysicalObject() = default;

        // update the object properties with dt time step
        // according to the current acceleration and velocity values
        void update(const double dt);
        // getters and setters for geometry and state
        // setters
        void setGeometry(IShapeUPtr geometry);
        void setState(const State & new_state);
        // getters
        core::IShape * const getGeometry() { return geometry.get(); }
        State & getState() { return state; }

        // conceptually, tell whether the current object is
        // interacting in the simulation /
        // set the activity flag (if object
        // is inactive, it is not taken into account during
        // collision detection)
        bool isInteractive() const { return interactive; }
        void setInteractivity(const bool new_interactive) { interactive = new_interactive; }
        bool hasGeometry() const { return static_cast<bool>(geometry); }

        bool collidesWith(const PhysicalObject & other) const;
        static const core::ContactPoint collide(PhysicalObject & a, PhysicalObject & b);
    };
}  // namespace physical

// _LIB_PHYSICS_INC_PHYSICAL_HPP
