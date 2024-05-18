#pragma once

class SpaceShip;

struct scene_structure;

#include "../ObjectWithHitbox.hpp"
#include "SpaceShipFlame.hpp"
#include "cgp/cgp.hpp"

using cgp::mesh_drawable;

class SpaceShip : public ObjectWithHitbox {
 public:
  std::vector<std::unique_ptr<SpaceShipFlame>> ship_flames;

 private:
  scene_structure* scene;
  mesh_drawable ship;

  SpaceShipFlame* ship_flame;
  SpaceShipFlame* ship_flame_front;
  SpaceShipFlame* ship_flame_top;
  SpaceShipFlame* ship_flame_bottom;
  SpaceShipFlame* ship_flame_right_up;
  SpaceShipFlame* ship_flame_right_down;
  SpaceShipFlame* ship_flame_left_up;
  SpaceShipFlame* ship_flame_left_down;

  vec3 speed;
  double rotation_z;  // Around z axis
  double speed_rotation_z;

  cgp::hierarchy_mesh_drawable hierarchy;

 public:
  SpaceShip(scene_structure* _scene);
  void update() override;
  void render() override;
  void render_debug() override;
  ~SpaceShip() override = default;

  void action_keyboard();
};

/**
 * Returns the vector `to_reflect` reflected based on vector `normal`.
 */
vec3 reflect(vec3 to_reflect, vec3 normal);