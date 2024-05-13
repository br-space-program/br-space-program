#pragma once

class SpaceShipFlame;

#include <string>
#include "../../scene.hpp"
#include "../Object.hpp"
#include "cgp/cgp.hpp"

using cgp::mesh_drawable;

class SpaceShipFlame {
 private:
  scene_structure* scene;
  cgp::hierarchy_mesh_drawable* hierarchy;
  std::string name;

  mesh_drawable flare;
  mesh_drawable flame;
  mesh_drawable flame_small;

 public:
  SpaceShipFlame(scene_structure* _scene,
                 cgp::hierarchy_mesh_drawable* _hierarchy,
                 std::string _name,
                 double scale,
                 vec3 position,
                 vec3 rotation_axis,
                 double rotation_angle);
  void update();

  ~SpaceShipFlame() = default;

  void on();
  void off();
};
