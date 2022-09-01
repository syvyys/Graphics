#include <QApplication>
#include <QSurfaceFormat>

#include "mainwindow.hpp"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);
    format.setVersion(2, 1);
    format.setDepthBufferSize(24);

    MainWindow mainWindow;
    mainWindow.setFormat(format);
    mainWindow.resize(640, 480);
    mainWindow.show();
    mainWindow.setAnimating(true);

    return QGuiApplication::exec();
}
