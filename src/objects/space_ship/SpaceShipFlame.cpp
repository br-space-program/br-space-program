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
  flame.initialize_data_on_gpu(
      mesh_load_file_obj(project::path + "assets/flame.obj"));

  flame.model.scaling = 0.02 * scale;
  flame.material.color = {0.96, 0.05, 0.04};  // {1, 0.73, 0.08};

  // Make it a light source
  flame.material.phong.ambient = 1;
  flame.material.phong.diffuse = 0;
  flame.material.phong.specular = 0;

  // Add shader
  flame.shader = scene->shader_custom;

  hierarchy->add(flame, name, "ship_center", position);
  (*hierarchy)[name].transform_local.rotation =
      rotation_transform::from_axis_angle(rotation_axis, rotation_angle);

  // ==== Flame small ====
  flame_small.initialize_data_on_gpu(
      mesh_load_file_obj(project::path + "assets/flame.obj"));

  flame_small.model.scaling = 0.015 * scale;
  flame_small.material.color = {1, 0.73, 0.08};

  // Make it a light source
  flame_small.material.phong.ambient = 1;
  flame_small.material.phong.diffuse = 0;
  flame_small.material.phong.specular = 0;

  // Add shader
  flame_small.shader = scene->shader_custom;

  hierarchy->add(flame_small, name + "_small", name, {0, -0.04, 0});
  (*hierarchy)[name + "_small"].transform_local.rotation =
      rotation_transform::from_axis_angle({0, 1, 0}, 3.14f / 2);

  // ==== Flare ====
  flare.initialize_data_on_gpu(mesh_primitive_ellipsoid({0.1f, 0.25f, 0.1f}));

  flare.model.scaling = 1.1 * scale;
  flare.material.color = {1, 0.73, 0.08};

  // Make it a light source
  flare.material.phong.ambient = 1;
  flare.material.phong.diffuse = 0;
  flare.material.phong.specular = 0;

  // Add shader
  flare.shader = scene->shader_glow;

  hierarchy->add(flare, name + "_flare", name);

  off();
}

void SpaceShipFlame::on() {
  (*hierarchy)[name].drawable = flame;
  (*hierarchy)[name + "_flare"].drawable = flare;
  (*hierarchy)[name + "_small"].drawable = flame_small;
}

void SpaceShipFlame::off() {
  (*hierarchy)[name].drawable = mesh_drawable();
  (*hierarchy)[name + "_flare"].drawable = mesh_drawable();
  (*hierarchy)[name + "_small"].drawable = mesh_drawable();
}

void SpaceShipFlame::update() {
  // Update the position of the flame : animation

  // (*hierarchy)[name].transform_local.translation = {0, offset, 0};
  (*hierarchy)[name].drawable.model.scaling_xyz = {
      1 + 0.2 * flame_offset(scene->timer.t),
      1 + 0.7 * flame_offset(scene->timer.t + 11), 1};

  (*hierarchy)[name + "_small"].drawable.model.scaling_xyz = {
      1 + 0.1 * flame_offset(scene->timer.t + 42),
      1 + 0.5 * flame_offset(scene->timer.t + 64), 1};
}

double flame_offset(double t) {
  return 0.2 * (1 + std::sin(34 * t)) + 0.17 * (1 + std::sin(42 * t)) +
         0.05 * (1 + std::sin(47 * t)) + 0.04 * (1 + std::sin(52 * t));
}