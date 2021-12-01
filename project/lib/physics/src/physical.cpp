#include "physical.hpp"

namespace physical {
    State::State(const core::vec2 & velocity, const core::vec2 & acceleration, const double mass) :
    velocity(velocity), acceleration(acceleration), mass(mass) {}

    State::State(const double mass) : mass(mass) {}

    // get const reference to the current geometry
    const IShapeWPtr FixedObject::getGeometry() const { return geometry; }

    // get const reference to the current state
    State & FixedObject::getState() { return state; }

    MovableObject::MovableObject(const IShapeSPtr & geometry, const State & state) :
        geometry(geometry), geometry_shadow_copy(geometry), state(state) {}

    MovableObject::MovableObject(const IShapeSPtr & geometry) :
        geometry(geometry), geometry_shadow_copy(geometry), state(State()) {}

    // utilize move semantics to transfer ownewship over
    // geometry from one object to another
    MovableObject & MovableObject::operator=(MovableObject && other) {
        geometry_shadow_copy = std::move(other.geometry_shadow_copy);
        geometry = std::move(other.geometry);
        state = other.state;
        return *this;
    }    

    // the problem here is the interactivity with the other objects
    // in such cases, the acceleration should change
    // e.g., according to the Newtons's second law
    void MovableObject::update(const double dt) {
        geometry->shift(state.velocity * dt);
        state.velocity += state.acceleration * dt;
    }

    // solve some equations to get the new acceleration
    // values
    void MovableObject::interact(IPhysicalObject & another) {
        
        // const auto other_mass = another.getState().mass;
        // const auto this_mass = state.mass;
    }

    const IShapeWPtr MovableObject::getGeometry() const { return geometry; }

    // get const reference to the current state
    State & MovableObject::getState() { return state; }

    IPhysicalObject::~IPhysicalObject() {}

}  // namespace physical
