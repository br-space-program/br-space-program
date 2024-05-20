#include "Tesseract.hpp"
#include <cgp/cgp.hpp>
#include "../mesh/primitives.hpp"
#include "../scene.hpp"

using cgp::mesh_drawable;

mesh mesh_primitive_open_box(vec3 const& center, float edge_length) {
  vec3 u = edge_length * vec3{1, 1, 1};
  vec3 p000 = center - u / 2.0f;
  vec3 p100 = p000 + u * vec3{1, 0, 0};
  vec3 p110 = p000 + u * vec3{1, 1, 0};
  vec3 p010 = p000 + u * vec3{0, 1, 0};
  vec3 p001 = p000 + u * vec3{0, 0, 1};
  vec3 p101 = p000 + u * vec3{1, 0, 1};
  vec3 p111 = p000 + u * vec3{1, 1, 1};
  vec3 p011 = p000 + u * vec3{0, 1, 1};

  mesh shape;
  shape.push_back(mesh_primitive_quadrangle(p000, p100, p101, p001));
  shape.push_back(mesh_primitive_quadrangle(p100, p110, p111, p101));
  shape.push_back(mesh_primitive_quadrangle(p110, p010, p011, p111));
  shape.push_back(mesh_primitive_quadrangle(p010, p000, p001, p011));
  // shape.push_back(mesh_primitive_quadrangle(p001, p101, p111, p011));
  shape.push_back(mesh_primitive_quadrangle(p100, p000, p010, p110));

  return shape;
}

mesh mesh_primitive_cuboid(vec3 const& p000, vec3 const& p111) {
  vec3 p100 = {p111.x, p000.y, p000.z};
  vec3 p010 = {p000.x, p111.y, p000.z};
  vec3 p001 = {p000.x, p000.y, p111.z};
  vec3 p110 = {p111.x, p111.y, p000.z};
  vec3 p101 = {p111.x, p000.y, p111.z};
  vec3 p011 = {p000.x, p111.y, p111.z};

  mesh shape;
  shape.push_back(mesh_primitive_quadrangle(p000, p100, p101, p001));
  shape.push_back(mesh_primitive_quadrangle(p100, p110, p111, p101));
  shape.push_back(mesh_primitive_quadrangle(p110, p010, p011, p111));
  shape.push_back(mesh_primitive_quadrangle(p010, p000, p001, p011));
  shape.push_back(mesh_primitive_quadrangle(p001, p101, p111, p011));
  shape.push_back(mesh_primitive_quadrangle(p100, p000, p010, p110));

  return shape;
}

