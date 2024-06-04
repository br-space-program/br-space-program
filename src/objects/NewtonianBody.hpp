#pragma once

#include <cgp/cgp.hpp>
#include "../World.hpp"
#include "CelestialBody.hpp"

using cgp::vec3;

class NewtonianBody : public CelestialBody {
 private:
  scene_structure* scene;
  World* world;
  vec3 velocity;

 public:
  NewtonianBody(scene_structure* _scene,
                World* _world,
                vec3 _position,
                double _mass,
                double _radius);
  void update() override;
  virtual void render() override = 0;
  virtual void render_debug() override = 0;
  ~NewtonianBody() override = default;
  void set_velocity(const vec3& _velocity);
  vec3 get_velocity() const;
};
