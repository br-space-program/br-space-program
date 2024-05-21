#pragma once

struct scene_structure;

#include <vector>
#include "objects/CelestialBody.hpp"
#include "objects/Object.hpp"

class World {
  scene_structure* scene;
  std::vector<std::unique_ptr<CelestialBody>> celestial_bodies;
  std::vector<std::unique_ptr<Object>> objects;
  std::vector<std::unique_ptr<ObjectWithHitbox>> hitboxes;
  std::vector<std::unique_ptr<Object>>
      transparent_objects;  // Must be drawn after all other objects
  vec3 light_position = {0, 0, 0};

 public:
  World(scene_structure* _scene);

  void update();
  void render();
  void render_debug();

  void add_celestial_body(std::unique_ptr<CelestialBody> body);
  void add_object(std::unique_ptr<Object> object);
  void add_hitbox(std::unique_ptr<ObjectWithHitbox> object);
  void add_transparent_object(std::unique_ptr<Object> object);

  std::vector<std::unique_ptr<CelestialBody>>& get_celestial_bodies();
  std::vector<std::unique_ptr<Object>>& get_objects();
  std::vector<std::unique_ptr<ObjectWithHitbox>>& get_hitboxes();
  std::vector<std::unique_ptr<Object>>& get_transparent_objects();

  void set_light_position(vec3 _position);
  vec3 get_light_position() const;
};