Tesseract::Tesseract(scene_structure* _scene, vec3 _position, double _scale)
    : scene(_scene), position(_position), scale(_scale) {
  mesh box_sides_mesh = mesh_primitive_open_box({0, 0, 0}, 1.0f);
  box_sides.initialize_data_on_gpu(box_sides_mesh);
  box_sides.material.color = {0.219f, 0.286f, 0.827f};
  box_sides.model.scaling = scale;

  frame_thickness = 0.05;

  mesh box_interface_mesh = mesh_primitive_quadrangle(
      {-0.5f + frame_thickness, -0.5f + frame_thickness,
       0.5f - frame_thickness},
      {0.5f - frame_thickness, -0.5f + frame_thickness, 0.5f - frame_thickness},
      {0.5f - frame_thickness, 0.5f - frame_thickness, 0.5f - frame_thickness},
      {-0.5f + frame_thickness, 0.5f - frame_thickness,
       0.5f - frame_thickness});
  box_interface.initialize_data_on_gpu(box_interface_mesh);
  box_interface.material.color = {1.0f, 0.301f, 0.313f};
  box_interface.model.scaling = scale;

  mesh sphere_mesh = mesh_primitive_sphere(0.1f, {0.5f, 0, 0});
  sphere.initialize_data_on_gpu(sphere_mesh);
  sphere.material.color = {1.0f, 1.0f, 0};
  sphere.model.scaling = scale;

  mesh_drawable frame_root, frame_origin, frame_part;
  mesh frame_part_mesh = mesh_primitive_cuboid(
      {-frame_thickness, -frame_thickness, -frame_thickness},
      {frame_thickness, frame_thickness, 1.f + frame_thickness});
  frame_part.initialize_data_on_gpu(frame_part_mesh);
  frame_part.material.color = {1.0f, 1.0f, 1.0f};
  frame_part.model.scaling = 1.0f;

  box_frame.add(frame_root, "root");
  affine_rts box_frame_scale = affine_rts().set_scaling(scale);
  box_frame.add(frame_origin, "origin", "root", box_frame_scale);
  box_frame.add(frame_part, "part1", "origin", {-0.5f, -0.5f, -0.5f});
  box_frame.add(frame_part, "part2", "origin", {0.5f, -0.5f, -0.5f});
  box_frame.add(frame_part, "part3", "origin", {0.5f, 0.5f, -0.5f});
  box_frame.add(frame_part, "part4", "origin", {-0.5f, 0.5f, -0.5f});
  affine_rts box_frame_rotation1 = affine_rts().set_rotation(
      rotation_transform::from_axis_angle({0, 1, 0}, 3.14f / 2));
  box_frame.add(frame_origin, "origin2", "origin", box_frame_rotation1);
  box_frame.add(frame_part, "part5", "origin2", {-0.5f, -0.5f, -0.5f});
  box_frame.add(frame_part, "part6", "origin2", {0.5f, -0.5f, -0.5f});
  box_frame.add(frame_part, "part7", "origin2", {0.5f, 0.5f, -0.5f});
  box_frame.add(frame_part, "part8", "origin2", {-0.5f, 0.5f, -0.5f});
  affine_rts box_frame_rotation2 = affine_rts().set_rotation(
      rotation_transform::from_axis_angle({1, 0, 0}, 3.14f / 2));
  box_frame.add(frame_origin, "origin3", "origin2", box_frame_rotation2);
  box_frame.add(frame_part, "part9", "origin3", {-0.5f, -0.5f, -0.5f});
  box_frame.add(frame_part, "part10", "origin3", {0.5f, -0.5f, -0.5f});
  box_frame.add(frame_part, "part11", "origin3", {0.5f, 0.5f, -0.5f});
  box_frame.add(frame_part, "part12", "origin3", {-0.5f, 0.5f, -0.5f});
}

bool Tesseract::is_inside_tesseract(vec3 const& p) const {
  const vec3 a = (p - position) / scale;

  return std::abs(a.x) <= 0.5 && std::abs(a.y) <= 0.5 && -0.5 <= a.z &&
         a.z <= 0.5 - frame_thickness;
}

void Tesseract::update() {
  box_sides.model.translation = position;
  box_interface.model.translation = position;
  sphere.model.translation = position;
  box_frame["root"].transform_local.translation = position;
  box_frame.update_local_to_global_coordinates();
}

void Tesseract::render() {
  bool outside = !is_inside_tesseract(scene->space_ship->get_position());

  std::cout << "Is spaceship outside tesseract? " << outside << std::endl;

  draw(box_frame, scene->environment);

  glEnable(GL_STENCIL_TEST);

  // Draw the box interface
  glStencilFunc(GL_ALWAYS, 1, 0xFF);
  glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
  glStencilMask(0xFF);
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glDepthMask(GL_FALSE);
  glClear(GL_STENCIL_BUFFER_BIT);
  draw(box_interface, scene->environment);

  // Draw the sphere
  glStencilFunc(GL_EQUAL, 1, 0xFF);
  glStencilMask(0x00);
  glDepthMask(GL_TRUE);
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  if (outside) {
    draw(sphere, scene->environment);
  }

  // Draw the sides
  glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
  if (outside) {
    draw(box_sides, scene->environment);
  } else {
    draw(sphere, scene->environment);
  }

  glDisable(GL_STENCIL_TEST);
}

void Tesseract::render_debug() {
  draw_wireframe(box_sides, scene->environment);
  draw_wireframe(box_interface, scene->environment);
  draw_wireframe(sphere, scene->environment);
}
