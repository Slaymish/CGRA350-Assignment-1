#pragma once

#include "cgra/cgra_mesh.hpp"
#include <vector>
#include <glm/glm.hpp>
#include "shape.hpp"

class Torus : public Shape {
public:
    Torus(){}


  Torus(std::vector<GLuint> *shaders, glm::vec3 color, int *currentShader,
          float majorRadius = 1.0f, float minorRadius = 0.3f,
          int majorSegments = 32, int minorSegments = 24)
        : Shape(shaders, color, currentShader),
          m_majorRadius(majorRadius), m_minorRadius(minorRadius),
          m_majorSegments(majorSegments), m_minorSegments(minorSegments) {
        update();
    }

  void update() override;

    float m_majorRadius;  // Distance from the center of the tube to the center of the torus
    float m_minorRadius;  // Radius of the tube
    int m_majorSegments;  // Number of segments around the major radius
    int m_minorSegments;  // Number of segments around the minor radius

private:
  void generateTorusPoints();
  void createTorus();
  void makeIndices();
  void clearTorusPoints();
};
