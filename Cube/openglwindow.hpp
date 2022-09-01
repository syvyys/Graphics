#pragma once

#include <memory>
#include <QWindow>
#include <QPainter>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLPaintDevice>

class QEvent;
class QPainter;
class QExposeEvent;
class QOpenGLContext;
class QOpenGLPaintDevice;

class OpenGLWindow : public QWindow, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    OpenGLWindow(QWindow* parent = nullptr);

    virtual void init();
    virtual void render();
    virtual void render(const QPainter& painter);

    void setAnimating(bool animating = false);

public slots:
    void renderNow();
    void renderLater();

protected:
    bool event(QEvent* event) override;
    void exposeEvent(QExposeEvent* event) override;

private:
    bool animating = false;
    std::unique_ptr<QOpenGLContext> context = nullptr;
    std::unique_ptr<QOpenGLPaintDevice> device = nullptr;
};


