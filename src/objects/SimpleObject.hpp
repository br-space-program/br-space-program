#pragma once

#include "../scene.hpp"
#include "Object.hpp"
#include "cgp/cgp.hpp"

using cgp::mesh_drawable;

class SimpleObject : public Object {
 public:
  mesh_drawable mesh;
  scene_structure* scene;

  void set_position(vec3 _position);

  SimpleObject(scene_structure* _scene);
  void update() override;
  void render() override;
  void render_debug() override;
  ~SimpleObject() override = default;
};