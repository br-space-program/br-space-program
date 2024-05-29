#include "Tesseract.hpp"
#include <cgp/cgp.hpp>
#include "../World.hpp"
#include "../mesh/primitives.hpp"
#include "../scene.hpp"

using cgp::mesh_drawable;

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

Tesseract::Tesseract(scene_structure* _scene,
                     World* _worlds[SIDES_COUNT],
                     vec3 _position,
                     double _scale)
    : scene(_scene),
      position(_position),
      scale(_scale),
      frame_thickness(0.05),
      current_world(SIDE_POS_Z) {
  for (int i = 0; i < SIDES_COUNT; i++) {
    worlds[i] = _worlds[i];
  }

  mesh sphere_mesh = mesh_primitive_sphere(0.1f, {0.5f, 0, 0});
  sphere.initialize_data_on_gpu(sphere_mesh);
  sphere.material.color = {1.0f, 1.0f, 0};
  sphere.model.scaling = scale;

  initialize_box_sides();
  initialize_box_interfaces();
  initialize_box_frame();

  sphere.shader = scene->shader_custom;
  for (int i = 0; i < SIDES_COUNT; i++) {
    box_sides[i].shader = scene->shader_custom;
  }
  for (auto& frame_element : box_frame.elements) {
    frame_element.drawable.shader = scene->shader_custom;
  }
}

void Tesseract::initialize_box_sides() {
  vec3 u = vec3{1, 1, 1};
  vec3 p000 = -u / 2.0f;
  vec3 p100 = p000 + u * vec3{1, 0, 0};
  vec3 p110 = p000 + u * vec3{1, 1, 0};
  vec3 p010 = p000 + u * vec3{0, 1, 0};
  vec3 p001 = p000 + u * vec3{0, 0, 1};
  vec3 p101 = p000 + u * vec3{1, 0, 1};
  vec3 p111 = p000 + u * vec3{1, 1, 1};
  vec3 p011 = p000 + u * vec3{0, 1, 1};

  box_sides[SIDE_POS_Z].initialize_data_on_gpu(
      mesh_primitive_quadrangle(p001, p101, p111, p011));
  box_sides[SIDE_NEG_Z].initialize_data_on_gpu(
      mesh_primitive_quadrangle(p100, p000, p010, p110));
  box_sides[SIDE_POS_Y].initialize_data_on_gpu(
      mesh_primitive_quadrangle(p110, p010, p011, p111));
  box_sides[SIDE_NEG_Y].initialize_data_on_gpu(
      mesh_primitive_quadrangle(p000, p100, p101, p001));
  box_sides[SIDE_POS_X].initialize_data_on_gpu(
      mesh_primitive_quadrangle(p100, p110, p111, p101));
  box_sides[SIDE_NEG_X].initialize_data_on_gpu(
      mesh_primitive_quadrangle(p010, p000, p001, p011));

  for (int i = 0; i < SIDES_COUNT; i++) {
    box_sides[i].material.color = {0.219f, 0.286f, 0.827f};
    box_sides[i].model.scaling = scale;
  }
}

void Tesseract::initialize_box_interfaces() {
  mesh box_interface_xy_mesh = mesh_primitive_quadrangle(
      {-0.5f + frame_thickness, -0.5f + frame_thickness,
       0.5f - frame_thickness},
      {0.5f - frame_thickness, -0.5f + frame_thickness, 0.5f - frame_thickness},
      {0.5f - frame_thickness, 0.5f - frame_thickness, 0.5f - frame_thickness},
      {-0.5f + frame_thickness, 0.5f - frame_thickness,
       0.5f - frame_thickness});
  mesh box_interface_xz_mesh = mesh_primitive_quadrangle(
      {-0.5f + frame_thickness, 0.5f - frame_thickness,
       -0.5f + frame_thickness},
      {0.5f - frame_thickness, 0.5f - frame_thickness, -0.5f + frame_thickness},
      {0.5f - frame_thickness, 0.5f - frame_thickness, 0.5f - frame_thickness},
      {-0.5f + frame_thickness, 0.5f - frame_thickness,
       0.5f - frame_thickness});
  mesh box_interface_yz_mesh = mesh_primitive_quadrangle(
      {0.5f - frame_thickness, -0.5f + frame_thickness,
       -0.5f + frame_thickness},
      {0.5f - frame_thickness, 0.5f - frame_thickness, -0.5f + frame_thickness},
      {0.5f - frame_thickness, 0.5f - frame_thickness, 0.5f - frame_thickness},
      {0.5f - frame_thickness, -0.5f + frame_thickness,
       0.5f - frame_thickness});

  for (int i = 0; i < SIDES_COUNT; i++) {
    mesh& box_interface_mesh = box_interface_xy_mesh;

    switch (i) {
      case SIDE_POS_Z:
      case SIDE_NEG_Z:
        box_interface_mesh = box_interface_xy_mesh;
        break;
      case SIDE_POS_Y:
      case SIDE_NEG_Y:
        box_interface_mesh = box_interface_xz_mesh;
        break;
      case SIDE_POS_X:
      case SIDE_NEG_X:
        box_interface_mesh = box_interface_yz_mesh;
        break;
    }

    box_interfaces[i].initialize_data_on_gpu(box_interface_mesh);
    box_interfaces[i].material.color = {1.0f, 0.286f, 1.0f};
    box_interfaces[i].model.scaling = scale;
  }
}

