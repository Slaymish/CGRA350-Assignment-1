#pragma once

#include <vector>

// project
#include "cgra/cgra_mesh.hpp"

inline glm::vec3 **generate_sphere_points(int resolution, float radius) {
  glm::vec3 **arr = new glm::vec3 *[resolution];

  for (unsigned int i = 0; i < resolution; ++i) {
    arr[i] = new glm::vec3[resolution];
    for (unsigned int j = 0; j < resolution; ++j) {
      // low2 + (value - low1) * (high2 - low2) / (high1 - low1)

      // lon = 0,res -> 0, pi/2
      float lon = ((j) * (glm::pi<float>() / 2)) / resolution;

      // lat = 0,res -> 0,pi
      float lat = ((i) * (glm::pi<float>())) / resolution;

      float x = radius * sin(lon) * cos(lat);
      float y = radius * sin(lon) * sin(lat);
      float z = radius * cos(lon);

      std::cout << "lat: " << lat << " long: " << lon << std::endl;
      std::cout << "x: " << x << " y: " << y << " z: " << z << std::endl;

      glm::vec3 v = glm::vec3(x, y, z);
      arr[i][j] = v;
    }
  }

  return arr;
}

inline cgra::mesh_builder create_sphere(int resolution, float radius) {

  // generate sphere points
  glm::vec3 **points;
  points = generate_sphere_points(resolution, radius);

  std::cout << "points generated" << std::endl;

  // if we don't have any normals, create them naively
  // todo create spherical UV's if they don't exist

  // create mesh data
  cgra::mesh_builder mb;

  int count = 0;

  for (unsigned int i = 0; i < resolution - 1; i++) {
    for (unsigned int j = 0; j < resolution; j++) {

      // index = j ^ (j+1) + (i*2)
      mb.push_index(count);
      mb.push_index(count + 1);

      // positions
      glm::vec3 point1 = points[j][i];
      glm::vec3 point2 = points[j][i + 1];

      // calulate normals

      mb.push_vertex(
          cgra::mesh_vertex{point1, glm::vec3(1, 0, 0), glm::vec3(0)});
      mb.push_vertex(
          cgra::mesh_vertex{point2, glm::vec3(1, 0, 0), glm::vec3(0)});

      count += 2;
    }
  }

  std::cout << "meshbuilder made" << std::endl;

  return mb;
}
