#pragma once

#include "cgra/cgra_mesh.hpp"
#include <vector>

// this needs to be the last import to compile for some reason lul
#include "basic_model.hpp"
#include "shape.hpp"

/*
  1. generate points at specified res/rad
  2. given m_points, create triangles
  3. rebuild cgra_mesh
  4. then assign to m_model.mesh
*/
class Sphere : public Shape {

public:
  Sphere() {}

  Sphere(std::vector<GLuint> *shaders, glm::vec3 color, int *currentShader)
      : Shape(shaders, color, currentShader) {
    update();
  }

  // Core Members
  int m_longResolution = 10;
  int m_latResolution = 10;
  bool m_isFunkySphere = false;

  void update() override {
    // somehow reset mesh builder here?
    m_mb = cgra::mesh_builder();

    generateSpherePoints();
    makeIndices();
    createSphere();
    clearSpherePoints();

    m_model.color = m_color;
    m_model.shader = (*shaders)[(*m_currentShader)];

    m_model.mesh = m_mb.build();
  }

private:
  // 0 == regular sphere, 1 == funky sphere

  void latLongToCartesian(float lat, float lon, float &x, float &y, float &z);
  void generateSpherePoints();

  void latLongToFunky(float lat, float lon, float &x, float &y, float &z,
                      float scale);

  void createSphere();
  void makeIndices();
  void clearSpherePoints() const;
};
