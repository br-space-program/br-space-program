#pragma once

struct scene_structure;
class ObjectWithHitbox;

#include <tuple>
#include "Object.hpp"
#include "cgp/cgp.hpp"

using cgp::mesh_drawable;
using cgp::vec3;

/*
Used to define an sphere object that can collide with other objects.

*/
class ObjectWithHitbox : public Object {
 private:
  // scene_structure* scene;
  mesh_drawable debug_sphere;

 protected:
  double radius;

 public:
  ObjectWithHitbox(double _radius);

  virtual ~ObjectWithHitbox() = default;
  void set_radius(double _radius);
  double get_radius() const;
  void collision_render_debug();

  /** Returns
   * - true/false if the body is in collision with the given body
   * - the distance between the two bodies
   */
  std::tuple<bool, double> is_in_collision(const ObjectWithHitbox& body) const;
};