#include "scene.hpp"
#include "objects/Planet.hpp"
#include "objects/Sun.hpp"
#include "objects/space_ship/SpaceShip.hpp"

using namespace cgp;

// This function is called only once at the beginning of the program
// This function can contain any complex operation that can be pre-computed once
void scene_structure::initialize() {
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
  objects.push_back(std::unique_ptr<Object>(space_ship));

  Sun* sun = new Sun(this);
  objects.push_back(std::unique_ptr<Object>(sun));
  celestial_bodies.push_back(std::unique_ptr<CelestialBody>(sun));
  collision_bodies.push_back(std::unique_ptr<CollisionBody>(sun));

  Planet* planet = new Planet(this, *sun, {100, 10, 0});
  objects.push_back(std::unique_ptr<Object>(planet));

  Planet* planet2 = new Planet(this, *sun, {50, 10, 0});
  objects.push_back(std::unique_ptr<Object>(planet2));
}

// This function is called permanently at every new frame
// Note that you should avoid having costly computation and large allocation
// defined there. This function is mostly used to call the draw() functions on
// pre-existing data.
void scene_structure::display_frame() {
  // Update time
  timer.update();

  // conditional display of the global frame (set via the GUI)
  if (gui.display_frame)
    draw(global_frame, environment);

  for (auto& object : objects) {
    object->update();
  }

  for (auto& object : objects) {
    object->render();
  }

  if (gui.display_wireframe) {
    for (auto& object : objects) {
      object->render_debug();
    }
  }
}

void scene_structure::display_gui() {
  ImGui::Checkbox("Frame", &gui.display_frame);
  ImGui::Checkbox("Wireframe", &gui.display_wireframe);
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
