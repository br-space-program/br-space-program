#include "scene.hpp"
#include <algorithm>
#include "objects/PerfectSphere.hpp"
#include "objects/Planet.hpp"
#include "objects/Sun.hpp"
#include "objects/Tesseract.hpp"
#include "objects/space_ship/SpaceShip.hpp"
#include "worlds/generic.hpp"
#include "worlds/giedi-prime.hpp"

using namespace cgp;
using namespace std;

// This function is called only once at the beginning of the program
// This function can contain any complex operation that can be pre-computed once
void scene_structure::initialize() {
  dt = (1.0f / 60.0f);

  environment.background_color = {0, 0, 0};
  // Set the behavior of the camera and its initial position
  // ********************************************** //
  camera_control.initialize(inputs, window);
  camera_control.set_rotation_axis_z();  // camera rotates around z-axis
  //   look_at(camera_position, targeted_point, up_direction)

  display_info();

  // Load the custom shader
  shader_custom.load(
      project::path + "shaders/shading_custom/shading_custom.vert.glsl",
      project::path + "shaders/shading_custom/shading_custom.frag.glsl");

  shader_glow.load(project::path + "shaders/shader_glow/shader_glow.vert.glsl",
                   project::path + "shaders/shader_glow/shader_glow.frag.glsl");

  // Create the global (x,y,z) frame
  global_frame.initialize_data_on_gpu(mesh_primitive_frame());

  // Set up objects here

  space_ship = new SpaceShip(this);

  // =/=/=/=/=/=/=/= SET WORLDS =/=/=/=/=/=/=/=

  worlds[SIDE_POS_Z] = create_world_generic1(this, {-200, 0, 400});
  worlds[SIDE_NEG_Z] = create_world_generic2(this, {0, 0, -800});
  worlds[SIDE_POS_X] = create_world_homeworld_harkonen(this, {-400, 0, 0});
  worlds[SIDE_NEG_X] = create_world_generic3(this, {0, 200, 0});
  worlds[SIDE_POS_Y] = create_world_empty(this);
  worlds[SIDE_NEG_Y] = create_world_empty(this);

  tesseract = new Tesseract(this, worlds, {0, 0, 0}, 15);
}

// This function is called permanently at every new frame
// Note that you should avoid having costly computation and large allocation
// defined there. This function is mostly used to call the draw() functions on
// pre-existing data.
void scene_structure::display_frame() {
  // Update time
  timer.update();

  //  Must be called before drawing the other shapes and without writing in the
  //  Depth Buffer

  // conditional display of the global frame (set via the GUI)
  if (gui.display_frame)
    draw(global_frame, environment);

  // Update the objects
  for (auto world : worlds) {
    if (world != nullptr) {
      world->update();
    }
  }
  space_ship->update();
  tesseract->update();

  // Display the objects
  tesseract->render();

  if (gui.display_wireframe) {
    for (auto world : worlds) {
      if (world != nullptr) {
        world->render_debug();
      }
    }
    space_ship->render_debug();
    tesseract->render_debug();
  }
}

void scene_structure::display_gui() {
  ImGui::Checkbox("Frame", &gui.display_frame);
  ImGui::Checkbox("Wireframe", &gui.display_wireframe);
  ImGui::Checkbox("Debug Movements", &debug_movements);
  ImGui::Checkbox("Locked camera", &space_ship->camera_locked);
  ImGui::SliderFloat("Time step", &dt, 1.f / 500, 1.f / 24);
}

void scene_structure::mouse_move_event() {
  if (!inputs.keyboard.shift)
    camera_control.action_mouse_move(environment.camera_view);
}
void scene_structure::mouse_click_event() {
  camera_control.action_mouse_click(environment.camera_view);
}
void scene_structure::keyboard_event() {
  space_ship->action_keyboard();
  camera_control.action_keyboard(environment.camera_view);
}
void scene_structure::idle_frame() {
  camera_control.idle_frame(environment.camera_view);
}

void scene_structure::display_info() {
  std::cout << "\nCAMERA CONTROL:" << std::endl;
  std::cout << "-----------------------------------------------" << std::endl;
  std::cout << camera_control.doc_usage() << std::endl;
  std::cout << "-----------------------------------------------\n" << std::endl;

  std::cout << "\nSCENE INFO:" << std::endl;
  std::cout << "-----------------------------------------------" << std::endl;
  std::cout << "Binet Réseau Space Program" << std::endl;
  std::cout << "-----------------------------------------------\n" << std::endl;
}
