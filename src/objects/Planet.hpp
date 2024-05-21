#pragma once

#include "../scene.hpp"
#include "KeplerianBody.hpp"
#include "SimpleObject.hpp"
#include "cgp/cgp.hpp"

using cgp::mesh_drawable;

class Planet : public KeplerianBody {
 private:
  scene_structure* scene;
  mesh_drawable sphere;

 public:
  SimpleObject* atmosphere;
  Planet(scene_structure* _scene,
         CelestialBody& _anchor,
         vec3 _position,
         double _radius,
         vec3 color);
  void update() override;
  void render() override;
  void render_debug() override;
  ~Planet() override = default;
};
