#include "cube.hpp"

#include <iostream>

Cube::Cube() {}

Cube::Cube(GLfloat halfW, GLuint N)
{
    reshape(halfW, N);
}

void Cube::reshape(GLfloat halfW, GLuint N)
{
    // clear all vectors
    indices.clear();
    vertices.clear();

    // change cube params
    if (halfW <= 0 or N <= 1)
    {
        halfWidth = 0;
        n = 0;
        return;
    }

    halfWidth = halfW;
    n = N;

    // create cube
    for (auto face = 0U; face < 6; ++face)
    {
        const auto coord0 = face % 3;
        const auto coord1 = (face + 1) % 3;
        const auto coord2 = (face + 2) % 3;

        // add vertices
        for (auto y = 0U; y < n + 1; ++y)
        {
            for (auto x = 0U; x < n + 1; ++x)
            {
                QVector3D position{0, 0, 0};
                QVector3D normal{0, 0, 0};

                normal[coord0] = static_cast<int>(face / 3 * 2) - 1;

                position[coord0] = halfWidth * normal[coord0];
                position[coord1] = halfWidth * (x * 2.0f / n - 1.0f);
                position[coord2] = halfWidth * (y * 2.0f / n - 1.0f) * normal[coord0];

                pushVertices(position, normal);
            }
        }

        // add indices
        const auto faceIndexOffset = face * (n + 1) * (n + 1);
        for (auto y = 0U; y < n; ++y)
        {
            const auto rowIndexOffset = (n + 1) * y;

            indices.push_back(faceIndexOffset + rowIndexOffset);
            for (auto x = 0U; x < n + 1; ++x)
            {
                indices.push_back(faceIndexOffset + rowIndexOffset + x);
                indices.push_back(faceIndexOffset + rowIndexOffset + x + (n + 1));
            }
            indices.push_back(faceIndexOffset + rowIndexOffset + n + (n + 1));
        }
    }
}

void Cube::changeColor(QColor newColor)
{
    const GLuint stride = 9;
    const GLuint vertex_color_offset = 6;
    for (auto i = 0U; i < vertices.size() / stride; ++i)
    {
        vertices[vertex_color_offset + stride * i + 0] = newColor.redF();
        vertices[vertex_color_offset + stride * i + 1] = newColor.greenF();
        vertices[vertex_color_offset + stride * i + 2] = newColor.blueF();
    }
}

void Cube::pushVertices(const QVector3D position,
                        const QVector3D normal,
                        const QColor color)
{
    vertices.push_back(position.x());
    vertices.push_back(position.y());
    vertices.push_back(position.z());
    vertices.push_back(normal.x());
    vertices.push_back(normal.y());
    vertices.push_back(normal.z());
    vertices.push_back(color.redF());
    vertices.push_back(color.greenF());
    vertices.push_back(color.blueF());
}

const std::vector<GLuint>& Cube::get_indices() const
{
    return indices;
}

const std::vector<GLfloat>& Cube::get_vertices() const
{
    return vertices;
}

GLint Cube::get_indices_size() const
{
    return indices.size();
}


GLint Cube::get_vertices_size() const
{
    return vertices.size();
}
