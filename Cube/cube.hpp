#pragma once

#include <QtMath>
#include <vector>
#include <QVector3D>
#include <QColorDialog>
#include <QOpenGLContext>

class Cube
{
public:
    Cube();
    Cube(GLfloat halfW, GLuint N = 1);

    void reshape(GLfloat halfW, GLuint N = 1);
    void changeColor(QColor color);

    const std::vector<GLuint>& get_indices() const;
    const std::vector<GLfloat>& get_vertices() const;

    int get_indices_size() const;
    int get_vertices_size() const;

private:
    void pushVertices(const QVector3D position,
                      const QVector3D normal,
                      const QColor color = {150, 150, 150});

private:
    GLuint n = 0;
    GLfloat halfWidth = 0;

    std::vector<GLuint> indices;
    std::vector<GLfloat> vertices;
};
