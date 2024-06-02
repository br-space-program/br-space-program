#include "../objects/Planet.hpp"
#include "../objects/Sun.hpp"
#include "../utils/rgb.hpp"
#include "../utils/uvector.hpp"
#include "generic.hpp"

World* create_world_homeworld_harkonen(scene_structure* scene, vec3 p0) {
  World* world = new World(scene);

  world->set_skybox(project::path + "assets/skybox-giedi-prime.png");

  Sun* sun = new Sun(scene, RGB(256, 223, 123), RGB(256, 223, 123), p0, 75);
  world->add_object(std::unique_ptr<Object>(sun));
  world->add_celestial_body(std::unique_ptr<CelestialBody>(sun));
  world->add_hitbox(std::unique_ptr<ObjectWithHitbox>(sun));
  world->set_light_position(sun->get_position());
  world->add_transparent_object(std::unique_ptr<Object>(sun->atmosphere));

  Planet* planet1 =
      new Planet(scene, *sun, p0 - 590 * ux, 12, RGB(40, 166, 69));
  world->add_celestial_body(std::unique_ptr<CelestialBody>(planet1));
  world->add_object(std::unique_ptr<Object>(planet1));
  world->add_hitbox(std::unique_ptr<ObjectWithHitbox>(planet1));
  world->add_transparent_object(std::unique_ptr<Object>(planet1->atmosphere));

  vec3 p2 = p0 + 510 * ux;
  Planet* planet2 = new Planet(scene, *sun, p2, 25, RGB(233, 127, 95));
  world->add_celestial_body(std::unique_ptr<CelestialBody>(planet2));
  world->add_object(std::unique_ptr<Object>(planet2));
  world->add_hitbox(std::unique_ptr<ObjectWithHitbox>(planet2));
  world->add_transparent_object(std::unique_ptr<Object>(planet2->atmosphere));

  Planet* moon1 =
      new Planet(scene, *planet2, p2 + 90 * ux, 5, RGB(250, 250, 250));
  world->add_celestial_body(std::unique_ptr<CelestialBody>(moon1));
  world->add_object(std::unique_ptr<Object>(moon1));
  world->add_hitbox(std::unique_ptr<ObjectWithHitbox>(moon1));

  return world;
}
