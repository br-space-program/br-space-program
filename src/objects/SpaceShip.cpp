#include "SpaceShip.hpp"
#include <cgp/cgp.hpp>

using cgp::mesh_drawable;

// Difference between the position of the center of rotation of camera and the
// spaceship
vec3 TARGET_DIFFERENCE = {0, 0.2, 0};
double SPEED = 0.1;

SpaceShip::SpaceShip(scene_structure* _scene) {
  scene = _scene;
  position = {10, 0, 0};
  speed = {0, 0, 0};
  rotation_z = 0;
  speed_rotation_z = 0;

  scene->camera_control.look_at(
      position +
          vec3({0, -1, 0.4}) /* position of the camera in the 3D scene */,
      position + TARGET_DIFFERENCE /* targeted point in 3D scene */,
      {0, 0, 1} /* direction of the "up" vector */);

  // Load the spaceship
  cylinder.initialize_data_on_gpu(
      mesh_load_file_obj(project::path + "assets/spaceship.obj"));

  cylinder.texture.load_and_initialize_texture_2d_on_gpu(
      project::path + "assets/spaceship.jpg", GL_CLAMP_TO_BORDER,
      GL_CLAMP_TO_BORDER);

  cylinder.model.scaling =
      0.001f;  // coordinates are multiplied by 0.2 in the shader
  cylinder.model.translation =
      position;  // coordinates are offseted by {1,2,0} in the shader

  // Make cylinder horizontal
  cylinder.model.rotation =
      rotation_transform::from_axis_angle({1, 0, 0}, -3.14f / 2);

  // Add shader
  cylinder.shader = scene->shader_custom;
}

void SpaceShip::update() {
  // Update the position of the sphere
  double dt = 1.0 / 60.0;
  position += speed * dt;
  rotation_z += speed_rotation_z * dt;

  cylinder.model.translation = position;
  cylinder.model.rotation =
      rotation_transform::from_axis_angle({1, 0, 0}, -3.14f / 2) *
      rotation_transform::from_axis_angle({0, 1, 0}, -rotation_z);

  scene->camera_control.camera_model.center_of_rotation =
      position + TARGET_DIFFERENCE;
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
  std::cout << rotation_z << std::endl;
  vec3 right = {cos(rotation_z), sin(rotation_z), 0};
  vec3 front = {-sin(rotation_z), cos(rotation_z), 0};

  if (scene->inputs.keyboard.is_pressed('z')) {
    speed += SPEED * front;
  }
  if (scene->inputs.keyboard.is_pressed('s')) {
    speed -= SPEED * front;
  }
  if (scene->inputs.keyboard.is_pressed('q')) {
    speed -= SPEED * right;
  }
  if (scene->inputs.keyboard.is_pressed('d')) {
    speed += SPEED * right;
  }
  if (scene->inputs.keyboard.is_pressed(GLFW_KEY_LEFT_SHIFT)) {
    speed.z += SPEED;
  }
  if (scene->inputs.keyboard.is_pressed(GLFW_KEY_LEFT_CONTROL)) {
    speed.z -= SPEED;
  }

  if (scene->inputs.keyboard.is_pressed('a')) {
    speed_rotation_z += SPEED;
  }
  if (scene->inputs.keyboard.is_pressed('e')) {
    speed_rotation_z -= SPEED;
  }
}
