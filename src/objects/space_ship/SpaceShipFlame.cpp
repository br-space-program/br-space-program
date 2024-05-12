#include "SpaceShipFlame.hpp"
#include <cgp/cgp.hpp>

using cgp::mesh_drawable;

SpaceShipFlame::SpaceShipFlame(scene_structure* _scene) {
  scene = _scene;

  // Load the SpaceShipFlame
  mesh sphere_mesh = mesh_primitive_ellipsoid({0.1f, 0.21f, 0.1f});

  flame.initialize_data_on_gpu(sphere_mesh);

  flame.model.scaling = 0.4f;  // coordinates are multiplied by 0.2 in the
                               // shader flame.model.translation = {
  //   0, 0, 2};  // coordinates are offseted by {1,2,0} in the shader

  // Make sphere horizontal
  // flame.model.rotation =
  //   rotation_transform::from_axis_angle({1, 0, 0}, -3.14f / 2);

  flame.material.color = {1, 0.73, 0.08};

  // Add shader
  flame.shader = scene->shader_glow;
}
