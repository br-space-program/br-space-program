#include "Sun.hpp"
#include <cgp/cgp.hpp>

using cgp::mesh_drawable;

Sun::Sun(scene_structure* _scene)
    : CelestialBody({0, 0, 0}, 42), CollisionBody({0, 0, 0}, 20.0) {
  scene = _scene;

  mesh sphere_mesh = mesh_primitive_sphere();

  sphere.initialize_data_on_gpu(sphere_mesh);
  sphere.model.scaling =
      5.0f;  // coordinates are multiplied by 0.2 in the shader
  sphere.model.translation = {0, 0, 0};
  sphere.material.color = {
      1, 0.87f, 0.48f};  // sphere will appear red (r,g,b components in [0,1])

  // Make it a light source
  sphere.material.phong.ambient = 1;
  sphere.material.phong.diffuse = 0;
  sphere.material.phong.specular = 0;

  // Add shader
  sphere.shader = scene->shader_custom;

  atmosphere.initialize_data_on_gpu(sphere_mesh);
  atmosphere.model.scaling =
      20.0f;  // coordinates are multiplied by 0.2 in the shader
  atmosphere.model.translation = {0, 0, 0};
  atmosphere.material.color = {
      1, 0.87f,
      0.48f};  // atmosphere will appear red (r,g,b components in [0,1])

  // Make it a light source
  atmosphere.material.phong.ambient = 1;
  atmosphere.material.phong.diffuse = 0;
  atmosphere.material.phong.specular = 0;

  // Add shader
  atmosphere.shader = scene->shader_glow;
}

void Sun::update() {
  // Update the position of the sphere
}

void Sun::render() {
  // Display the sphere
  draw(sphere, scene->environment);
  draw(atmosphere, scene->environment);
}

void Sun::render_debug() {
  // Display the wireframe of the sphere
  draw_wireframe(sphere, scene->environment);
}
