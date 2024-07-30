#pragma once

#include "shape.hpp"
#include "sphere_cube.hpp"
#include <glm/gtc/constants.hpp>

void CubeSphere::generateCubePoints()
{
    m_points = new glm::vec3 * [6 * (m_cubeResolution + 1)];
    for (int i = 0; i < 6 * (m_cubeResolution + 1); i++) {
        m_points[i] = new glm::vec3[m_cubeResolution + 1];
    }

    int idx = 0;

    // Loop through each face of the cube
    for (int face = 0; face < 6; face++) {
        // Loop through each row and column of the face
        for (int j = 0; j <= m_cubeResolution; j++) {
            for (int i = 0; i <= m_cubeResolution; i++) {
                // Calculate the x, y, z values based on the face and i, j
                float x, y, z;
                switch (face) {
                case 0: // Bottom face
                    x = (i - m_cubeResolution / 2.0f) / (m_cubeResolution / 2.0f);
                    y = -1;
                    z = (j - m_cubeResolution / 2.0f) / (m_cubeResolution / 2.0f);
                    break;
                case 1: // Top face
                    x = (i - m_cubeResolution / 2.0f) / (m_cubeResolution / 2.0f);
                    y = 1;
                    z = (j - m_cubeResolution / 2.0f) / (m_cubeResolution / 2.0f);
                    break;
                case 2: // Left face
                    x = -1;
                    y = (i - m_cubeResolution / 2.0f) / (m_cubeResolution / 2.0f);
                    z = (j - m_cubeResolution / 2.0f) / (m_cubeResolution / 2.0f);
                    break;
                case 3: // Right face
                    x = 1;
                    y = (i - m_cubeResolution / 2.0f) / (m_cubeResolution / 2.0f);
                    z = (j - m_cubeResolution / 2.0f) / (m_cubeResolution / 2.0f);
                    break;
                case 4: // Front face
                    x = (i - m_cubeResolution / 2.0f) / (m_cubeResolution / 2.0f);
                    y = (j - m_cubeResolution / 2.0f) / (m_cubeResolution / 2.0f);
                    z = 1;
                    break;
                case 5: // Back face
                    x = (i - m_cubeResolution / 2.0f) / (m_cubeResolution / 2.0f);
                    y = (j - m_cubeResolution / 2.0f) / (m_cubeResolution / 2.0f);
                    z = -1;
                    break;
                }

                // Store the vertex in the array
                m_points[idx / (m_cubeResolution + 1)][idx % (m_cubeResolution + 1)] = glm::vec3(x, y, z);
                idx++;
            }
        }
    }

    // Move the cube points to create the sphere shape
    for (int face = 0; face < 6; face++) {
        for (int j = 0; j <= m_cubeResolution; j++) {
            for (int i = 0; i <= m_cubeResolution; i++) {
                glm::vec3 vertex = m_points[face * (m_cubeResolution + 1) + j][i];
                float distance = glm::length(vertex);
                vertex *= (1 - m_sphereness) + m_sphereness * (1 / distance);
                m_points[face * (m_cubeResolution + 1) + j][i] = vertex;
            }
        }
    }
}

void CubeSphere::createCube()
{
    int idx = 0;
    for (int face = 0; face < 6; face++) {
        for (int j = 0; j <= m_cubeResolution; j++) {
            for (int i = 0; i <= m_cubeResolution; i++) {
                glm::vec3 point = m_points[face * (m_cubeResolution + 1) + j][i];
                glm::vec3 normal;
                // calc normal :D
                if (m_cubeNormals) {
                    // Calculate normal using original cube vertex position
                    switch (face) {
                    case 0: // Bottom face
                        normal = glm::vec3(0, -1, 0);
                        break;
                    case 1: // Top face
                        normal = glm::vec3(0, 1, 0);
                        break;
                    case 2: // Left face
                        normal = glm::vec3(-1, 0, 0);
                        break;
                    case 3: // right
                        normal = glm::vec3(1, 0, 0);
                        break;
                    case 4: // front
                        normal = glm::vec3(0, 0, -1);
                        break;
                    case 5: //back
                        normal = glm::vec3(0, 0, 1);
                        break;
                    }
                }
                else {
                    normal = glm::normalize(point);
                }
                float u = static_cast<float>(i) / (m_cubeResolution);
                float v = static_cast<float>(j) / (m_cubeResolution);

                m_mb.push_vertex(cgra::mesh_vertex{ point, normal, glm::vec2(u, v) });
                idx++;
            }
        }
    }
}

void CubeSphere::makeIndices()
{
    int idx = 0;
    for (int face = 0; face < 6; face++) {
        for (int j = 0; j < m_cubeResolution; j++) {
            for (int i = 0; i < m_cubeResolution; i++) {
                int i1 = idx;
                int i2 = idx + m_cubeResolution + 1;
                int i3 = idx + 1;
                int i4 = idx + m_cubeResolution + 2;

                m_mb.push_index(i1);
                m_mb.push_index(i2);
                m_mb.push_index(i3);

                m_mb.push_index(i3);
                m_mb.push_index(i2);
                m_mb.push_index(i4);

                idx++;
            }
            idx++; // Skip the last vertex in each row
        }
        idx += m_cubeResolution + 1; // Skip the last row in each face
    }
}

void CubeSphere::clearCubePoints() const {
    for (int i = 0; i < 6 * (m_cubeResolution + 1); i++) {
        delete[] m_points[i];
    }
    delete[] m_points;
}