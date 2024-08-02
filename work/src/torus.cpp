#include "torus.hpp"
#include <cmath>


void Torus::generateTorusPoints() {
    m_points = new glm::vec3*[m_majorSegments];
    for (int i = 0; i < m_majorSegments; ++i) {
        m_points[i] = new glm::vec3[m_minorSegments];
    }

    for (int i = 0; i < m_majorSegments; ++i) {
        float theta = 2.0f * glm::pi<float>() * i / m_majorSegments;

        for (int j = 0; j < m_minorSegments; ++j) {
            float phi = 2.0f * glm::pi<float>() * j / m_minorSegments;

            float x = (m_majorRadius + m_minorRadius * std::cos(phi)) * std::cos(theta);
            float y = (m_majorRadius + m_minorRadius * std::cos(phi)) * std::sin(theta);
            float z = m_minorRadius * std::sin(phi);

            m_points[i][j] = glm::vec3(x, y, z);
        }
    }
}

void Torus::createTorus() {
    for (int i = 0; i < m_majorSegments; ++i) {
        for (int j = 0; j < m_minorSegments; ++j) {
            float u = static_cast<float>(i) / m_majorSegments;
            float v = static_cast<float>(j) / m_minorSegments;

            glm::vec3 point = m_points[i][j];
            glm::vec3 normal = glm::normalize(point - glm::vec3(m_majorRadius * std::cos(2.0f * glm::pi<float>() * i / m_majorSegments),
                                                                m_majorRadius * std::sin(2.0f * glm::pi<float>() * i / m_majorSegments),
                                                                0.0f));

            m_mb.push_vertex(cgra::mesh_vertex{point, normal, glm::vec2(u, v)});
        }
    }
}

void Torus::makeIndices() {
    for (int i = 0; i < m_majorSegments; ++i) {
        for (int j = 0; j < m_minorSegments; ++j) {
            int i1 = i;
            int i2 = (i + 1) % m_majorSegments;
            int j1 = j;
            int j2 = (j + 1) % m_minorSegments;

            int v1 = i1 * m_minorSegments + j1;
            int v2 = i2 * m_minorSegments + j1;
            int v3 = i2 * m_minorSegments + j2;
            int v4 = i1 * m_minorSegments + j2;

            m_mb.push_index(v1);
            m_mb.push_index(v2);
            m_mb.push_index(v3);

            m_mb.push_index(v1);
            m_mb.push_index(v3);
            m_mb.push_index(v4);
        }
    }
}

void Torus::clearTorusPoints() {
    if (m_points) {
        for (int i = 0; i < m_majorSegments; ++i) {
            delete[] m_points[i];
        }
        delete[] m_points;
        m_points = nullptr;
    }
}


void Torus::update() {
    m_mb = cgra::mesh_builder();

    generateTorusPoints();
    createTorus();
    makeIndices();
    clearTorusPoints();

    m_model.color = m_color;
    m_model.shader = (*shaders)[(*m_currentShader)];

    m_model.mesh = m_mb.build();
}