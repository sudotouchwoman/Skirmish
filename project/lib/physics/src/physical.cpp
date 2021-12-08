#include "physical.hpp"

namespace physical {
    State::State(const core::vec2 & velocity, const core::vec2 & acceleration, const double inverse_mass) :
        velocity(velocity), acceleration(acceleration), inverse_mass(inverse_mass) {}

    State::State(const double inverse_mass) : inverse_mass(inverse_mass) {}

    PhysicalObject::PhysicalObject(const double inverse_mass) :
        state(State(inverse_mass)) {}

    PhysicalObject::PhysicalObject(const State & state, IShapeUPtr geometry) :
        state(state), geometry(std::move(geometry)) {}

    void PhysicalObject::update(const double dt) {
        state.velocity += state.acceleration * dt;
        geometry->shift(state.velocity * dt);
    }

    void PhysicalObject::setGeometry(IShapeUPtr new_geometry) {
        geometry = std::move(new_geometry);
    }

    void PhysicalObject::setState(const State & new_state) {
        state = new_state;
    }

    core::IShape & PhysicalObject::getGeometry() {
        if (not geometry) throw -1;  // just to make sure the caller will not casue segfault
        return *geometry.get();
        }


    bool PhysicalObject::collidesWith(const PhysicalObject & other) const {
        if (not geometry) return false;
        if (not other.geometry) return false;

        return geometry->IntersectsWith(*other.geometry.get());
    }

    const core::ContactPoint PhysicalObject::collide(PhysicalObject & a, PhysicalObject & b) {
        // the objects do not have valid geometry, i.e. they cannot collide anyways
        if (not a.geometry or not b.geometry) return core::ContactPoint();
        // obtain collision data
        const core::ContactPoint cp = a.geometry->IntersectsWith(b.getGeometry());
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
        const double total_inverse_mass = a.state.inverse_mass + b.state.inverse_mass;
        if (core::allclose(total_inverse_mass, 0.0)) return cp;

        // move the objects with respect to their masses
        // if this boi is damn thicc af (its inverse mass is 0)
        // the shift will not change its position
        a.geometry->shift(-cp.normal * cp.penetration * (a.state.inverse_mass / total_inverse_mass));
        b.geometry->shift(cp.normal * cp.penetration * (b.state.inverse_mass / total_inverse_mass));

        // the second trick is about rotation of the velocity vectors
        const double a_theta_r = cp.normal.angle_r() - a.state.velocity.angle_r();
        a.state.velocity.rotate_r(2*a_theta_r);
        a.state.velocity.inverse();

        const double b_theta_r = cp.normal.angle_r() - b.state.velocity.angle_r();
        b.state.velocity.rotate_r(2*b_theta_r);
        b.state.velocity.inverse();

        return cp;
    }
}  // namespace physical
