#include "planet.hpp"
#include "primitives.hpp"

cgp::mesh create_planet_mesh(float R, cgp::vec3 center, int Nu, int Nv) {
  cgp::mesh shape = create_sphere_mesh(R, {0, 0, 0}, Nu, Nv);

  shape.fill_empty_field();

  int octave = 4;
  float persistency = 0.5f;
  float frequency_gain = 2.0f;
  float terrain_height = 0.1f;

  for (int ku = 0; ku < Nu; ++ku) {
    for (int kv = 0; kv < Nv; ++kv) {
      int const idx = ku * Nv + kv;

      // Compute the Perlin noise
      float const noise = noise_perlin(shape.position[idx], octave, persistency,
                                       frequency_gain);

      // use the noise as height value
      shape.position[idx].x *= (1 + terrain_height * noise);
      shape.position[idx].y *= (1 + terrain_height * noise);
      shape.position[idx].z *= (1 + terrain_height * noise);

      // use also the noise as color value
      shape.color[idx] = 0.3f * vec3(0, 0.5f, 0) + 0.7f * noise * vec3(1, 1, 1);
    }
  }

  shape.normal_update();

  return shape;
}
