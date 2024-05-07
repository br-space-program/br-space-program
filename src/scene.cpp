#include "scene.hpp"
#include "objects/ExampleObject.hpp"
#include "objects/Sun.hpp"

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
  camera_control.look_at(
      {20.0f, 0.0f, 0.0f} /* position of the camera in the 3D scene */,
      {0, 0, 0} /* targeted point in 3D scene */,
      {0, 0, 1} /* direction of the "up" vector */);

  display_info();

  // Load the custom shader
  shader_custom.load(
      project::path + "shaders/shading_custom/shading_custom.vert.glsl",
      project::path + "shaders/shading_custom/shading_custom.frag.glsl");

  // Create the global (x,y,z) frame
  global_frame.initialize_data_on_gpu(mesh_primitive_frame());

  // Set up objects here
  // objects.push_back(std::unique_ptr<Object>(new ExampleObject(this)));
  objects.push_back(std::unique_ptr<Object>(new Sun(this)));
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
