#include "Sun.hpp"
#include <cgp/cgp.hpp>
#include "../constants.hpp"

using cgp::mesh_drawable;

Sun::Sun(scene_structure* _scene,
         vec3 surface_color,
         vec3 atmosphere_color,
         vec3 position,
         double radius)
    : CelestialBody(position, STAR_DENSITY * pow(radius, 3), radius) {
  scene = _scene;

  mesh sphere_mesh = mesh_primitive_sphere(radius);

  sphere.initialize_data_on_gpu(sphere_mesh);
  sphere.model.translation = position;
  sphere.material.color = surface_color;

  // Make it a light source
  sphere.material.phong.ambient = 1;
  sphere.material.phong.diffuse = 0;
  sphere.material.phong.specular = 0;

  // Add shader
  sphere.shader = scene->shader_custom;

  atmosphere = new SimpleObject(scene);

  atmosphere->mesh.initialize_data_on_gpu(sphere_mesh);
  atmosphere->mesh.model.scaling = 3;
  atmosphere->mesh.material.color = atmosphere_color;

  // Make it a light source
  atmosphere->mesh.material.phong.ambient = 1;
  atmosphere->mesh.material.phong.diffuse = 0;
  atmosphere->mesh.material.phong.specular = 0;

  // Add shader
  atmosphere->mesh.shader = scene->shader_glow;

  atmosphere->set_position(position);
  atmosphere->update();
}

void Sun::update() {
  // Update the position of the sphere
}

void Sun::render() {
  // Display the sphere
  draw(sphere, scene->environment);
}

void Sun::render_debug() {
  // Display the wireframe of the sphere
  draw_wireframe(sphere, scene->environment);
  collision_render_debug();
}
