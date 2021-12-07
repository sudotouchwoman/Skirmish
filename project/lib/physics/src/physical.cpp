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
        state.acceleration += state.acceleration * dt;
        state.velocity += state.velocity * dt;
        geometry->shift(state.velocity * dt);
    }

    void PhysicalObject::setGeometry(IShapeUPtr new_geometry) {
        geometry = std::move(new_geometry);
    }

    void PhysicalObject::setState(const State & new_state) {
        state = new_state;
    }

    const core::ContactPoint PhysicalObject::collide(PhysicalObject & a, PhysicalObject & b) {
        const core::ContactPoint cp = a.geometry->IntersectsWith(*a.geometry.get());
        if (not cp) return cp;

        const double total_mass = a.state.inverse_mass + b.state.inverse_mass;

        a.geometry->shift(-cp.normal * cp.penetration * (a.state.inverse_mass / total_mass));
        b.geometry->shift(cp.normal * cp.penetration * (b.state.inverse_mass / total_mass));

        const double a_theta_d = cp.normal.angle_r() - a.state.velocity.angle_r();
        a.state.velocity.rotate_r(2*a_theta_d);
        a.state.velocity.inverse();

        const double b_theta_d = cp.normal.angle_r() - b.state.velocity.angle_r();
        b.state.velocity.rotate_r(2*b_theta_d);
        b.state.velocity.inverse();

        return cp;
    }
}  // namespace physical
