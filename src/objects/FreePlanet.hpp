#pragma once

#include "../scene.hpp"
#include "NewtonianBody.hpp"
#include "SimpleObject.hpp"
#include "cgp/cgp.hpp"

using cgp::mesh_drawable;

class FreePlanet : public NewtonianBody {
 private:
  scene_structure* scene;
  mesh_drawable sphere;
  vec3 next_position;

 public:
  FreePlanet(scene_structure* _scene,
             World* _world,
             vec3 _position,
             double _radius,
             vec3 color);
  void update() override;
  void render() override;
  void render_debug() override;
  ~FreePlanet() override = default;
};
