#include "World.hpp"
#include "scene.hpp"

World::World(scene_structure* _scene) : scene(_scene) {}

void World::update() {
  for (auto& object : objects) {
    object->update();
  }
}

void World::render() {
  scene->environment.uniform_generic.uniform_vec3["light"] = light_position;

  for (auto& object : objects) {
    object->render();
  }
}

void World::render_debug() {
  for (auto& object : objects) {
    object->render_debug();
  }

  for (auto& object : transparent_objects) {
    object->render_debug();
  }
}

void World::add_celestial_body(std::unique_ptr<CelestialBody> body) {
  celestial_bodies.push_back(std::move(body));
}

void World::add_object(std::unique_ptr<Object> object) {
  objects.push_back(std::move(object));
}

void World::add_hitbox(std::unique_ptr<ObjectWithHitbox> object) {
  hitboxes.push_back(std::move(object));
}

void World::add_transparent_object(std::unique_ptr<Object> object) {
  transparent_objects.push_back(std::move(object));
}

std::vector<std::unique_ptr<CelestialBody>>& World::get_celestial_bodies() {
  return celestial_bodies;
}

std::vector<std::unique_ptr<Object>>& World::get_objects() {
  return objects;
}

std::vector<std::unique_ptr<ObjectWithHitbox>>& World::get_hitboxes() {
  return hitboxes;
}

std::vector<std::unique_ptr<Object>>& World::get_transparent_objects() {
  return transparent_objects;
}

void World::set_light_position(vec3 _position) {
  light_position = _position;
}

vec3 World::get_light_position() const {
  return light_position;
}
