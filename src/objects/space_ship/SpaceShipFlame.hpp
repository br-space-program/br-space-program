#pragma once

class SpaceShipFlame;

#include "../../scene.hpp"
#include "../Object.hpp"
#include "cgp/cgp.hpp"

using cgp::mesh_drawable;

class SpaceShipFlame {
 private:
  scene_structure* scene;

 public:
  mesh_drawable flame;
  SpaceShipFlame(scene_structure* _scene);
  ~SpaceShipFlame() = default;

  void action_keyboard();
};
