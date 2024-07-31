#pragma once

#include "basic_model.hpp"
#include "cgra/cgra_mesh.hpp"

class Shape {

public:
  Shape() {}
  Shape(std::vector<GLuint> *shaders, glm::vec3 color, int *currentShader)
      : shaders(shaders), m_color(color), m_points(nullptr),
        m_currentShader(currentShader) {}

  void draw(const glm::mat4 &view, const glm::mat4 proj) {
    m_model.draw(view, proj);
  }

  float m_radius = 5;
  int *m_currentShader;
  std::vector<GLuint> *shaders;
  glm::vec3 m_color;
  virtual void update() {};

protected:
  glm::vec3 **m_points;
  basic_model m_model;
  cgra::mesh_builder m_mb;
};