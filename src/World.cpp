#include "World.hpp"
#include "scene.hpp"

World::World(scene_structure* _scene) : scene(_scene) {}

void World::update() {
  for (auto& object : objects) {
    object->update();
  }
}

void World::render(bool include_spaceship) {
  scene->environment.uniform_generic.uniform_vec3["light"] = light_position;

  glDepthMask(GL_FALSE);
  draw(skybox, scene->environment);
  glDepthMask(GL_TRUE);

  for (auto& object : objects) {
    object->render();
  }

  if (include_spaceship) {
    scene->space_ship->render();
  }
}

void World::render_transparent(bool include_spaceship) {
  // Step 1: collect
  std::vector<Object*> target_objects;
  for (auto& object : transparent_objects) {
    target_objects.push_back(object.get());
  }
  if (include_spaceship) {
    for (auto& ship_flame : scene->space_ship->ship_flames) {
      target_objects.push_back((Object*)ship_flame->flare);
    }
  }

  // Step 2: sort
  vec3 camera_position = scene->camera_control.camera_model.position();
  std::sort(target_objects.begin(), target_objects.end(),
            [&camera_position](const Object* a, const Object* b) {
              return norm(a->get_position() - camera_position) >
                     norm(b->get_position() - camera_position);
            });

  // Step 3: render
  for (auto& object : target_objects) {
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

void World::set_skybox(const std::string& filepath) {
  image_structure image_skybox = image_load_file(filepath);

  std::vector<image_structure> image_skybox_grid =
      image_split_grid(image_skybox, 4, 3);

  skybox.initialize_data_on_gpu();
  skybox.texture.initialize_cubemap_on_gpu(
      image_skybox_grid[1], image_skybox_grid[7], image_skybox_grid[5],
      image_skybox_grid[3], image_skybox_grid[10], image_skybox_grid[4]);
}
