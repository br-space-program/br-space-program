#include "SpaceShip.hpp"
#include <cgp/cgp.hpp>
#include "../../constants.hpp"

using cgp::mesh_drawable;

// Difference between the position of the center of rotation of camera and the
// spaceship
double SPEED = 0.1;

SpaceShip::SpaceShip(scene_structure* _scene) : ObjectWithHitbox(1) {
  scene = _scene;
  position = {100, 0, 0};
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

  ship.model.scaling = 0.001f;
  ship.shader = scene->shader_custom;

  // ==== Setup ship ====

  hierarchy.add(mesh_drawable(), "ship_center");  // Empty point for the center
  hierarchy.add(ship, "ship", "ship_center");

  hierarchy["ship"].transform_local.translation = {0, -0.05, 0};
  hierarchy["ship"].transform_local.rotation =
      rotation_transform::from_axis_angle({1, 0, 0}, -3.14f / 2);

  // ==== Setup Flames ====
  ship_flame_front = new SpaceShipFlame(scene, &hierarchy, "ship_flame_front",
                                        0.5, {0, 0.4, 0}, {1, 0, 0}, 0);

  ship_flame_top = new SpaceShipFlame(scene, &hierarchy, "ship_flame_top", 0.5,
                                      {0, 0.14, 0.115}, {1, 0, 0}, 3.14 / 2);

  ship_flame_bottom =
      new SpaceShipFlame(scene, &hierarchy, "ship_flame_bottom", 0.5,
                         {0, 0.14, -0.115}, {1, 0, 0}, -3.14 / 2);

  ship_flame_right_up =
      new SpaceShipFlame(scene, &hierarchy, "ship_flame_right_up", 0.5,
                         {0.09, 0.28, 0}, {0, 0, 1}, -3.14 / 2);

  ship_flame_right_down =
      new SpaceShipFlame(scene, &hierarchy, "ship_flame_right_down", 0.5,
                         {0.09, 0.05, 0}, {0, 0, 1}, -3.14 / 2);

  ship_flame_left_up =
      new SpaceShipFlame(scene, &hierarchy, "ship_flame_left_up", 0.5,
                         {-0.09, 0.28, 0}, {0, 0, 1}, 3.14 / 2);

  ship_flame_left_down =
      new SpaceShipFlame(scene, &hierarchy, "ship_flame_left_down", 0.5,
                         {-0.09, 0.05, 0}, {0, 0, 1}, 3.14 / 2);

  ship_flame = new SpaceShipFlame(
      scene, &hierarchy, "ship_flame", 1, {0, -0.17, 0}, {1, 0, 0},
      3.14);  // Order is important to avoid overlapping with shader_glow
}

void SpaceShip::update() {
  // Update the position of the sphere
  double dt = 1.0 / 60.0;

  // Gravity
  vec3 acceleration = vec3({0, 0, 0});
  for (int i = 0; i < scene->celestial_bodies.size(); i++) {
    CelestialBody* body = scene->celestial_bodies[i].get();

    vec3 direction = body->get_position() - position;
    double distance = norm(direction);

    acceleration += GRAVITATIONAL_CONSTANT * direction * body->get_mass() /
                    pow(distance, 3);
  }

  speed += acceleration * dt;
  position += speed * dt;
  rotation_z += speed_rotation_z * dt;

  // Collision
  for (int i = 0; i < scene->hitboxes.size(); i++) {
    ObjectWithHitbox* body = scene->hitboxes[i].get();

    auto [is_collision, distance] = body->is_in_collision(*this);

    std::cout << "Collision: " << is_collision << "Distance: " << distance
              << std::endl;

    if (is_collision) {
      vec3 direction = body->get_position() - position;
      position -= direction * (distance - body->get_radius() - radius);
      speed = {0, 0, 0};
    }
  }

  hierarchy["ship_center"].transform_local.translation = position;
  hierarchy["ship_center"].transform_local.rotation =
      rotation_transform::from_axis_angle({0, 0, 1}, rotation_z);

  scene->camera_control.camera_model.center_of_rotation =
      position + 0.2 * vec3({-sin(rotation_z), cos(rotation_z), 0});

  ship_flame->update();
  ship_flame_front->update();
  ship_flame_top->update();
  ship_flame_bottom->update();
  ship_flame_right_up->update();
  ship_flame_right_down->update();
  ship_flame_left_up->update();
  ship_flame_left_down->update();

  hierarchy.update_local_to_global_coordinates();
}

void SpaceShip::render() {
  // Display the sphere
  draw(hierarchy, scene->environment);
}

void SpaceShip::render_debug() {
  // Display the wireframe of the sphere
  draw_wireframe(hierarchy, scene->environment);
  collision_render_debug();
}

void SpaceShip::action_keyboard() {
  vec3 right = {cos(rotation_z), sin(rotation_z), 0};
  vec3 front = {-sin(rotation_z), cos(rotation_z), 0};

  // Front
  if (scene->inputs.keyboard.is_pressed('z')) {
    speed += SPEED * front;
    ship_flame->on();
  } else {
    ship_flame->off();
  }

  if (scene->inputs.keyboard.is_pressed('s')) {
    speed -= SPEED * front;
    ship_flame_front->on();
  } else {
    ship_flame_front->off();
  }

  // Side
  if (scene->inputs.keyboard.is_pressed('q')) {
    speed -= SPEED * right;
  } else {
  }

  if (scene->inputs.keyboard.is_pressed('d')) {
    speed += SPEED * right;
  } else {
  }

  // Top
  if (scene->inputs.keyboard.is_pressed('f')) {
    speed.z += SPEED;
    ship_flame_bottom->on();
  } else {
    ship_flame_bottom->off();
  }

  // Down
  if (scene->inputs.keyboard.is_pressed('v')) {
    speed.z -= SPEED;
    ship_flame_top->on();
  } else {
    ship_flame_top->off();
  }

  // Rotation
  if (scene->inputs.keyboard.is_pressed('a')) {
    speed_rotation_z += SPEED;
    ship_flame_right_up->on();
    ship_flame_left_down->on();
  } else {
    ship_flame_right_up->off();
    ship_flame_left_down->off();
  }

  if (scene->inputs.keyboard.is_pressed('e')) {
    speed_rotation_z -= SPEED;
    ship_flame_left_up->on();
    ship_flame_right_down->on();
  } else {
    ship_flame_left_up->off();
    ship_flame_right_down->off();
  }

  // Flame for rotation and side movement
  // (Defined here because else it would be turned off by the previous)

  if (scene->inputs.keyboard.is_pressed('a') ||
      scene->inputs.keyboard.is_pressed('q')) {
    ship_flame_right_up->on();
  } else {
    ship_flame_right_up->off();
  }

  if (scene->inputs.keyboard.is_pressed('a') ||
      scene->inputs.keyboard.is_pressed('d')) {
    ship_flame_left_down->on();
  } else {
    ship_flame_left_down->off();
  }

  if (scene->inputs.keyboard.is_pressed('e') ||
      scene->inputs.keyboard.is_pressed('d')) {
    ship_flame_left_up->on();
  } else {
    ship_flame_left_up->off();
  }

  if (scene->inputs.keyboard.is_pressed('e') ||
      scene->inputs.keyboard.is_pressed('q')) {
    ship_flame_right_down->on();
  } else {
    ship_flame_right_down->off();
  }
}
