#pragma once

#include "cube.hpp"
#include "controller.hpp"
#include "openglwindow.hpp"

#include <cmath>
#include <vector>
#include <iostream>
#include <algorithm>
#include <QVector4D>
#include <QBasicTimer>
#include <QElapsedTimer>
#include <QOpenGLShader>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class MainWindow : public OpenGLWindow
{
    Q_OBJECT
public:
    void init() override;
    void render() override;
    void showFPS();

protected:
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;

    void keyPressEvent(QKeyEvent* e) override;
    void keyReleaseEvent(QKeyEvent* e) override;

private:
    void initShaders();

private:
    std::unique_ptr<Controller> controller = nullptr;
    std::unique_ptr<QOpenGLShaderProgram> program = nullptr;

    QOpenGLBuffer vertexBuffer;
    QOpenGLBuffer indexBuffer{QOpenGLBuffer::IndexBuffer};

    Cube cube;
    GLint numCubes = 2;
    GLuint morphingCounter = 0;

    float fps = 0.0f;
    float frames = 0.0f;
    float lastTime = 0.0f;
    float currentTime = 0.0f;
    QElapsedTimer timer;
};
