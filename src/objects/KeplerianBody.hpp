#pragma once

#include "CelestialBody.hpp"
#include "cgp/cgp.hpp"

using cgp::vec3;

class KeplerianBody : public CelestialBody {
 private:
  CelestialBody* anchor;
  vec3 u0;
  vec3 normal;
  double theta = 0;

 public:
  KeplerianBody(CelestialBody& _anchor,
                vec3 _position,
                double _mass,
                double _radius);
  void update() override;
  virtual void render() override = 0;
  virtual void render_debug() override = 0;
  ~KeplerianBody() override = default;

  void set_anchor(CelestialBody& _anchor, vec3 _normal);
  CelestialBody* get_anchor() const;
  vec3 get_normal() const;
};
