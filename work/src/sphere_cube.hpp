#pragma once

#include "shape.hpp"

class CubeSphere : public Shape {
public:
  CubeSphere(GLuint shader, glm::vec3 color) : Shape(shader, color) {
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

	  m_model.mesh = m_mb.build();
  }

private:
	void generateCubePoints();
	void createCube();
	void makeIndices();
	void clearCubePoints() const;
};