#include "AnchoredPlanet.hpp"
#include <cgp/cgp.hpp>
#include "../constants.hpp"
#include "../mesh/planet.hpp"

using cgp::mesh_drawable;

AnchoredPlanet::AnchoredPlanet(scene_structure* _scene,
                               CelestialBody& _anchor,
                               vec3 _position,
                               double radius,
                               vec3 color)
    : KeplerianBody(_scene,
                    _anchor,
                    _position,
                    PLANET_DENSITY * pow(radius, 3),
                    radius),
      scene(_scene) {
  int Nu = 100;
  int Nv = 70;
  mesh planet_mesh = create_planet_mesh(radius, {0, 0, 0}, Nu, Nv);

  sphere.initialize_data_on_gpu(planet_mesh);
  sphere.model.scaling = 1.0f;
  sphere.material.color = color;

  sphere.vbo_position.update(planet_mesh.position);
  sphere.vbo_normal.update(planet_mesh.normal);
  sphere.vbo_color.update(planet_mesh.color);

  // ==== Atmoshpere ====
  atmosphere = new SimpleObject(scene);

  atmosphere->mesh.initialize_data_on_gpu(mesh_primitive_sphere(radius * 5));
  atmosphere->mesh.shader = scene->shader_glow;
  atmosphere->mesh.material.color = {1, 1, 1};
  atmosphere->mesh.material.alpha = 0.3;

  atmosphere->set_position(position);
}

void AnchoredPlanet::update() {
  KeplerianBody::update();
  sphere.model.translation = position;
  atmosphere->set_position(position);
  atmosphere->update();
}

void AnchoredPlanet::render() {
  draw(sphere, scene->environment);
}

void AnchoredPlanet::render_debug() {
  draw_wireframe(sphere, scene->environment);
}
