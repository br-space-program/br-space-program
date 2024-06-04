#include "Planet.hpp"
#include <cgp/cgp.hpp>
#include "../constants.hpp"
#include "../mesh/primitives.hpp"

using cgp::mesh_drawable;

Planet::Planet(scene_structure* _scene,
               CelestialBody& _anchor,
               vec3 _position,
               double radius,
               vec3 color)
    : KeplerianBody(_anchor,
                    _position,
                    PLANET_DENSITY * pow(radius, 3),
                    radius),
      scene(_scene) {
  int Nu = 100;
  int Nv = 70;

  mesh shape = create_sphere_mesh(radius, {0, 0, 0}, Nu, Nv);

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

  sphere.initialize_data_on_gpu(shape);
  sphere.model.scaling = 1.0f;
  sphere.material.color = color;

  sphere.vbo_position.update(shape.position);
  sphere.vbo_normal.update(shape.normal);
  sphere.vbo_color.update(shape.color);

  // ==== Atmoshpere ====
  atmosphere = new SimpleObject(scene);

  atmosphere->mesh.initialize_data_on_gpu(mesh_primitive_sphere(radius * 5));
  atmosphere->mesh.shader = scene->shader_glow;
  atmosphere->mesh.material.color = {1, 1, 1};
  atmosphere->mesh.material.alpha = 0.3;

  atmosphere->set_position(position);
}

void Planet::update() {
  KeplerianBody::update();
  sphere.model.translation = position;
  atmosphere->set_position(position);
  atmosphere->update();
}

void Planet::render() {
  draw(sphere, scene->environment);
}

void Planet::render_debug() {
  draw_wireframe(sphere, scene->environment);
}
