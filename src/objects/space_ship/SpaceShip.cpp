#include "SpaceShip.hpp"
#include <cgp/cgp.hpp>
#include "SpaceShipFlame.hpp"

using cgp::mesh_drawable;

// Difference between the position of the center of rotation of camera and the
// spaceship
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
      position /* targeted point in 3D scene */,
      {0, 0, 1} /* direction of the "up" vector */);

  // Load the spaceship
  ship.initialize_data_on_gpu(
      mesh_load_file_obj(project::path + "assets/spaceship.obj"));

  ship.texture.load_and_initialize_texture_2d_on_gpu(
      project::path + "assets/spaceship.jpg", GL_CLAMP_TO_BORDER,
      GL_CLAMP_TO_BORDER);

  ship.model.scaling = 0.001f;  // coordinates are multiplied by 0.2 in the
                                // shader ship.model.translation =
  // position;    // coordinates are offseted by {1,2,0} in the shader

  // Make cylinder horizontal
  // ship.model.rotation =
  //    rotation_transform::from_axis_angle({1, 0, 0}, -3.14f / 2);

  // Add shader
  ship.shader = scene->shader_custom;

  SpaceShipFlame* ship_flame = new SpaceShipFlame(scene);

  hierarchy.add(mesh_drawable(), "ship_center");
  hierarchy.add(ship, "ship", "ship_center");
  hierarchy.add(ship_flame->flame, "ship_flame", "ship_center");

  hierarchy["ship"].transform_local.translation = {0, -0.05, 0};
  hierarchy["ship"].transform_local.rotation =
      rotation_transform::from_axis_angle({1, 0, 0}, -3.14f / 2);

  hierarchy["ship_flame"].transform_local.translation = {0, -0.08, 0};
}

void SpaceShip::update() {
  // Update the position of the sphere
  double dt = 1.0 / 60.0;
  position += speed * dt;
  rotation_z += speed_rotation_z * dt;

  hierarchy["ship_center"].transform_local.translation = position;
  hierarchy["ship_center"].transform_local.rotation =
      rotation_transform::from_axis_angle({0, 0, 1}, rotation_z);

  scene->camera_control.camera_model.center_of_rotation =
      position + 0.2 * vec3({-sin(rotation_z), cos(rotation_z), 0});

  hierarchy.update_local_to_global_coordinates();
}

void SpaceShip::render() {
  // Display the sphere
  draw(hierarchy, scene->environment);
}

void SpaceShip::render_debug() {
  // Display the wireframe of the sphere
  draw_wireframe(hierarchy, scene->environment);
}

void SpaceShip::action_keyboard() {
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
