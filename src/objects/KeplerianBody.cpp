#include "KeplerianBody.hpp"
#include <cgp/cgp.hpp>
#include "../mesh/primitives.hpp"

using cgp::mat3;
using cgp::mesh_drawable;
using cgp::vec3;

KeplerianBody::KeplerianBody(CelestialBody& _anchor,
                             vec3 _position,
                             double _mass,
                             double _radius)
    : CelestialBody(_position, _mass, _radius) {
  set_anchor(_anchor);
  position.z = 0;
}

void KeplerianBody::update() {
  double dt = 1.0 / 60.0;
  double G = 100;
  double a = cgp::norm(position - anchor->get_position());
  double dTheta = std::sqrt(G * anchor->get_mass() / std::pow(a, 3)) * dt;

  mat3 R = mat3::build_rotation_from_axis_angle({0, 0, 1}, dTheta);

  position = R * (position - anchor->get_position()) + anchor->get_position();
}

void KeplerianBody::set_anchor(CelestialBody& _anchor) {
  anchor = &_anchor;
}

CelestialBody* KeplerianBody::get_anchor() const {
  return anchor;
}
