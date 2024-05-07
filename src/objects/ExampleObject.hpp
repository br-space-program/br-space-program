#pragma once

#include "../scene.hpp"
#include "Object.hpp"
#include "cgp/cgp.hpp"

using cgp::mesh_drawable;

class ExampleObject : public Object {
 private:
  scene_structure* scene;
  mesh_drawable sphere;

 public:
  ExampleObject(scene_structure* _scene);
  void update() override;
  void render() override;
  void render_debug() override;
  ~ExampleObject() override = default;
};
