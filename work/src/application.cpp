
// std
#include <chrono>
#include <iostream>
#include <string>

// glm
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

// project
#include "application.hpp"
#include "cgra/cgra_geometry.hpp"
#include "cgra/cgra_gui.hpp"
#include "cgra/cgra_image.hpp"
#include "cgra/cgra_shader.hpp"
#include "cgra/cgra_wavefront.hpp"

using namespace std;
using namespace cgra;
using namespace glm;

void basic_model::draw(const glm::mat4 &view, const glm::mat4 proj) {
  mat4 modelview = view * modelTransform;

  glUseProgram(shader); // load shader and variables
  glUniformMatrix4fv(glGetUniformLocation(shader, "uProjectionMatrix"), 1,
                     false, value_ptr(proj));
  glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewMatrix"), 1, false,
                     value_ptr(modelview));
  glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, value_ptr(color));

  mesh.draw(); // draw
}

GLuint Application::createShader() {
  shader_builder sb;
  sb.set_shader(GL_VERTEX_SHADER,
                CGRA_SRCDIR + std::string("//res//shaders//color_vert.glsl"));
  sb.set_shader(GL_FRAGMENT_SHADER,
                CGRA_SRCDIR + std::string("//res//shaders//color_frag.glsl"));
  return sb.build();
}

Application::Application(GLFWwindow *window)
    : m_window(window), m_core(createShader(), vec3(1, 0, 0)),
      m_completion(createShader(), vec3(1, 0, 0)) {}

void Application::render() {

  // retrieve the window hieght
  int width, height;
  glfwGetFramebufferSize(m_window, &width, &height);

  m_windowsize = vec2(width, height); // update window size
  glViewport(0, 0, width,
             height); // set the viewport to draw to the entire window

  // clear the back-buffer
  glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // enable flags for normal/forward rendering
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // projection matrix
  mat4 proj = perspective(1.f, float(width) / height, 0.1f, 1000.f);

  // view matrix
  mat4 view = translate(mat4(1), vec3(0, 0, -m_distance)) *
              rotate(mat4(1), m_pitch, vec3(1, 0, 0)) *
              rotate(mat4(1), m_yaw, vec3(0, 1, 0));

  // helpful draw options
  if (m_show_grid)
    drawGrid(view, proj);
  if (m_show_axis)
    drawAxis(view, proj);
  glPolygonMode(GL_FRONT_AND_BACK, (m_showWireframe) ? GL_LINE : GL_FILL);

  // draw the model
  // m_model.draw(view, proj);

  // Draw generated sphere
  switch (m_stage) {
  case 0:
    m_core.draw(view, proj);
    break;
  case 1:
    m_completion.draw(view, proj);
  }
}

void Application::renderGUI() {

  // setup window
  ImGui::SetNextWindowPos(ImVec2(5, 5), ImGuiSetCond_Once);
  ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiSetCond_Once);
  ImGui::Begin("Options", 0);

  // display current camera parameters
  ImGui::Text("Application %.3f ms/frame (%.1f FPS)",
              1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  ImGui::SliderFloat("Pitch", &m_pitch, -pi<float>() / 2, pi<float>() / 2,
                     "%.2f");
  ImGui::SliderFloat("Yaw", &m_yaw, -pi<float>(), pi<float>(), "%.2f");
  ImGui::SliderFloat("Distance", &m_distance, 0, 100, "%.2f", 2.0f);

  // helpful drawing options
  ImGui::Checkbox("Show axis", &m_show_axis);
  ImGui::SameLine();
  ImGui::Checkbox("Show grid", &m_show_grid);
  ImGui::Checkbox("Wireframe", &m_showWireframe);
  ImGui::SameLine();
  if (ImGui::Button("Screenshot"))
    rgba_image::screenshot(true);

  if (ImGui::Button("Core")) {
    setStage(0);
  }
  ImGui::SameLine();
  if (ImGui::Button("Completion")) {
    setStage(1);
  }
  ImGui::SameLine();
  if (ImGui::Button("Challenge")) {
    setStage(2);
  }

  ImGui::Separator();

  switch (m_stage) {
  case 0:
    ImGui::Text("Core");
    if (ImGui::SliderFloat("Radius", &m_core.m_radius, 1.0F, 100.0F)) {
      if (m_core.m_radius <= 0) {
        m_core.m_radius = 0.1;
      }
      m_core.update();
    }

    if (ImGui::InputInt("Longitude Divisions", &m_core.m_longResolution)) {
      if (m_core.m_longResolution <= 0) {
        m_core.m_longResolution = 1;
      }
      m_core.update();
    }

    if (ImGui::InputInt("Latitude Divisions", &m_core.m_latResolution)) {
      if (m_core.m_latResolution <= 0) {
        m_core.m_latResolution = 1;
      }
      m_core.update();
    }

    if (ImGui::Checkbox("Funky Sphere", &m_core.m_isFunkySphere)) {
      m_core.update();
    }
    break;
  case 1:
    ImGui::Text("Completion");

    if (ImGui::InputInt("Cube Resolution", &m_completion.m_cubeResolution)) {
      if (m_completion.m_cubeResolution <= 0) {
        m_completion.m_cubeResolution = 1;
      }
      m_completion.update();
    }

    if (ImGui::SliderFloat("Sphereness", &m_completion.m_sphereness, 0, 1)) {
      m_completion.update();
    }
    break;
  case 2:
    ImGui::Text("Challenge");

    break;
  }

  // finish creating window
  ImGui::End();
}

void Application::cursorPosCallback(double xpos, double ypos) {
  if (m_leftMouseDown) {
    vec2 whsize = m_windowsize / 2.0f;

    // clamp the pitch to [-pi/2, pi/2]
    m_pitch += float(
        acos(glm::clamp((m_mousePosition.y - whsize.y) / whsize.y, -1.0f,
                        1.0f)) -
        acos(glm::clamp((float(ypos) - whsize.y) / whsize.y, -1.0f, 1.0f)));
    m_pitch = float(glm::clamp(m_pitch, -pi<float>() / 2, pi<float>() / 2));

    // wrap the yaw to [-pi, pi]
    m_yaw += float(
        acos(glm::clamp((m_mousePosition.x - whsize.x) / whsize.x, -1.0f,
                        1.0f)) -
        acos(glm::clamp((float(xpos) - whsize.x) / whsize.x, -1.0f, 1.0f)));
    if (m_yaw > pi<float>())
      m_yaw -= float(2 * pi<float>());
    else if (m_yaw < -pi<float>())
      m_yaw += float(2 * pi<float>());
  }

  // updated mouse position
  m_mousePosition = vec2(xpos, ypos);
}

void Application::mouseButtonCallback(int button, int action, int mods) {
  (void)mods; // currently un-used

  // capture is left-mouse down
  if (button == GLFW_MOUSE_BUTTON_LEFT)
    m_leftMouseDown =
        (action == GLFW_PRESS); // only other option is GLFW_RELEASE
}

void Application::scrollCallback(double xoffset, double yoffset) {
  (void)xoffset; // currently un-used
  m_distance *= pow(1.1f, -yoffset);
}

void Application::keyCallback(int key, int scancode, int action, int mods) {
  (void)key, (void)scancode, (void)action, (void)mods; // currently un-used
}

void Application::charCallback(unsigned int c) {
  (void)c; // currently un-used
}

void Application::setStage(int i) { m_stage = i; }