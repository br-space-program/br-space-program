#include "Sun.hpp"
#include <cgp/cgp.hpp>

using cgp::mesh_drawable;

Sun::Sun(scene_structure* _scene) {
  scene = _scene;

  mesh sphere_mesh = mesh_primitive_sphere();

  sphere.initialize_data_on_gpu(sphere_mesh);
  sphere.model.scaling =
      5.0f;  // coordinates are multiplied by 0.2 in the shader
  sphere.model.translation = {
      1, 2, 0};  // coordinates are offseted by {1,2,0} in the shader
  sphere.material.color = {
      1, 0.5f, 0.5f};  // sphere will appear red (r,g,b components in [0,1])

  // Make it a light source
  sphere.material.phong.ambient = 1;
  sphere.material.phong.diffuse = 0;
  sphere.material.phong.specular = 0;

  // Add shader
  sphere.shader = scene->shader_custom;
}

void Sun::update() {
  // Update the position of the sphere
  sphere.model.translation = {1, 2, 0};
}

void Sun::render() {
  // Display the sphere
  draw(sphere, scene->environment);
}

void Sun::render_debug() {
  // Display the wireframe of the sphere
  draw_wireframe(sphere, scene->environment);
}
