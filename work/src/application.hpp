
#pragma once

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "cgra/cgra_mesh.hpp"
#include "opengl.hpp"
#include "skeleton_model.hpp"

#include "sphere.hpp"
#include "sphere_cube.hpp"
#include "torus.hpp"

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

  Sphere m_core;
  CubeSphere m_completion;
  Torus m_challenge;

  // Core/Completion/Challenge (0,1,2)
  int m_stage = 0;

  std::vector<GLuint> shaders;
  int m_currentShader = 0;

  // Shader uniforms
  float m_roughness = 0.5f;
  float m_F0 = 0.04f;
  glm::vec3 m_lightPos = glm::vec3(5.0f, 5.0f, 5.0f);
  float m_ambient = 0.1f;
  glm::vec3 m_color = glm::vec3(1.0f, 0.0f, 0.0f);
  float m_sigma = 0.1f;

  GLuint loadShader(std::string vert_path, std::string frag_path);
  void loadTextures();
  void initialiseShadowMapping();

  GLuint m_textureID;
  GLuint m_normalMapID;

  GLuint m_shadowMapFBO;
  GLuint m_shadowMapTexture;
  GLuint m_shadowMapShader;
  glm::mat4 m_lightSpaceMatrix;


public:
  // setup
  Application(GLFWwindow *);

  // disable copy constructors (for safety)
  Application(const Application &) = delete;
  Application &operator=(const Application &) = delete;

  // rendering callbacks (every frame)
  void render();
  void renderGUI();


  void setShader(int i);

  // input callbacks
  void createShaders();
  void cursorPosCallback(double xpos, double ypos);
  void mouseButtonCallback(int button, int action, int mods);
  void scrollCallback(double xoffset, double yoffset);
  void keyCallback(int key, int scancode, int action, int mods);
  void charCallback(unsigned int c);
  void setStage(int i);
};