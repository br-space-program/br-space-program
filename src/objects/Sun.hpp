#pragma once

#include "../scene.hpp"
#include "CelestialBody.hpp"
#include "SimpleObject.hpp"
#include "cgp/cgp.hpp"

using cgp::mesh_drawable;

class Sun : public CelestialBody {
 private:
  scene_structure* scene;
  mesh_drawable sphere;

 public:
  SimpleObject* atmosphere;
  Sun(scene_structure* _scene);
  void update() override;
  void render() override;
  void render_debug() override;
  ~Sun() override = default;
};
