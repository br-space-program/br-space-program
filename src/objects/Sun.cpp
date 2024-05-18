#include "Sun.hpp"
#include <cgp/cgp.hpp>

using cgp::mesh_drawable;

Sun::Sun(scene_structure* _scene) : CelestialBody({0, 0, 0}, 42, 5) {
  scene = _scene;

  mesh sphere_mesh = mesh_primitive_sphere(5);

  sphere.initialize_data_on_gpu(sphere_mesh);
  sphere.model.translation = {0, 0, 0};
  sphere.material.color = {
      1, 0.87f, 0.48f};  // sphere will appear red (r,g,b components in [0,1])

  // Make it a light source
  sphere.material.phong.ambient = 1;
  sphere.material.phong.diffuse = 0;
  sphere.material.phong.specular = 0;

  // Add shader
  sphere.shader = scene->shader_custom;

  atmosphere = new SimpleObject(scene);

  atmosphere->mesh.initialize_data_on_gpu(sphere_mesh);
  atmosphere->mesh.model.scaling =
      3;  // coordinates are multiplied by 0.2 in the shader
  atmosphere->mesh.material.color = {1, 0.87f, 0.48f};

  // Make it a light source
  atmosphere->mesh.material.phong.ambient = 1;
  atmosphere->mesh.material.phong.diffuse = 0;
  atmosphere->mesh.material.phong.specular = 0;

  // Add shader
  atmosphere->mesh.shader = scene->shader_glow;

  atmosphere->set_position({0, 0, 0});
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
