
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

void Application::createShaders() {
  shaders.push_back(loadShader("//res//shaders//color_vert.glsl",
                               "//res//shaders//color_frag.glsl"));

  shaders.push_back(loadShader("//res//shaders//color_vert.glsl",
                               "//res//shaders//cook_torrence_frag.glsl"));

  shaders.push_back(loadShader("//res//shaders//color_vert.glsl",
                               "//res//shaders//oren_nayar_frag.glsl"));

  shaders.push_back(loadShader("//res//shaders//comp_vert.glsl",
                               "//res//shaders//comp_frag.glsl"));
}

GLuint Application::loadShader(std::string vert_path, std::string frag_path) {
  shader_builder sb;

  sb.set_shader(GL_VERTEX_SHADER, CGRA_SRCDIR + vert_path);
  sb.set_shader(GL_FRAGMENT_SHADER, CGRA_SRCDIR + frag_path);

  return sb.build();
}

void Application::loadTextures() {
    // Load and upload texture
    cgra::rgba_image textureImage(CGRA_SRCDIR + std::string("//res//textures//checkerboard.jpg"));
    m_textureID = textureImage.uploadTexture();

    // Load and upload normal map
    cgra::rgba_image normalMapImage(CGRA_SRCDIR + std::string("//res//textures//normal.png"));
    m_normalMapID = normalMapImage.uploadTexture();
}


Application::Application(GLFWwindow *window) {
  m_window = window;

  createShaders(); // creates the shaders vec
  loadTextures();

  m_core = Sphere(&shaders, vec3(1, 0, 0), &m_currentShader);
  m_completion = CubeSphere(&shaders, vec3(1, 0, 0), &m_currentShader);
  m_challenge = Torus(&shaders, vec3(1, 0, 0), &m_currentShader);

  std::cout << "Init complete :D" << std::endl;
}

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

  // Set uniform values for the current shader
  glUseProgram(shaders[m_currentShader]);
  glUniform3fv(glGetUniformLocation(shaders[m_currentShader], "uColor"), 1, glm::value_ptr(m_color));

  switch (m_currentShader) {
      case 0: // default shader
          // Set default shader uniform values here if needed
          break;
      case 1: // cook torrence
          glUniform1f(glGetUniformLocation(shaders[m_currentShader], "roughness"), m_roughness);
          glUniform1f(glGetUniformLocation(shaders[m_currentShader], "F0"), m_F0);
          glUniform3fv(glGetUniformLocation(shaders[m_currentShader], "lightPos"), 1, glm::value_ptr(m_lightPos));
          glUniform1f(glGetUniformLocation(shaders[m_currentShader], "ambient"), m_ambient);
          break;
      case 2: // oren nayar
          glUniform1f(glGetUniformLocation(shaders[m_currentShader], "roughness"), m_roughness);
          glUniform1f(glGetUniformLocation(shaders[m_currentShader], "sigma"), m_sigma);  // Set sigma value
          glUniform3fv(glGetUniformLocation(shaders[m_currentShader], "lightPos"), 1, glm::value_ptr(m_lightPos));
          glUniform1f(glGetUniformLocation(shaders[m_currentShader], "ambient"), m_ambient);
          break;
      case 3: // textured
        glUniform3fv(glGetUniformLocation(shaders[m_currentShader], "lightPos"), 1, glm::value_ptr(m_lightPos));
        glUniform1f(glGetUniformLocation(shaders[m_currentShader], "ambient"), m_ambient);

        // Bind textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textureID);
        glUniform1i(glGetUniformLocation(shaders[m_currentShader], "texture_diffuse"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_normalMapID);
        glUniform1i(glGetUniformLocation(shaders[m_currentShader], "texture_normal"), 1);
        break;
    }


  // Draw generated sphere
  switch (m_stage) {
  case 0:
    m_core.draw(view, proj);
    break;
  case 1:
    m_completion.draw(view, proj);
    break;
  case 2:
    m_challenge.draw(view,proj);
    break;
  }
}

void Application::setShader(int i){
  m_currentShader = i;
  m_core.update();
  m_completion.update();
  m_challenge.update();
}

void Application::renderGUI() {

  // setup window
  ImGui::SetNextWindowPos(ImVec2(5, 5), ImGuiSetCond_Once);
  ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiSetCond_Once);
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

  if (ImGui::Button("Basic")) {
    setShader(0);
  }
  ImGui::SameLine();
  if (ImGui::Button("Cook-Torrance")) {
    setShader(1);
  }
  ImGui::SameLine();
  if (ImGui::Button("Oren-Nayar")) {
    setShader(2);
  }
  ImGui::SameLine();
  if (ImGui::Button("Textured")) {
    setShader(3);
  }



  // Add uniform controls
