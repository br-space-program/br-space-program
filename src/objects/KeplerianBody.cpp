#include "KeplerianBody.hpp"
#include <cgp/cgp.hpp>
#include "../constants.hpp"
#include "../mesh/primitives.hpp"
#include "../utils/uvector.hpp"

using cgp::mat3;
using cgp::mesh_drawable;
using cgp::vec3;

KeplerianBody::KeplerianBody(CelestialBody& _anchor,
                             vec3 _position,
                             double _mass,
                             double _radius)
    : CelestialBody(_position, _mass, _radius) {
  set_anchor(_anchor, uz);
}

void KeplerianBody::update() {
  double dt = 1.0 / 60.0;
  double a = cgp::norm(u0);
  double dTheta =
      std::sqrt(GRAVITATIONAL_CONSTANT * anchor->get_mass() / std::pow(a, 3)) *
      dt;
  theta += dTheta;

  mat3 R = mat3::build_rotation_from_axis_angle(normal, theta);

  position = R * u0 + anchor->get_position();
}

void KeplerianBody::set_anchor(CelestialBody& _anchor, vec3 _normal) {
  anchor = &_anchor;
  normal = _normal;
  // https://fr.wikipedia.org/wiki/Projection_orthogonale#Projection_orthogonale_sur_une_droite_vectorielle
  vec3 x = position - anchor->get_position();
  u0 = x - (cgp::dot(x, normal) / cgp::dot(normal, normal)) * normal;
  theta = 0;
}

CelestialBody* KeplerianBody::get_anchor() const {
  return anchor;
}

vec3 KeplerianBody::get_normal() const {
  return normal;
}
