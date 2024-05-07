#pragma once

class SpaceShipFlame;

#include "../../scene.hpp"
#include "../Object.hpp"
#include "cgp/cgp.hpp"

using cgp::mesh_drawable;

class SpaceShipFlame : public Object {
 private:
  scene_structure* scene;
  mesh_drawable sphere;

 public:
  SpaceShipFlame(scene_structure* _scene);
  void update() override;
  void render() override;
  void render_debug() override;
  ~SpaceShipFlame() override = default;

  void action_keyboard();
};
