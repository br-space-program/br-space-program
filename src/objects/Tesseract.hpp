#pragma once

struct scene_structure;

#include "Object.hpp"
#include "cgp/cgp.hpp"

using cgp::hierarchy_mesh_drawable;
using cgp::mesh_drawable;
using cgp::vec3;

class Tesseract : public Object {
 private:
  scene_structure* scene;
  mesh_drawable box_sides;
  mesh_drawable box_interface;
  mesh_drawable sphere;
  hierarchy_mesh_drawable box_frame;
  vec3 position;  // center of the tesseract
  double scale;
  double frame_thickness;

  bool is_inside_tesseract(vec3 const& p) const;

 public:
  Tesseract(scene_structure* _scene, vec3 _position, double _scale);
  void update() override;
  void render() override;
  void render_debug() override;
  ~Tesseract() override = default;
};
