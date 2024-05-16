#pragma once

#include "CelestialBody.hpp"
#include "cgp/cgp.hpp"

using cgp::vec3;

class KeplerianBody : public CelestialBody {
 private:
  CelestialBody* anchor;

 public:
  KeplerianBody(CelestialBody& _anchor,
                vec3 _position,
                double _mass,
                double _radius);
  void update() override;
  virtual void render() override = 0;
  virtual void render_debug() override = 0;
  ~KeplerianBody() override = default;

  void set_anchor(CelestialBody& _anchor);
  CelestialBody* get_anchor() const;
};