void Tesseract::initialize_box_frame() {
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
  box_sides[SIDE_POS_Z].model.translation = position;
  box_sides[SIDE_NEG_Z].model.translation = position;
  box_sides[SIDE_POS_Y].model.translation = position;
  box_sides[SIDE_NEG_Y].model.translation = position;
  box_sides[SIDE_POS_X].model.translation = position;
  box_sides[SIDE_NEG_X].model.translation = position;

  double interface_unit_size = 1.0f - 2 * frame_thickness;
  box_interfaces[SIDE_POS_Z].model.translation = position;
  box_interfaces[SIDE_NEG_Z].model.translation =
      position - vec3{0, 0, interface_unit_size} * scale;
  box_interfaces[SIDE_POS_Y].model.translation = position;
  box_interfaces[SIDE_NEG_Y].model.translation =
      position - vec3{0, interface_unit_size, 0} * scale;
  box_interfaces[SIDE_POS_X].model.translation = position;
  box_interfaces[SIDE_NEG_X].model.translation =
      position - vec3{interface_unit_size, 0, 0} * scale;

  sphere.model.translation = position;
  box_frame["root"].transform_local.translation = position;
  box_frame.update_local_to_global_coordinates();

  // detect if space ship is close to an interface
  // and update "current_world" accordingly
  vec3 ship_position = scene->space_ship->get_position();
  vec3 center = position;
  vec3 ship_position_relative = ship_position - center;

  if (is_inside_tesseract(ship_position)) {
    tesseract_side z = ship_position_relative.z > 0 ? SIDE_POS_Z : SIDE_NEG_Z;
    tesseract_side y = ship_position_relative.y > 0 ? SIDE_POS_Y : SIDE_NEG_Y;
    tesseract_side x = ship_position_relative.x > 0 ? SIDE_POS_X : SIDE_NEG_X;

    // computing dominant side
    tesseract_side dominant_side = z;
    if (std::abs(ship_position_relative.y) >
        std::abs(ship_position_relative.z)) {
      dominant_side = y;
    }
    if (std::abs(ship_position_relative.x) >
            std::abs(ship_position_relative.y) &&
        std::abs(ship_position_relative.x) >
            std::abs(ship_position_relative.z)) {
      dominant_side = x;
    }

    current_world = dominant_side;
  }
}

void Tesseract::draw_world_through_interface(tesseract_side side) {
  if (worlds[side] == nullptr) {
    return;
  }

  glStencilFunc(GL_EQUAL, 1, 0xFF);
  glStencilOp(GL_KEEP, GL_INCR, GL_INCR);
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glDepthMask(GL_FALSE);
  draw(box_interfaces[side], scene->environment);

  glStencilFunc(GL_EQUAL, 2, 0xFF);
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  glDepthMask(GL_TRUE);
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  worlds[side]->render();
  worlds[side]->render_transparent(false);

  glStencilFunc(GL_EQUAL, 2, 0xFF);
  glStencilOp(GL_KEEP, GL_DECR, GL_DECR);
  glDepthMask(GL_FALSE);
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  draw(box_interfaces[side], scene->environment);  // any draw() operation
}

void Tesseract::draw_tesseract_content() {
  draw(sphere, scene->environment);
}

void Tesseract::render_inside_tesseract() {
  scene->environment.uniform_generic.uniform_vec3["light"] = position;

  draw(box_frame, scene->environment);

  draw_tesseract_content();

  glEnable(GL_STENCIL_TEST);
  glClear(GL_STENCIL_BUFFER_BIT);

  // set stencil buffer all to one
  glStencilMask(0xFF);
  glStencilFunc(GL_ALWAYS, 1, 0xFF);
  glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glDepthMask(GL_FALSE);
  for (int i = 0; i < SIDES_COUNT; i++) {
    draw(box_interfaces[i], scene->environment);
  }

  // Draw worlds through interfaces
  for (int i = 0; i < SIDES_COUNT; i++) {
    draw_world_through_interface((tesseract_side)i);
  }

  glDepthMask(GL_TRUE);
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glDisable(GL_STENCIL_TEST);
}

