#include "controller.hpp"

QQuaternion Controller::getRotation() const
{
    return rotation;
}

void Controller::mousePressEvent(QMouseEvent* e)
{
    mousePosition = e->pos();
    pressedMouseButtons.setFlag(e->button(), true);
}

void Controller::mouseMoveEvent(QMouseEvent* e)
{
    if (pressedMouseButtons.testFlag(Qt::LeftButton))
    {
        handleMove(e->pos());
    }
}

void Controller::mouseReleaseEvent(QMouseEvent* e)
{
    if (pressedMouseButtons.testFlag(Qt::LeftButton))
    {
        handleMove(e->pos());
    }
    pressedMouseButtons.setFlag(e->button(), false);
}

void Controller::handleMove(QPoint newPosition)
{
    const auto diff = QVector2D{newPosition - mousePosition};
    const auto axis = QVector3D{diff.y(), diff.x(), 0.0F}.normalized();
    const auto angle = diff.length() * rotationSensitivity;

    rotation = QQuaternion::fromAxisAndAngle(axis, angle) * rotation;
    mousePosition = newPosition;
}
