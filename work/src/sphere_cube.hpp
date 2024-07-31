#pragma once

#include "shape.hpp"

class CubeSphere : public Shape {
public:
  CubeSphere() {}

  CubeSphere(std::vector<GLuint> *shaders, glm::vec3 color, int *currentShader)
      : Shape(shaders, color, currentShader) {
    update();
  }
  // Completion Members
  int m_cubeResolution = 10;
  float m_sphereness = 0; // 0-1 (1 being 100% sphere)
  bool m_cubeNormals = true;

  void update() override {
    // somehow reset mesh builder here?
    m_mb = cgra::mesh_builder();

    generateCubePoints();
    makeIndices();
    createCube();
    clearCubePoints();

    m_model.color = m_color;
    m_model.shader = (*shaders)[(*m_currentShader)];

    m_model.mesh = m_mb.build();
  }

private:
  void generateCubePoints();
  void createCube();
  void makeIndices();
  void clearCubePoints() const;
};