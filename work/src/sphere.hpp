#pragma once

#include "cgra/cgra_mesh.hpp"
#include <vector>

// this needs to be the last import to compile for some reason lul
#include "basic_model.hpp"

/*
  1. generate points at specified res/rad
  2. given m_points, create triangles
  3. rebuild cgra_mesh
  4. then assign to m_model.mesh
*/
class Sphere {

public:
  int m_resolution = 10;
  float m_radius = 20;

  Sphere(GLuint shader, glm::vec3 color) {
    m_model.shader = shader;
    m_model.color = color;
    update();
  }

  // These update the member vars, then rebuild the sphere
  void draw(const glm::mat4 &view, const glm::mat4 proj);

  // recreates and assigns m_mesh with current res/rad members
  void update();

  ~Sphere();

private:
  glm::vec3 **m_points;
  cgra::mesh_builder m_mb;
  basic_model m_model;

  void latLongToCartesian(float lat, float lon, float &x, float &y, float &z);
  void generateSpherePoints();

  void createSphere();
  void makeIndices();
};