void Tesseract::draw_external_sides() {
  for (int i = 0; i < SIDES_COUNT; i++) {
    if (i == current_world) {
      continue;
    }

    draw(box_sides[i], scene->environment);
  }
}

bool Tesseract::is_space_ship_looking_at_current_interface() {
  vec3 normal;
  switch (current_world) {
    case SIDE_POS_Z:
      normal = vec3{0, 0, 1};
      break;
    case SIDE_NEG_Z:
      normal = vec3{0, 0, -1};
      break;
    case SIDE_POS_Y:
      normal = vec3{0, 1, 0};
      break;
    case SIDE_NEG_Y:
      normal = vec3{0, -1, 0};
      break;
    case SIDE_POS_X:
      normal = vec3{1, 0, 0};
      break;
    case SIDE_NEG_X:
      normal = vec3{-1, 0, 0};
      break;
  }

  vec3 center_interface = position + normal * 0.5f * scale;
  vec3 camera_position = scene->camera_control.camera_model.position();

  vec3 view_vector = camera_position - center_interface;

  double scalar_product = cgp::dot(view_vector, normal);
  return scalar_product > 0;
}

void Tesseract::render_outside_tesseract() {
  glEnable(GL_STENCIL_TEST);
  glClear(GL_STENCIL_BUFFER_BIT);
  glStencilMask(0xFF);

  bool render_external_sides_first =
      !is_space_ship_looking_at_current_interface();

  std::cout << "Is spaceship looking at current interface? "
            << render_external_sides_first << std::endl;

  scene->environment.uniform_generic.uniform_vec3["light"] = position;

  // Draw the external sides first
  if (render_external_sides_first) {
    glStencilFunc(GL_LEQUAL, 0, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_DECR_WRAP);
    glDepthMask(GL_TRUE);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    draw(box_frame, scene->environment);
    draw_external_sides();
  }

  // Draw the box interface
  glStencilFunc(GL_EQUAL, 0, 0xFF);
  glStencilOp(GL_KEEP, GL_INCR, GL_INCR);
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glDepthMask(GL_FALSE);
  draw(box_interfaces[current_world], scene->environment);

  glStencilFunc(GL_EQUAL, 1, 0xFF);
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  glDepthMask(GL_TRUE);
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  draw_tesseract_content();

  // Draw other worlds
  for (int i = 0; i < SIDES_COUNT; i++) {
    if (i == current_world) {
      continue;
    }

    draw_world_through_interface((tesseract_side)i);
  }

  scene->environment.uniform_generic.uniform_vec3["light"] = position;

  if (!render_external_sides_first) {
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glDepthMask(GL_TRUE);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    draw(box_frame, scene->environment);
  }

  glStencilFunc(GL_ALWAYS, 1, 0xFF);
  glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glDepthMask(GL_TRUE);
  draw(box_interfaces[current_world], scene->environment);

  // Draw the external sides last
  if (!render_external_sides_first) {
    glStencilFunc(GL_LEQUAL, 0, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_DECR_WRAP);
    glDepthMask(GL_TRUE);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    draw_external_sides();
  }

  glDepthMask(GL_TRUE);
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glDisable(GL_STENCIL_TEST);

  // Draw the current world
  if (worlds[current_world] != nullptr) {
    worlds[current_world]->render();
    worlds[current_world]->render_transparent(true);
  }
}

void Tesseract::render() {
  bool outside = !is_inside_tesseract(scene->space_ship->get_position());

  // std::cout << "Is spaceship outside tesseract? " << outside << std::endl;

  if (outside) {
    render_outside_tesseract();
  } else {
    render_inside_tesseract();
  }
}

void Tesseract::render_debug() {
  for (int i = 0; i < SIDES_COUNT; i++) {
    draw_wireframe(box_sides[i], scene->environment);
  }
  for (int i = 0; i < SIDES_COUNT; i++) {
    draw_wireframe(box_interfaces[i], scene->environment);
  }
  draw_wireframe(sphere, scene->environment);
}

World* Tesseract::get_active_world() {
  if (is_inside_tesseract(scene->space_ship->get_position())) {
    return nullptr;
  }

  return worlds[current_world];
}
