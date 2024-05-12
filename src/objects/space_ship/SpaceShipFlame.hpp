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
  vec3 position;

 public:
  mesh_drawable flame;
  SpaceShipFlame(scene_structure* _scene,
                 cgp::hierarchy_mesh_drawable* _hierarchy,
                 std::string _name,
                 vec3 _position);

  ~SpaceShipFlame() = default;

  void on();
  void off();
};
