#include "mainwindow.hpp"

#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);
    format.setVersion(2, 1);
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    MainWindow window;
    window.showNormal();

    return app.exec();
}
