#pragma once

struct scene_structure;

#include "../World.hpp"
#include "Object.hpp"
#include "cgp/cgp.hpp"
#define SIDES_COUNT 6

using cgp::hierarchy_mesh_drawable;
using cgp::mesh_drawable;
using cgp::vec3;

enum tesseract_side {
  SIDE_POS_Z,
  SIDE_NEG_Z,
  SIDE_POS_Y,
  SIDE_NEG_Y,
  SIDE_POS_X,
  SIDE_NEG_X
};

class Tesseract : public Object {
 private:
  World* worlds[SIDES_COUNT];
  scene_structure* scene;
  mesh_drawable box_sides[SIDES_COUNT];
  mesh_drawable box_interfaces[SIDES_COUNT];
  mesh_drawable sphere;
  hierarchy_mesh_drawable box_frame;
  vec3 position;  // center of the tesseract
  double scale;
  double frame_thickness;
  tesseract_side current_world;  // world currently considered as "outside"

  void initialize_box_sides();
  void initialize_box_interfaces();
  void initialize_box_frame();
  bool is_inside_tesseract(vec3 const& p) const;
  void draw_world_through_interface(tesseract_side side);
  void draw_tesseract_content();
  void draw_external_sides();
  void render_inside_tesseract();
  void render_outside_tesseract();
  bool is_space_ship_looking_at_current_interface();

 public:
  Tesseract(scene_structure* _scene,
            World* _worlds[SIDES_COUNT],
            vec3 _position,
            double _scale);
  void update() override;
  void render() override;
  void render_debug() override;
  World* get_active_world();
  ~Tesseract() override = default;
};
