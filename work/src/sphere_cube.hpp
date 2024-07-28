#pragma once

#include "shape.hpp"

class CubeSphere : public Shape {
public:
  CubeSphere(GLuint shader, glm::vec3 color) : Shape(shader, color) {}
  // Completion Members
  int m_cubeResolution = 10;
  float m_sphereness = 0; // 0-1 (1 being 100% sphere)
};