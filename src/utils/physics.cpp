#include "physics.hpp"
#include "../constants.hpp"

vec3 compute_gravitional_force(
    const cgp::vec3& position,
    const std::vector<std::unique_ptr<CelestialBody>>& bodies) {
  vec3 force(0, 0, 0);

  for (int i = 0; i < bodies.size(); i++) {
    CelestialBody* body = bodies[i].get();

    if (cgp::norm(body->get_position() - position) < 1e-8) {
      continue;
    }

    vec3 direction = body->get_position() - position;
    double distance = cgp::norm(direction);

    force += GRAVITATIONAL_CONSTANT * direction * body->get_mass() /
             pow(distance, 3);
  }

  return force;
}
