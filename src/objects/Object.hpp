#pragma once

#include "cgp/cgp.hpp"

using cgp::vec3;

class Object {
 protected:
  vec3 position;

 public:
  virtual void update() = 0;
  virtual void render() = 0;
  virtual void render_debug() = 0;
  virtual ~Object() = default;

  vec3 get_position() const;
};
