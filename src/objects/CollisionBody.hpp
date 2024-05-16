#pragma once

struct scene_structure;
class CollisionBody;

#include <tuple>
#include "cgp/cgp.hpp"

using cgp::mesh_drawable;
using cgp::vec3;

/*
Used to define an sphere object that can collide with other objects.

*/
class CollisionBody {
 private:
  scene_structure* scene;
  mesh_drawable debug_sphere;

 protected:
  vec3 position;
  double radius;

 public:
  CollisionBody(scene_structure* _scene, vec3 _position, double _radius);

  virtual ~CollisionBody() = default;
  void set_position(vec3 _position);
  vec3 get_position() const;
  void set_radius(double _radius);
  double get_radius() const;
  void collision_render_debug();

  /** Returns
   * - true/false if the body is in collision with the given body
   * - the distance between the two bodies
   */
  std::tuple<bool, double> is_in_collision(const CollisionBody& body) const;
};