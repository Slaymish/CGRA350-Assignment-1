#pragma once

#include "sphere.hpp"
#include "shape.hpp"
#include <glm/gtc/constants.hpp>

void Sphere::latLongToCartesian(float lat, float lon, float &x, float &y,
                                float &z) {
  x = m_radius * cos(lat) * cos(lon);
  y = m_radius * cos(lat) * sin(lon);
  z = m_radius * sin(lat);
}

void Sphere::latLongToFunky(float lat, float lon, float &x, float &y, float &z,
                            float scale) {

  float adjustedRadius = scale * m_radius; // do smth interesting here???
  x = adjustedRadius * cos(lat) * cos(lon);
  y = adjustedRadius * cos(lat) * sin(lon);
  z = adjustedRadius * sin(lat);
}

void Sphere::generateSpherePoints() {
  glm::vec3 **arr = new glm::vec3 *[m_latResolution + 1];

  for (int j = 0; j <= m_latResolution; j++) {
    arr[j] = new glm::vec3[m_longResolution +
                           1]; // malloc sig fault here when increasing lat/long
    for (int i = 0; i <= m_longResolution; i++) {
      // lon = 0,res -> 0, pi/2
      float lon = (i * 2 * glm::pi<float>()) / m_longResolution;

      // lat = 0,res -> 0,pi
      float lat =
          (j * glm::pi<float>() / m_latResolution) - glm::half_pi<float>();

      float x, y, z;

      if (!m_isFunkySphere)
        latLongToCartesian(lat, lon, x, y, z);
      else {
        float scale = sin(lon) * cos(lat);
        latLongToFunky(lat, lon, x, y, z, scale);
      }

      arr[j][i] = glm::vec3(x, y, z);
    }
  }

  m_points = arr;
}

void Sphere::generateCubePoints() {
  // Normallise cube points turns into sphere!!!!!
  // 1. create cube points
  // 2. Mix with sphere points (0,1)
  // fill in cube/sphere
}

void Sphere::makeIndices() {
  for (int j = 0; j <= m_latResolution; j++) {
    for (int i = 0; i <= m_longResolution; i++) {
      int i1 = j * (m_longResolution + 1) + i;
      int i2 = (j + 1) * (m_longResolution + 1) + i; // mayb change to longRes
      int i3 = i1 + 1;
      int i4 = i2 + 1;

      m_mb.push_index(i1);
      m_mb.push_index(i2);
      m_mb.push_index(i3);

      m_mb.push_index(i3);
      m_mb.push_index(i2);
      m_mb.push_index(i4);
    }
  }
}

void Sphere::createSphere() {
  for (int j = 0; j <= m_latResolution; j++) {
    for (int i = 0; i <= m_longResolution; i++) {
      // positions
      glm::vec3 point = m_points[j][i];

      // calulate normals
      glm::vec3 normal = glm::normalize(point);

      // calculate texture coordinates
      float u = static_cast<float>(i) / (m_longResolution);
      float v = static_cast<float>(j) / (m_latResolution);

      m_mb.push_vertex(cgra::mesh_vertex{point, normal, glm::vec2(u, v)});
    }
  }

  // Add the last ring of vertices
  int j = m_latResolution;
  for (int i = 0; i <= m_longResolution; i++) {
    glm::vec3 point = m_points[j][i];
    glm::vec3 normal = glm::normalize(point);
    float u = static_cast<float>(i) / (m_longResolution);
    float v = 1.0f;

    m_mb.push_vertex(cgra::mesh_vertex{point, normal, glm::vec2(u, v)});
  }
}

void Sphere::update() {
  // somehow reset mesh builder here?
  m_mb = cgra::mesh_builder();

  generateSpherePoints();
  makeIndices();
  createSphere();
  clearSpherePoints();

  m_model.mesh = m_mb.build();
}

void Sphere::clearSpherePoints() {
  for (int j = 0; j <= m_latResolution; j++) {
    delete[] m_points[j];
  }
  delete[] m_points;
}
