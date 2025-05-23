#pragma once

struct scene_structure;

#include <vector>
#include "World.hpp"
#include "cgp/cgp.hpp"
#include "environment.hpp"
#include "objects/Tesseract.hpp"
#include "objects/space_ship/SpaceShip.hpp"

// This definitions allow to use the structures: mesh, mesh_drawable, etc.
// without mentionning explicitly cgp::
using cgp::mesh;
using cgp::mesh_drawable;
using cgp::numarray;
using cgp::timer_basic;
using cgp::vec3;

// Variables associated to the GUI (buttons, etc)
struct gui_parameters {
  bool display_frame = false;
  bool display_wireframe = false;

  // vec3 light_color[] = [ {1, 1, 1}, {0.4, 0.6, 0.8}, {1, 1, 0.2} ];
  // vec3 light_position[] = [ {-2, 2, 0.8}, {1, -3, 0.2}, {-2, 1, 1} ];
  vec3 light_color = {1, 1, 1};
  vec3 light_position = {-2, -2, 0.8};

  float ambiant = 0.3f;
  float diffus = 0.8f;
  float speculaire = 0.5f;
  float exp_spec = 100.0f;
};

// The structure of the custom scene
struct scene_structure : cgp::scene_inputs_generic {
  bool debug_movements = false;

  // ****************************** //
  // Elements and shapes of the scene
  // ****************************** //
  camera_controller_orbit_euler camera_control;
  camera_projection_perspective camera_projection;
  window_structure window;

  mesh_drawable global_frame;         // The standard global frame
  environment_structure environment;  // Standard environment controler
  input_devices
      inputs;  // Storage for inputs status (mouse, keyboard, window dimension)
  gui_parameters gui;  // Standard GUI element storage

  // ****************************** //
  // Elements and shapes of the scene
  // ****************************** //

  timer_basic timer;

  float dt;

  opengl_shader_structure shader_custom;
  opengl_shader_structure shader_glow;

  World* worlds[SIDES_COUNT] = {nullptr};
  SpaceShip* space_ship;
  Tesseract* tesseract;

  // ****************************** //
  // Functions
  // ****************************** //

  void initialize();  // Standard initialization to be called before the
                      // animation loop
  void
  display_frame();  // The frame display to be called within the animation loop
  void display_gui();  // The display of the GUI, also called within the
                       // animation loop

  void mouse_move_event();
  void mouse_click_event();
  void keyboard_event();
  void idle_frame();

  void display_info();
};

typedef struct scene_structure scene_structure;
