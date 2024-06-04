#include "FreePlanet.hpp"
#include <cgp/cgp.hpp>
#include "../constants.hpp"
#include "../mesh/planet.hpp"

FreePlanet::FreePlanet(scene_structure* _scene,
                       World* _world,
                       vec3 _position,
                       double radius,
                       vec3 color)
    : NewtonianBody(_scene,
                    _world,
                    _position,
                    PLANET_DENSITY * pow(radius, 3),
                    radius),
      scene(_scene) {
  int Nu = 100;
  int Nv = 70;
  cgp::mesh planet_mesh = create_planet_mesh(radius, {0, 0, 0}, Nu, Nv);

  sphere.initialize_data_on_gpu(planet_mesh);
  sphere.model.scaling = 1.0f;
  sphere.material.color = color;

  sphere.vbo_position.update(planet_mesh.position);
  sphere.vbo_normal.update(planet_mesh.normal);
  sphere.vbo_color.update(planet_mesh.color);
}

void FreePlanet::update() {
  next_position = position;
  NewtonianBody::update();
  std::swap(next_position, position);
}

void FreePlanet::render() {
  position = next_position;
  sphere.model.translation = position;
  draw(sphere, scene->environment);
}

void FreePlanet::render_debug() {
  draw_wireframe(sphere, scene->environment);
}
