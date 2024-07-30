#pragma once

#include "basic_model.hpp"
#include "cgra/cgra_mesh.hpp"

class Shape {
public:
  Shape(GLuint shader, glm::vec3 color) {
    m_model.shader = shader;
    m_model.color = color;
    update();
  }
  void draw(const glm::mat4 &view, const glm::mat4 proj) {
    m_model.draw(view, proj);
  }

  virtual void update() {};

protected:
  glm::vec3 **m_points;
  basic_model m_model;
  cgra::mesh_builder m_mb;
};