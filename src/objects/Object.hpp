#pragma once

class Object {
 public:
  virtual void update() = 0;
  virtual void render() = 0;
  virtual void render_debug() = 0;
  virtual ~Object() = default;
};
