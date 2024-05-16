#include "CelestialBody.hpp"

CelestialBody::CelestialBody(vec3 _position, double _mass)
    : ObjectWithHitbox(20) {
  position = _position;
  mass = _mass;
}

void CelestialBody::set_position(vec3 _position) {
  position = _position;
}

vec3 CelestialBody::get_position() const {
  return position;
}

void CelestialBody::set_mass(double _mass) {
  mass = _mass;
}

double CelestialBody::get_mass() const {
  return mass;
}
