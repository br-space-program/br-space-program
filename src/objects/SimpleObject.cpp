#include "SimpleObject.hpp"
#include "cgp/cgp.hpp"

SimpleObject::SimpleObject(scene_structure* _scene) {
  // Constructor
  scene = _scene;
}

void SimpleObject::render() {
  // Render the object
  draw(mesh, scene->environment);
}

void SimpleObject::render_debug() {
  // Render the object's debug information
  draw_wireframe(mesh, scene->environment);
}

void SimpleObject ::set_position(vec3 _position) {
  position = _position;
}

void SimpleObject::update() {
  // Update the object
  mesh.model.translation = position;
}