#include "ObjectWithHitbox.hpp"
#include "../scene.hpp"

ObjectWithHitbox::ObjectWithHitbox(double _radius) {
  // scene = _scene;
  radius = _radius;

  debug_sphere.initialize_data_on_gpu(cgp::mesh_primitive_sphere(radius));
  debug_sphere.model.translation = position;
  debug_sphere.material.color = {1, 0, 0};
}

void ObjectWithHitbox::set_radius(double _radius) {
  radius = _radius;
}

double ObjectWithHitbox::get_radius() const {
  return radius;
}

std::tuple<bool, double> ObjectWithHitbox::is_in_collision(
    const ObjectWithHitbox& body) const {
  double distance = norm(position - body.get_position());

  return {distance < radius + body.get_radius(), distance};
}

void ObjectWithHitbox::collision_render_debug() {
  debug_sphere.model.translation = position;
  // std::cout << "Position: " << position << std::endl;

  // draw(debug_sphere, scene->environment);
}
