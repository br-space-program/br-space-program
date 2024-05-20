#include "PerfectSphere.hpp"
#include <cgp/cgp.hpp>
#include "../mesh/primitives.hpp"

using cgp::mesh_drawable;

PerfectSphere::PerfectSphere(scene_structure* _scene,
                             vec3 _position,
                             double _radius) {
  scene = _scene;
  position = _position;

  /* Parameters */
  int Nu = 100;
  int Nv = 70;
  float R = 1.0f;
  vec3 center = {0, 0, 0};

  /* Building the sphere */
  mesh shape = create_sphere_mesh(R, center, Nu, Nv);

  sphere.initialize_data_on_gpu(shape);
  sphere.model.scaling = _radius;
  sphere.material.color = {0.9f, 0.65f, 0.27f};
}

void PerfectSphere::update() {
  sphere.model.translation = position;
}

void PerfectSphere::render() {
  draw(sphere, scene->environment);
}

void PerfectSphere::render_debug() {
  draw_wireframe(sphere, scene->environment);
}
