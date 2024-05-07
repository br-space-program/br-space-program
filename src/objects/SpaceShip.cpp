#include "SpaceShip.hpp"
#include <cgp/cgp.hpp>

using cgp::mesh_drawable;

SpaceShip::SpaceShip(scene_structure* _scene) {
  scene = _scene;
  position = {10, 0, 0};
  speed = {0, 0, 0};

  scene->camera_control.look_at(
      {8, 0, 0} /* position of the camera in the 3D scene */,
      {10.5, 0, 0} /* targeted point in 3D scene */,
      {0, 0, 1} /* direction of the "up" vector */);

  mesh cylinder_mesh = mesh_primitive_cylinder();

  cylinder.initialize_data_on_gpu(cylinder_mesh);
  cylinder.model.scaling =
      1.0f;  // coordinates are multiplied by 0.2 in the shader
  cylinder.model.translation =
      position;  // coordinates are offseted by {1,2,0} in the shader

  // Make cylinder horizontal
  cylinder.model.rotation =
      rotation_transform::from_axis_angle({1, 0, 0}, 3.14f / 2) *
      rotation_transform::from_axis_angle({0, 1, 0}, 3.14f / 2);

  cylinder.material.color = {0.62f, 0.27f, 0.07f};

  // Add shader
  cylinder.shader = scene->shader_custom;
}

void SpaceShip::update() {
  // Update the position of the sphere
  double dt = 1.0 / 60.0;
  position += speed * dt;
  cylinder.model.translation = position;

  scene->camera_control.camera_model.center_of_rotation = position;
}

void SpaceShip::render() {
  // Display the sphere
  draw(cylinder, scene->environment);
}

void SpaceShip::render_debug() {
  // Display the wireframe of the sphere
  draw_wireframe(cylinder, scene->environment);
}

void SpaceShip::action_keyboard() {
  if (scene->inputs.keyboard.is_pressed('s')) {
    speed.x -= 0.1;
  }
  if (scene->inputs.keyboard.is_pressed('z')) {
    speed.x += 0.1f;
  }
  if (scene->inputs.keyboard.is_pressed('q')) {
    speed.y += 0.1;
  }
  if (scene->inputs.keyboard.is_pressed('d')) {
    speed.y -= 0.1;
  }
  if (scene->inputs.keyboard.shift) {
    speed.z += 0.1;
  }
  if (scene->inputs.keyboard.ctrl) {
    speed.z -= 0.1;
  }
}
