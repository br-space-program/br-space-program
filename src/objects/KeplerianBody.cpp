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
  position.z = anchor->get_position().z;
}

void KeplerianBody::update() {
  double dt = 1.0 / 60.0;
  double G = 100;
  double a = cgp::norm(u0);
  double dTheta = std::sqrt(G * anchor->get_mass() / std::pow(a, 3)) * dt;
  theta += dTheta;

  mat3 R = mat3::build_rotation_from_axis_angle({0, 0, 1}, theta);

  position = R * u0 + anchor->get_position();
}

void KeplerianBody::set_anchor(CelestialBody& _anchor) {
  anchor = &_anchor;
  u0 = position - anchor->get_position();
  theta = 0;
}

CelestialBody* KeplerianBody::get_anchor() const {
  return anchor;
}
