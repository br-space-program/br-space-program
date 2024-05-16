#pragma once

#include "../scene.hpp"
#include "CelestialBody.hpp"
#include "cgp/cgp.hpp"

using cgp::mesh_drawable;

class Sun : public CelestialBody {
 private:
  scene_structure* scene;
  mesh_drawable sphere;
  mesh_drawable atmosphere;

 public:
  Sun(scene_structure* _scene);
  void update() override;
  void render() override;
  void render_debug() override;
  ~Sun() override = default;
};
