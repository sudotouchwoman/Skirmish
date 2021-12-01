#include "physical.hpp"

namespace physical {

    State::State(const vec2 & velocity, const vec2 & acceleration, const double mass) :
    velocity(velocity), acceleration(acceleration), mass(mass) {}

    State::State(const double mass) : mass(mass) {}

    // IPhysicalObject::IPhysicalObject(const IShapeSPtr & geometry, const State & state) :
    // geometry(geometry), state(state) {}

    // IPhysicalObject::IPhysicalObject(const State & state) :
    // state(state) {}

    // IPhysicalObject::IPhysicalObject(const IShapeSPtr & geometry) :
    // geometry(geometry) {}

    IPhysicalObject::~IPhysicalObject() {}

}  // namespace physical
