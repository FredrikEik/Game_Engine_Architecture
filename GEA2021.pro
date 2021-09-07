QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = GEA2021

INCLUDEPATH += \
    ./gsl \
    ./Shaders

SOURCES += main.cpp \
    camera.cpp \
    components.cpp \
    cube.cpp \
    gameobject.cpp \
    gsl/matrix3x3.cpp \
    gsl/matrix4x4.cpp \
    mariocube.cpp \
    objreader.cpp \
    plane.cpp \
    renderwindow.cpp \
    shader.cpp \
    mainwindow.cpp \
    texture.cpp \
    gsl/vector2d.cpp \
    gsl/vector3d.cpp \
    vertex.cpp \
    xyz.cpp

HEADERS += \
    camera.h \
    components.h \
    constants.h \
    cube.h \
    gameobject.h \
    gltypes.h \
    input.h \
    gsl/math_constants.h \
    gsl/matrix3x3.h \
    gsl/matrix4x4.h \
    mariocube.h \
    objreader.h \
    plane.h \
    renderwindow.h \
    shader.h \
    mainwindow.h \
    texture.h \
    gsl/vector2d.h \
    gsl/vector3d.h \
    vertex.h \
    xyz.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    Shaders/textureshader.frag \
    Shaders/textureshader.vert \
    Shaders/plainfragment.frag \
    Shaders/plainvertex.vert
