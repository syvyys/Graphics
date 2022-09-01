#include "controller.hpp"

QQuaternion Controller::getRotation() const
{
    return rotation;
}

QColor Controller::getColor() const
{
    return color;
}

bool Controller::phongShaderIsActive() const
{
    return phongShaderIsActive_;
}

bool Controller::morphingIsActive() const
{
    return morphingIsActive_;
}


void Controller::mousePressEvent(QMouseEvent* event)
{
    mousePosition = event->pos();
    pressedMouseButtons.setFlag(event->button(), true);
}

void Controller::mouseMoveEvent(QMouseEvent* event)
{
    if (pressedMouseButtons.testFlag(Qt::LeftButton))
    {
        handleMove(event->pos());
    }
}

void Controller::mouseReleaseEvent(QMouseEvent* event)
{
    if (pressedMouseButtons.testFlag(Qt::LeftButton))
    {
        handleMove(event->pos());
    }
    pressedMouseButtons.setFlag(event->button(), false);
}

void Controller::keyPressEvent(QKeyEvent* event)
{
    pressedKeys.insert(static_cast<Qt::Key>(event->key()));
}

void Controller::keyReleaseEvent(QKeyEvent* event)
{
    pressedKeys.erase(static_cast<Qt::Key>(event->key()));
}

void Controller::update()
{
    if (!!pressedKeys.count(Qt::Key::Key_Space))
    {
        const auto newColor = QColorDialog::getColor(Qt::black, nullptr, "Select color");
        if (newColor.isValid())
        {
            color = newColor;
            newColorSelected_ = true;
        }
        pressedKeys.erase(Qt::Key::Key_Space);
    }

    if (!!pressedKeys.count(Qt::Key::Key_G))
    {
        phongShaderIsActive_ = false;
        pressedKeys.erase(Qt::Key::Key_G);
    }

    if (!!pressedKeys.count(Qt::Key::Key_P))
    {
        phongShaderIsActive_ = true;
        pressedKeys.erase(Qt::Key::Key_P);
    }

    if (!!pressedKeys.count(Qt::Key::Key_M))
    {
        morphingIsActive_ = !morphingIsActive_;
        pressedKeys.erase(Qt::Key::Key_M);
    }

    if (!!pressedKeys.count(Qt::Key::Key_Left))
    {
        --newNumCubes_;
        pressedKeys.erase(Qt::Key::Key_Left);
    }

    if (!!pressedKeys.count(Qt::Key::Key_Right))
    {
        ++newNumCubes_;
        pressedKeys.erase(Qt::Key::Key_Right);
    }
}

bool Controller::newColorSelected()
{
    bool res = newColorSelected_;
    newColorSelected_ = false;
    return res;
}

int Controller::newNumCubes()
{
    int res = newNumCubes_;
    newNumCubes_ = 0;
    return res;
}

void Controller::handleMove(QPoint newPosition)
{
    const auto diff = QVector2D{newPosition - mousePosition};
    const auto axis = QVector3D{diff.y(), diff.x(), 0.0F}.normalized();
    const auto angle = diff.length() * rotationSensitivity;

    rotation = QQuaternion::fromAxisAndAngle(axis, angle) * rotation;
    mousePosition = newPosition;
}
