QT       += core gui widgets openglwidgets opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    controller.cpp \
    cube.cpp \
    main.cpp \
    mainwindow.cpp \
    openglwindow.cpp

HEADERS += \
    controller.hpp \
    cube.hpp \
    mainwindow.hpp \
    openglwindow.hpp

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    gouraud.fsh \
    gouraud.vsh \
    phong.fsh \
    phong.vsh

RESOURCES += \
    shaders.qrc
