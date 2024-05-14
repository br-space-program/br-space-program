#include "primitives.hpp"

using cgp::norm;
using cgp::Pi;

mesh create_sphere_mesh(float R, vec3 center, int Nu, int Nv) {
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

      if (norm(shape.position[k00] - shape.position[k11]) < 1e-6f) {
        continue;
      }
      if (norm(shape.position[k10] - shape.position[k01]) < 1e-6f) {
        continue;
      }
      if (norm(shape.position[k00] - shape.position[k10]) < 1e-6f) {
        continue;
      }
      if (norm(shape.position[k10] - shape.position[k11]) < 1e-6f) {
        continue;
      }
      if (norm(shape.position[k11] - shape.position[k01]) < 1e-6f) {
        continue;
      }
      if (norm(shape.position[k01] - shape.position[k00]) < 1e-6f) {
        continue;
      }

      shape.connectivity.push_back({k00, k10, k01});
      shape.connectivity.push_back({k10, k11, k01});
    }
  }

  shape.fill_empty_field();

  return shape;
}
