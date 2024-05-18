#pragma once

class SpaceShipFlame;
class SimpleObject;

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
  double scale;
  vec3 position;
  vec3 rotation_axis;
  double rotation_angle;

  mesh_drawable flame;
  mesh_drawable flame_small;
  mesh_drawable flare_mesh;

 public:
  SimpleObject* flare;
  SpaceShipFlame(scene_structure* _scene,
                 cgp::hierarchy_mesh_drawable* _hierarchy,
                 std::string _name,
                 double _scale,
                 vec3 _position,
                 vec3 _rotation_axis,
                 double _rotation_angle);
  void update();

  ~SpaceShipFlame() = default;

  void on();
  void off();
};

double flame_offset(double t);