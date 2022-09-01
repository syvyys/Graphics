#pragma once

#include <QPoint>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QQuaternion>

class Controller
{
public:
    QQuaternion getRotation() const;

public:
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);

private:
    void handleMove(QPoint newPosition);

private:
    QPoint mousePosition;
    QQuaternion rotation;
    Qt::MouseButtons pressedMouseButtons;
    const float rotationSensitivity = 0.2f;
};
