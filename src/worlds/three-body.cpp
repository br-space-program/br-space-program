#include "three-body.hpp"
#include "../objects/FreePlanet.hpp"
#include "../utils/rgb.hpp"
#include "../utils/uvector.hpp"

World* create_world_three_body_problem(scene_structure* scene, vec3 p0) {
  World* world = new World(scene);

  world->set_skybox(project::path + "assets/skybox/", ".png");
  world->set_light_position({0, 0, 0});

  vec3 p1 = p0 - 50 * ux + 42 * uy;
  FreePlanet* planet1 = new FreePlanet(scene, world, p1, 7, RGB(40, 167, 69));
  planet1->set_velocity({0, -3, 0});
  planet1->set_mass(planet1->get_mass() * 100);
  world->add_celestial_body(std::unique_ptr<CelestialBody>(planet1));
  world->add_object(std::unique_ptr<Object>(planet1));
  world->add_hitbox(std::unique_ptr<ObjectWithHitbox>(planet1));

  vec3 p2 = p0 - 32 * ux - 10 * uy - 43 * uz;
  FreePlanet* planet2 = new FreePlanet(scene, world, p2, 7, RGB(251, 139, 11));
  planet2->set_velocity({0, 0, 0.5});
  planet2->set_mass(planet2->get_mass() * 100);
  world->add_celestial_body(std::unique_ptr<CelestialBody>(planet2));
  world->add_object(std::unique_ptr<Object>(planet2));
  world->add_hitbox(std::unique_ptr<ObjectWithHitbox>(planet2));

  vec3 p3 = 3 * p0 - p1 - p2;
  FreePlanet* planet3 = new FreePlanet(scene, world, p3, 7, RGB(250, 250, 250));
  planet3->set_velocity({-.5, 0, -.5});
  planet3->set_mass(planet3->get_mass() * 100);
  world->add_celestial_body(std::unique_ptr<CelestialBody>(planet3));
  world->add_object(std::unique_ptr<Object>(planet3));
  world->add_hitbox(std::unique_ptr<ObjectWithHitbox>(planet3));

  return world;
}
