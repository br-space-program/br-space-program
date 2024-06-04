#include "NewtonianBody.hpp"
#include <cgp/cgp.hpp>
#include "../World.hpp"
#include "../constants.hpp"
#include "../mesh/primitives.hpp"
#include "../scene.hpp"
#include "../utils/physics.hpp"
#include "../utils/uvector.hpp"

NewtonianBody::NewtonianBody(scene_structure* _scene,
                             World* _world,
                             vec3 _position,
                             double _mass,
                             double _radius)
    : CelestialBody(_position, _mass, _radius),
      scene(_scene),
      world(_world),
      velocity({0, 0, 0}) {}

void NewtonianBody::update() {
  vec3 acceleration =
      compute_gravitional_force(position, world->get_celestial_bodies()) / mass;

  velocity += acceleration * scene->dt;
  position += velocity * scene->dt;
}

void NewtonianBody::set_velocity(const vec3& _velocity) {
  velocity = _velocity;
}

vec3 NewtonianBody::get_velocity() const {
  return velocity;
}
