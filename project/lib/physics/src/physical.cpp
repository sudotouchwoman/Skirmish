#include "physical.hpp"

namespace physical {

State::State(const core::vec2 & velocity, const core::vec2 & acceleration, const double inverse_mass) :
    velocity(velocity),
    acceleration(acceleration),
    inverse_mass(inverse_mass) {}

State::State(const double inverse_mass) :
    inverse_mass(inverse_mass) {}

PhysicalObject::PhysicalObject(const double inverse_mass) :
    state_(State(inverse_mass)) {}

PhysicalObject::PhysicalObject(const State & state, IShapeUPtr geometry) :
    state_(state),
    geometry_(std::move(geometry)) {}

void PhysicalObject::update(const double dt) {
    state_.velocity += state_.acceleration * dt;
    geometry_->shift(state_.velocity * dt);
}

void PhysicalObject::setGeometry(IShapeUPtr new_geometry) {
    geometry_ = std::move(new_geometry);
}

void PhysicalObject::setState(const State & state) {
    state_ = state;
}

core::IShape & PhysicalObject::getGeometry() {
    if (not geometry_) throw -1;  // just to make sure the caller will not casue segfault
    return *geometry_;
    }

// collision check, but checking the pointers
// is nessesary!
bool PhysicalObject::collidesWith(const PhysicalObject & other) const {
    if (not geometry_) return false;
    if (not other.geometry_) return false;

    return geometry_->IntersectsWith(*other.geometry_);
}

const core::ContactPoint PhysicalObject::collide(PhysicalObject & a, PhysicalObject & b) {
    // the objects do not have valid geometry_, i.e. they cannot collide anyways
    if (not a.geometry_ or not b.geometry_) return core::ContactPoint();
    // obtain collision data
    const core::ContactPoint cp = a.geometry_->IntersectsWith(b.getGeometry());
    // it may happen that the normal was not found correctly
    // in this case, we cannot resolve the collision and should
    // just propagate the collision data
    // the game itself will possibly handle such situation, e.g.
    // if projectile has hit the wall and got inside it
    // the problem is that sometimes we may like *not* to remove this
    // object
    // our simple physics engine will be unable of handling such situations
    // as long as the collision detection is discrete
    // ...making it continuous will be really complicated, however
    // this method may only be needed for projectiles
    // maybe I should implement the ray IShape...
    if (not cp or cp.normal.len_squared() == 0.0) return cp;

    // resolve the collision using the mass info
    // note: if the total inverse mass is zero,
    // both colliding objects are infinetely heavy!
    // one should be warned that in this case the collision will appear constantly
    // as it cannot be resolved using this simple method
    // (to be honest, I do not know how to behave in such sutuation)
    const double total_inverse_mass = a.state_.inverse_mass + b.state_.inverse_mass;
    if (core::allclose(total_inverse_mass, 0.0)) return cp;

    // move the objects with respect to their masses
    // if this boi is damn thicc af (its inverse mass is 0)
    // the shift will not change its position
    a.geometry_->shift(-cp.normal * cp.penetration * (a.state_.inverse_mass / total_inverse_mass));
    b.geometry_->shift(cp.normal * cp.penetration * (b.state_.inverse_mass / total_inverse_mass));

    // the second trick is about rotation of the velocity vectors
    const double a_theta_r = cp.normal.angle_r() - a.state_.velocity.angle_r();
    a.state_.velocity.rotate_r(2*a_theta_r);
    a.state_.velocity.inverse();

    const double b_theta_r = cp.normal.angle_r() - b.state_.velocity.angle_r();
    b.state_.velocity.rotate_r(2*b_theta_r);
    b.state_.velocity.inverse();

    return cp;
}

}  // namespace physical
