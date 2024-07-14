
#pragma once

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "cgra/cgra_mesh.hpp"
#include "opengl.hpp"
#include "skeleton_model.hpp"

#include "sphere.hpp"

#include "basic_model.hpp"

// Basic model that holds the shader, mesh and transform for drawing.
// Can be copied and modified for adding in extra information for drawing
// including textures for texture mapping etc.

// Main application class
//
class Application {
private:
  // window
  glm::vec2 m_windowsize;
  GLFWwindow *m_window;

  // oribital camera
  float m_pitch = .86;
  float m_yaw = -.86;
  float m_distance = 20;

  // last input
  bool m_leftMouseDown = false;
  glm::vec2 m_mousePosition;

  // drawing flags
  bool m_show_axis = false;
  bool m_show_grid = false;
  bool m_showWireframe = false;

  // geometry
  basic_model m_model;

  Sphere sphere;

public:
  // setup
  Application(GLFWwindow *);

  // disable copy constructors (for safety)
  Application(const Application &) = delete;
  Application &operator=(const Application &) = delete;

  // rendering callbacks (every frame)
  void render();
  void renderGUI();

  // input callbacks
  GLuint createShader();
  void cursorPosCallback(double xpos, double ypos);
  void mouseButtonCallback(int button, int action, int mods);
  void scrollCallback(double xoffset, double yoffset);
  void keyCallback(int key, int scancode, int action, int mods);
  void charCallback(unsigned int c);
};