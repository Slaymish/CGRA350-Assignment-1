#pragma once

#include <vector>

// project
#include "cgra_mesh.hpp"

inline mesh_builder create_sphere(int resolution, float radius) {
  // struct for storing wavefront index data
  struct wavefront_vertex {
    unsigned int p, n, t;
  };

  // create reading buffers
  vector<vec3> positions;
  vector<vec3> normals;
  vector<vec2> uvs;

  // generate sphere points

  vec3 v = vec3(0, 0, 0)

               positions.push_back(v);
  normals.push_back(vn);
  uvs.push_back(vt);

  // if we don't have any normals, create them naively
  // todo create spherical UV's if they don't exist

  // create mesh data
  mesh_builder mb;

  for (unsigned int i = 0; i < wv_vertices.size(); ++i) {
    mb.push_index(i);
    mb.push_vertex(mesh_vertex{positions[wv_vertices[i].p],
                               normals[wv_vertices[i].n],
                               uvs[wv_vertices[i].t]});
  }

  return mb;
}

inline vector<mesh_vertex> generate_sphere_points(int resolution,
                                                  float radius) {
  for (int i = 0; i < resolution; i++) {
    for (int j = 0; j < resolution; j++) {
      float lon = map(j, 0, resolution, 0, glm::pi / 2);
      float lat = map(i, 0, resolution, 0, glm::pi);

      float x = radius * sin(lon) * cos(lat);
      float y = radius * sin(lon) * sin(lat);
      float z = radius * cos(lon);
    }
  }
}