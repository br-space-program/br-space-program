#include "generic.hpp"
#include "../objects/Planet.hpp"
#include "../objects/Sun.hpp"
#include "../utils/rgb.hpp"
#include "../utils/uvector.hpp"

World* create_world_generic1(scene_structure* scene, vec3 p0) {
  World* world = new World(scene);

  world->set_skybox(project::path + "assets/skybox/", ".png");

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

World* create_world_generic2(scene_structure* scene, vec3 p0) {
  World* world = new World(scene);

  world->set_skybox(project::path + "assets/skybox/", ".png");

  Sun* sun = new Sun(scene, RGB(57, 76, 211), RGB(0, 175, 207), p0, 150);
  world->add_object(std::unique_ptr<Object>(sun));
  world->add_celestial_body(std::unique_ptr<CelestialBody>(sun));
  world->add_hitbox(std::unique_ptr<ObjectWithHitbox>(sun));
  world->set_light_position(sun->get_position());
  world->add_transparent_object(std::unique_ptr<Object>(sun->atmosphere));

  Planet* planet1 =
      new Planet(scene, *sun, p0 + 300 * uy, 10, RGB(129, 47, 39));
  world->add_celestial_body(std::unique_ptr<CelestialBody>(planet1));
  world->add_object(std::unique_ptr<Object>(planet1));
  world->add_hitbox(std::unique_ptr<ObjectWithHitbox>(planet1));
  world->add_transparent_object(std::unique_ptr<Object>(planet1->atmosphere));

  vec3 p2 = p0 + 800 * ux;
  Planet* planet2 = new Planet(scene, *sun, p2, 40, RGB(40, 167, 69));
  world->add_celestial_body(std::unique_ptr<CelestialBody>(planet2));
  world->add_object(std::unique_ptr<Object>(planet2));
  world->add_hitbox(std::unique_ptr<ObjectWithHitbox>(planet2));
  world->add_transparent_object(std::unique_ptr<Object>(planet2->atmosphere));

  Planet* moon1 =
      new Planet(scene, *planet2, p2 + 100 * ux, 2, RGB(134, 142, 150));
  world->add_celestial_body(std::unique_ptr<CelestialBody>(moon1));
  world->add_object(std::unique_ptr<Object>(moon1));
  world->add_hitbox(std::unique_ptr<ObjectWithHitbox>(moon1));

  Planet* moon2 =
      new Planet(scene, *planet2, p2 - 150 * uy, 1, RGB(134, 142, 150));
  world->add_celestial_body(std::unique_ptr<CelestialBody>(moon2));
  world->add_object(std::unique_ptr<Object>(moon2));
  world->add_hitbox(std::unique_ptr<ObjectWithHitbox>(moon2));

  return world;
}

World* create_world_generic3(scene_structure* scene, vec3 p0) {
  World* world = new World(scene);

  world->set_skybox(project::path + "assets/skybox/", ".png");

  Sun* sun = new Sun(scene, RGB(147, 27, 28), RGB(220, 53, 69), p0, 40);
  world->add_object(std::unique_ptr<Object>(sun));
  world->add_celestial_body(std::unique_ptr<CelestialBody>(sun));
  world->add_hitbox(std::unique_ptr<ObjectWithHitbox>(sun));
  world->set_light_position(sun->get_position());
  world->add_transparent_object(std::unique_ptr<Object>(sun->atmosphere));

  vec3 p1 = p0 + 150 * uy;
  vec3 n1 = {1, 0, 1};
  Planet* planet = new Planet(scene, *sun, p1, 8, RGB(45, 56, 73));
  world->add_celestial_body(std::unique_ptr<CelestialBody>(planet));
  world->add_object(std::unique_ptr<Object>(planet));
  world->add_hitbox(std::unique_ptr<ObjectWithHitbox>(planet));
  world->add_transparent_object(std::unique_ptr<Object>(planet->atmosphere));
  planet->set_anchor(*sun, n1);

  Planet* moon =
      new Planet(scene, *planet, p1 + 20 * ux, 2, RGB(134, 142, 150));
  world->add_celestial_body(std::unique_ptr<CelestialBody>(moon));
  world->add_object(std::unique_ptr<Object>(moon));
  world->add_hitbox(std::unique_ptr<ObjectWithHitbox>(moon));
  moon->set_anchor(*planet, cgp::cross(uy, n1));

  return world;
}

World* create_world_empty(scene_structure* scene) {
  World* world = new World(scene);
  world->set_skybox(project::path + "assets/skybox/", ".png");
  return world;
}
