#include "CollisionBody.hpp"

CollisionBody::CollisionBody(vec3 _position, double _radius) {
  position = _position;
  radius = _radius;
}

void CollisionBody::set_position(vec3 _position) {
  position = _position;
}

vec3 CollisionBody::get_position() const {
  return position;
}

void CollisionBody::set_radius(double _radius) {
  radius = _radius;
}

double CollisionBody::get_radius() const {
  return radius;
}

std::tuple<bool, double> CollisionBody::is_in_collision(
    const CollisionBody& body) const {
  double distance = norm(position - body.get_position());

  return {distance < radius + body.get_radius(), distance};
}
