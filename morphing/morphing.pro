QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        cube.cpp \
        cubeedges.cpp \
        main.cpp \
        mainwidget.cpp \
        plainobject3d.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    cube.h \
    cubeedges.h \
    cubicobject.h \
    mainwidget.h \
    plainobject3d.h

DISTFILES += \
    shaders/fragment.fsh \
    shaders/vertex.vsh \
    shaders/vertex_shader.vs
