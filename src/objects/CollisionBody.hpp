#pragma once

#include <tuple>
#include "cgp/cgp.hpp"

using cgp::vec3;

/*
Used to define an sphere object that can collide with other objects.

*/
class CollisionBody {
 protected:
  vec3 position;
  double radius;

 public:
  CollisionBody(vec3 _position, double _radius);

  virtual ~CollisionBody() = default;
  void set_position(vec3 _position);
  vec3 get_position() const;
  void set_radius(double _radius);
  double get_radius() const;
  /** Returns
   * - true/false if the body is in collision with the given body
   * - the distance between the two bodies
   */
  std::tuple<bool, double> is_in_collision(const CollisionBody& body) const;
};