
#pragma once

#include "cgra/cgra_mesh.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // Include this header for value_ptr

struct basic_model {
  GLuint shader = 0;
  cgra::gl_mesh mesh;
  glm::vec3 color{0.7};
  glm::mat4 modelTransform{1.0};
  GLuint texture;

  void draw(const glm::mat4 &view, const glm::mat4 proj) {
    glm::mat4 modelview = view * modelTransform;

    glUseProgram(shader); // load shader and variables
    glUniformMatrix4fv(glGetUniformLocation(shader, "uProjectionMatrix"), 1,
                       false, glm::value_ptr(proj));
    glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewMatrix"), 1,
                       false, glm::value_ptr(modelview));
    glUniform3fv(glGetUniformLocation(shader, "uColor"), 1,
                 glm::value_ptr(color));

    mesh.draw(); // draw
  }
};
