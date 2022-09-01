#include "openglwindow.hpp"

OpenGLWindow::OpenGLWindow(QWindow* parent) : QWindow(parent)
{
    setSurfaceType(QWindow::OpenGLSurface);
}

void OpenGLWindow::init() {}

void OpenGLWindow::setAnimating(bool animating)
{
    this->animating = animating;

    if (animating)
    {
        renderLater();
    }
}

void OpenGLWindow::render(const QPainter&) {}

void OpenGLWindow::render()
{
    if (!device)
    {
        device = std::make_unique<QOpenGLPaintDevice>(size());
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    device->setSize(size() * devicePixelRatio());
    device->setDevicePixelRatio(devicePixelRatio());

    const QPainter painter(device.get());
    render(painter);
}

void OpenGLWindow::renderLater()
{
    requestUpdate();
}

void OpenGLWindow::renderNow()
{
    if (!isExposed())
    {
        return;
    }

    bool needsInitialize = false;
    if (!context)
    {
        context = std::make_unique<QOpenGLContext>(this);
        context->setFormat(requestedFormat());
        context->create();

        needsInitialize = true;
    }

    const auto contextBindSuccess = context->makeCurrent(this);
    if (!contextBindSuccess)
    {
        return;
    }

    if (needsInitialize)
    {
        initializeOpenGLFunctions();
        init();
    }

    render();

    context->swapBuffers(this);

    if (animating)
    {
        renderLater();
    }
}

bool OpenGLWindow::event(QEvent* event)
{
    switch (event->type())
    {
    case QEvent::UpdateRequest:
        renderNow();
        return true;
    default:
        return QWindow::event(event);
    }
}

void OpenGLWindow::exposeEvent(QExposeEvent*)
{
    if (isExposed())
    {
        renderNow();
    }
}
