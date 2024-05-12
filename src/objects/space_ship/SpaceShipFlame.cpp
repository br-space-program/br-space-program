#include "SpaceShipFlame.hpp"
#include <cgp/cgp.hpp>

using cgp::mesh_drawable;

SpaceShipFlame::SpaceShipFlame(scene_structure* _scene,
                               cgp::hierarchy_mesh_drawable* _hierarchy,
                               std::string _name,
                               double scale,
                               vec3 position,
                               vec3 rotation_axis,
                               double rotation_angle) {
  scene = _scene;
  hierarchy = _hierarchy;
  name = _name;

  // Load the SpaceShipFlame
  // mesh sphere_mesh = mesh_primitive_ellipsoid({0.1f, 0.21f, 0.1f});

  flame.initialize_data_on_gpu(
      mesh_load_file_obj(project::path + "assets/flame.obj"));

  // flame.initialize_data_on_gpu(sphere_mesh);

  flame.model.scaling = 0.02 * scale;
  flame.material.color = {1, 0.73, 0.08};

  // Make it a light source
  flame.material.phong.ambient = 1;
  flame.material.phong.diffuse = 0;
  flame.material.phong.specular = 0;

  // Add shader
  flame.shader = scene->shader_custom;

  hierarchy->add(flame, name, "ship_center", position);
  (*hierarchy)[name].transform_local.rotation =
      rotation_transform::from_axis_angle(rotation_axis, rotation_angle);
  off();
}

void SpaceShipFlame::on() {
  (*hierarchy)[name].drawable = flame;
}

void SpaceShipFlame::off() {
  (*hierarchy)[name].drawable = mesh_drawable();
}

void SpaceShipFlame::update() {
  // Update the position of the flame
}