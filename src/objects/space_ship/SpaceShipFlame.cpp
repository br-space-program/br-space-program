#include "SpaceShipFlame.hpp"
#include <cgp/cgp.hpp>

using cgp::mesh_drawable;

SpaceShipFlame::SpaceShipFlame(scene_structure* _scene) {
  scene = _scene;

  // Load the SpaceShipFlame
  mesh sphere_mesh = mesh_primitive_sphere();

  sphere.initialize_data_on_gpu(sphere_mesh);

  sphere.model.scaling =
      0.1f;  // coordinates are multiplied by 0.2 in the shader
  sphere.model.translation = {
      0, 1, 0};  // coordinates are offseted by {1,2,0} in the shader

  // Make sphere horizontal
  sphere.model.rotation =
      rotation_transform::from_axis_angle({1, 0, 0}, -3.14f / 2);

  // Add shader
  sphere.shader = scene->shader_custom;
}

void SpaceShipFlame::update() {}

void SpaceShipFlame::render() {
  // Display the sphere
  draw(sphere, scene->environment);
}

void SpaceShipFlame::render_debug() {
  // Display the wireframe of the sphere
  draw_wireframe(sphere, scene->environment);
}
