#pragma once

#include "Object.hpp"
#include "ObjectWithHitbox.hpp"
#include "cgp/cgp.hpp"

using cgp::vec3;

class CelestialBody : public ObjectWithHitbox {
 protected:
  double mass;

 public:
  CelestialBody(vec3 _position, double _mass, double _radius);
  virtual void update() override = 0;
  virtual void render() override = 0;
  virtual void render_debug() override = 0;
  virtual ~CelestialBody() override = default;
  void set_position(vec3 _position);
  vec3 get_position() const;
  void set_mass(double _mass);
  double get_mass() const;
};
