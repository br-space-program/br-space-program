#include "scene.hpp"
#include <algorithm>
#include "objects/PerfectSphere.hpp"
#include "objects/Planet.hpp"
#include "objects/Sun.hpp"
#include "objects/Tesseract.hpp"
#include "objects/space_ship/SpaceShip.hpp"

using namespace cgp;
using namespace std;

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

  // Load the skybox
  image_structure image_skybox =
      image_load_file(project::path + "assets/skybox.png");

  std::vector<image_structure> image_skybox_grid =
      image_split_grid(image_skybox, 4, 3);

  skybox.initialize_data_on_gpu();
  skybox.texture.initialize_cubemap_on_gpu(
      image_skybox_grid[1], image_skybox_grid[7], image_skybox_grid[5],
      image_skybox_grid[3], image_skybox_grid[10], image_skybox_grid[4]);

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

  World* world1 = new World(this);

  Sun* sun = new Sun(this);
  world1->add_celestial_body(std::unique_ptr<CelestialBody>(sun));
  world1->add_object(std::unique_ptr<Object>(sun));
  world1->add_hitbox(std::unique_ptr<ObjectWithHitbox>(sun));

  Planet* planet = new Planet(this, *sun, {100, 10, 0}, 1);
  world1->add_celestial_body(std::unique_ptr<CelestialBody>(planet));
  world1->add_object(std::unique_ptr<Object>(planet));
  world1->add_hitbox(std::unique_ptr<ObjectWithHitbox>(planet));

  Planet* planet2 = new Planet(this, *sun, {50, 10, 0}, 10);
  world1->add_celestial_body(std::unique_ptr<CelestialBody>(planet2));
  world1->add_object(std::unique_ptr<Object>(planet2));
  world1->add_hitbox(std::unique_ptr<ObjectWithHitbox>(planet2));

  // === Atmospheres and glow ===
  world1->add_transparent_object(std::unique_ptr<Object>(sun->atmosphere));
  world1->add_transparent_object(std::unique_ptr<Object>(planet->atmosphere));
  world1->add_transparent_object(std::unique_ptr<Object>(planet2->atmosphere));

  World* world2 = new World(this);

  PerfectSphere* sphere = new PerfectSphere(this, {100, 5, -5}, 1);
  world2->add_object(std::unique_ptr<Object>(sphere));

  PerfectSphere* sphere2 = new PerfectSphere(this, {100, 0, 5}, 1);
  world1->add_object(std::unique_ptr<Object>(sphere2));

  worlds[0] = world1;
  worlds[1] = world2;

  tesseract = new Tesseract(this, worlds, {100, 0, 0}, 3);
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
  glDepthMask(GL_FALSE);  // disable depth-buffer writing
  draw(skybox, environment);
  glDepthMask(GL_TRUE);  // re-activate depth-buffer write

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
  space_ship->render();
  tesseract->render();

  // == Render transparent objects ==

  // Step 1: collect
  std::vector<Object*> transparent_objects;
  World* world = tesseract->get_active_world();
  if (world != nullptr) {
    for (auto& object : world->get_transparent_objects()) {
      transparent_objects.push_back(object.get());
    }
  }
  for (auto& ship_flame : space_ship->ship_flames) {
    transparent_objects.push_back(ship_flame->flare);
  }

  // Step 2: sort
  // TODO: Render closer transparent objects first
  vec3 camera_position = camera_control.camera_model.position();
  std::sort(transparent_objects.begin(), transparent_objects.end(),
            [&camera_position](const Object* a, const Object* b) {
              return norm(a->get_position() - camera_position) >
                     norm(b->get_position() - camera_position);
            });

  // Step 3: render
  for (auto& object : transparent_objects) {
    object->render();
  }

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