if (ImGui::CollapsingHeader("Shader Uniforms")) {
  if (ImGui::ColorEdit3("Color", glm::value_ptr(m_color))) {
    glUseProgram(shaders[m_currentShader]);
    glUniform3fv(glGetUniformLocation(shaders[m_currentShader], "uColor"), 1, glm::value_ptr(m_color));
  }

  switch (m_currentShader) {
      case 0: // default shader
          // Add default shader uniform controls here if needed
          break;
      case 1: // cook torrence
          ImGui::SliderFloat("Roughness", &m_roughness, 0.0f, 1.0f);
          ImGui::SliderFloat("F0", &m_F0, 0.0f, 1.0f);
          ImGui::SliderFloat3("Light Position", glm::value_ptr(m_lightPos), -10.0f, 10.0f);
          ImGui::SliderFloat("Ambient", &m_ambient, 0.0f, 1.0f);
          break;
      case 2: // oren nayar
          ImGui::SliderFloat("Sigma", &m_sigma, 0.0f, 1.0f);
          ImGui::SliderFloat3("Light Position", glm::value_ptr(m_lightPos), -10.0f, 10.0f);
          ImGui::SliderFloat("Ambient", &m_ambient, 0.0f, 1.0f);
          break;
      case 3: // textured
          ImGui::SliderFloat("Roughness", &m_roughness, 0.0f, 1.0f);
          ImGui::SliderFloat3("Light Position", glm::value_ptr(m_lightPos), -10.0f, 10.0f);
          ImGui::SliderFloat("Ambient", &m_ambient, 0.0f, 1.0f);
          ImGui::SliderFloat("Specular", &m_specular, 0.0f, 1.0f);
          ImGui::Checkbox("Use Normal Map", &m_useNormalMap);
          break;
    }
}


  if (ImGui::Button("Core")) {
    setStage(0);
    m_core.update();
  }
  ImGui::SameLine();
  if (ImGui::Button("Completion")) {
    setStage(1);
    m_completion.update();
  }
  ImGui::SameLine();
  if (ImGui::Button("Challenge")) {
    setStage(2);
    m_challenge.update();
  }

  ImGui::Separator();

  switch (m_stage) {
  case 0:
    ImGui::Text("Core");
    if (ImGui::SliderFloat("Radius", &m_core.m_radius, 1.0F, 50.0F)) {
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

    if (ImGui::SliderFloat("Radius", &m_completion.m_radius, 1.0F, 50.0F)) {
      if (m_completion.m_radius <= 0) {
        m_completion.m_radius = 0.1;
      }
      m_completion.update();
    }

    if (ImGui::InputInt("Cube Resolution", &m_completion.m_cubeResolution)) {
      if (m_completion.m_cubeResolution <= 0) {
        m_completion.m_cubeResolution = 1;
      }
      m_completion.update();
    }

    if (ImGui::SliderFloat("Sphereness", &m_completion.m_sphereness, 0, 1)) {
      m_completion.update();
    }

    if (ImGui::Checkbox("Cube Normals", &m_completion.m_cubeNormals)) {
      m_completion.update();
    }
    break;
  case 2:
    ImGui::Text("Challenge");
    if (ImGui::SliderFloat("Major Radius", &m_challenge.m_majorRadius,1.0F,10.0F)) {
      if (m_challenge.m_majorRadius <= 0) {
        m_challenge.m_majorRadius = 1;
      }
      m_challenge.update();
    }
    if (ImGui::SliderFloat("Minor Radius", &m_challenge.m_minorRadius,1.0F,10.0F)) {
      if (m_challenge.m_minorRadius <= 0) {
        m_challenge.m_minorRadius = 1;
      }
      m_challenge.update();
    }
    if (ImGui::InputInt("Major Segments", &m_challenge.m_majorSegments)) {
      if (m_challenge.m_majorSegments <= 0) {
        m_challenge.m_majorSegments = 1;
      }
      m_challenge.update();
    }
    if (ImGui::InputInt("Minor Segments", &m_challenge.m_minorSegments)) {
      if (m_challenge.m_minorSegments <= 0) {
        m_challenge.m_minorSegments = 1;
      }
      m_challenge.update();
    }
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