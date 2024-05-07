#pragma once

class SpaceShip;

#include "../../scene.hpp"
#include "../Object.hpp"
#include "cgp/cgp.hpp"

using cgp::mesh_drawable;

class SpaceShip : public Object {
 private:
  scene_structure* scene;
  mesh_drawable cylinder;
  vec3 position;
  vec3 speed;
  double rotation_z;  // Around z axis
  double speed_rotation_z;

 public:
  SpaceShip(scene_structure* _scene);
  void update() override;
  void render() override;
  void render_debug() override;
  ~SpaceShip() override = default;

  void action_keyboard();
};
