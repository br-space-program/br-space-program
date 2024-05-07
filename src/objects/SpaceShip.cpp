#include "SpaceShip.hpp"
#include <cgp/cgp.hpp>

using cgp::mesh_drawable;

SpaceShip::SpaceShip(scene_structure* _scene) {
  scene = _scene;

  mesh cylinder_mesh = mesh_primitive_cylinder();

  cylinder.initialize_data_on_gpu(cylinder_mesh);
  cylinder.model.scaling =
      5.0f;  // coordinates are multiplied by 0.2 in the shader
  cylinder.model.translation = {
      10, 0, 0};  // coordinates are offseted by {1,2,0} in the shader

  // Make cylinder horizontal
  cylinder.model.rotation =
      rotation_transform::from_axis_angle({1, 0, 0}, 3.14f / 2);

  cylinder.material.color = {
      0.62f, 0.27f,
      0.07f};  // sphere will appear red (r,g,b components in [0,1])

  // Add shader
  cylinder.shader = scene->shader_custom;
}

void SpaceShip::update() {
  // Update the position of the sphere
  // sphere.model.translation = {1, 2, 0};
}

void SpaceShip::render() {
  // Display the sphere
  draw(cylinder, scene->environment);
}

void SpaceShip::render_debug() {
  // Display the wireframe of the sphere
  draw_wireframe(cylinder, scene->environment);
}
