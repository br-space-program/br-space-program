#include "PerfectSphere.hpp"
#include <cgp/cgp.hpp>

using cgp::mesh_drawable;

PerfectSphere::PerfectSphere(scene_structure* _scene) {
  scene = _scene;

  /* Parameters */
  int Nu = 100;
  int Nv = 70;
  float R = 1.0f;
  vec3 center = {0, 0, 0};

  /* Building the sphere */

  mesh shape;

  for (int ku = 0; ku < Nu; ku++) {
    const float u = ku / (Nu - 1.0f);
    const float theta = Pi * u;

    for (int kv = 0; kv < Nv; kv++) {
      float const v = kv / (Nv - 1.0f);

      const float phi = 2 * Pi * v;

      const vec3 p = {std::sin(theta) * std::cos(phi),
                      std::sin(theta) * std::sin(phi), std::cos(theta)};

      shape.position.push_back(center + R * p);
      shape.uv.push_back({u, v});
    }
  }

  for (int ku = 0; ku < Nu; ku++) {
    for (int kv = 0; kv < Nv; kv++) {
      const int k00 = kv + Nv * ku;
      const int k10 = ((kv + 1) % Nv) + Nv * ku;
      const int k01 = kv + Nv * ((ku + 1) % Nu);
      const int k11 = ((kv + 1) % Nv) + Nv * ((ku + 1) % Nu);

      shape.connectivity.push_back({k00, k10, k01});
      shape.connectivity.push_back({k10, k11, k01});
    }

    const int k0N = Nv - 1 + Nv * ku;
    const int k1N = Nv - 1 + Nv * (ku + 1);
  }

  shape.fill_empty_field();

  sphere.initialize_data_on_gpu(shape);
  sphere.model.scaling = 3.0f;
  sphere.material.color = {0.156f, 0.65f, 0.27f};
}

void PerfectSphere::update() {
  sphere.model.translation = {0, 0, 0};
}

void PerfectSphere::render() {
  draw(sphere, scene->environment);
}

void PerfectSphere::render_debug() {
  draw_wireframe(sphere, scene->environment);
}
