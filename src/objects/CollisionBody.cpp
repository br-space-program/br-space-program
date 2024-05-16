#include "CollisionBody.hpp"
#include "../scene.hpp"

CollisionBody::CollisionBody(scene_structure* _scene,
                             vec3 _position,
                             double _radius) {
  scene = _scene;
  position = _position;
  radius = _radius;

  debug_sphere.initialize_data_on_gpu(cgp::mesh_primitive_sphere(radius));
  debug_sphere.model.translation = position;
  debug_sphere.material.color = {1, 0, 0};
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

void CollisionBody::collision_render_debug() {
  debug_sphere.model.translation = position;
  std::cout << "Rendering debug sphere at " << position << std::endl;
  draw(debug_sphere, scene->environment);
}