#pragma once

#include <vector>

// project
#include "cgra/cgra_mesh.hpp"

inline glm::vec3 **generate_sphere_points(int resolution, float radius) {
  glm::vec3 **arr = new glm::vec3 *[resolution];

  for (int i = 0; i < resolution; ++i) {
    arr[i] = new glm::vec3[resolution];
    for (int j = 0; j < resolution; ++j) {
      float lon = (j / resolution) * (glm::pi<float>() / 2);
      float lat = (i / resolution) * (glm::pi<float>());

      float x = radius * sin(lon) * cos(lat);
      float y = radius * sin(lon) * sin(lat);
      float z = radius * cos(lon);

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

  for (int i = 0; i < resolution - 1; i++) {
    for (int j = 0; j < resolution; j++) {

      // index = j ^ (j+1) + (i*2)
      std::cout << count << std::endl;
      mb.push_index(count);
      mb.push_index(count + 1);

      // positions
      glm::vec3 point1 = points[i][j];
      glm::vec3 point2 = points[i + 1][j];

      // calulate normals

      mb.push_vertex(cgra::mesh_vertex{point1, glm::vec3(1), glm::vec3(0)});
      mb.push_vertex(cgra::mesh_vertex{point2, glm::vec3(1), glm::vec3(0)});

      count += 2;
    }
  }

  std::cout << "meshbuilder made" << std::endl;

  return mb;
}
