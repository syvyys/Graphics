#pragma once

#include <memory>
#include <unordered_set>

#include <QPoint>
#include <QFlags>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QQuaternion>
#include <QColorDialog>

class Controller
{
public:
    QColor getColor() const;
    QQuaternion getRotation() const;

    bool morphingIsActive() const;
    bool phongShaderIsActive() const;

    bool newColorSelected();
    int newNumCubes();

    void update();

public:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

private:
    void handleMove(QPoint newPosition);

    QPoint mousePosition;
    Qt::MouseButtons pressedMouseButtons;
    std::unordered_set<Qt::Key> pressedKeys;

    const float rotationSensitivity = 0.2f;
    QQuaternion rotation;
    QColor color;

    bool morphingIsActive_ = true;
    bool phongShaderIsActive_ = false;
    bool newColorSelected_ = false;
    int newNumCubes_ = 0;
};
