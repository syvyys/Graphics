#pragma once

#include "controller.hpp"

#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QBasicTimer>
#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

struct VertexData
{
    QVector3D position;
    QVector3D normal;
    QVector2D textcoord;
    QVector3D tangent;
    QVector3D bitangent;

    VertexData(QVector3D pos, QVector3D nor, QVector2D tex, QVector3D tan, QVector3D bitan)
        : position(pos), normal(nor), textcoord(tex), tangent(tan), bitangent(bitan) {}
};

class MainWindow : public QOpenGLWidget, protected QOpenGLFunctions
{
Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    void initializeGL() override;
    void paintGL() override;

protected:
    void initCube(float halfWidth, int N);
    void initSphere(float rad);
    void initTextures();

private:
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void timerEvent(QTimerEvent* e) override;

private:
    std::unique_ptr<QOpenGLShaderProgram> program = nullptr;
    std::unique_ptr<Controller> controller = nullptr;

    std::unique_ptr<QOpenGLTexture> texture = nullptr;
    std::unique_ptr<QOpenGLTexture> map = nullptr;

    QOpenGLBuffer vertexBuffer;
    QOpenGLBuffer indexBuffer{QOpenGLBuffer::IndexBuffer};

    GLint posAttr = 0;
    GLint norAttr = 0;
    GLint texAttr = 0;
    GLint tanAttr = 0;
    GLint bitanAttr = 0;

    int frames = 0;
    QBasicTimer timer;
};
