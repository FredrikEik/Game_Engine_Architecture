QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = GEA2021

SOURCES += main.cpp \
    camera.cpp \
    component.cpp \
    graphics.cpp \
    graphicscomponent.cpp \
    inputcomponent.cpp \
    matrix3x3.cpp \
    matrix4x4.cpp \
    physicscomponent.cpp \
    playerinputcomponent.cpp \
    renderwindow.cpp \
    shader.cpp \
    mainwindow.cpp \
    texture.cpp \
    triangle.cpp \
    vector2d.cpp \
    vector3d.cpp \
    vertex.cpp \
    visualobject.cpp \
    xyz.cpp

HEADERS += \
    camera.h \
    component.h \
    constants.h \
    gltypes.h \
    graphics.h \
    graphicscomponent.h \
    input.h \
    inputcomponent.h \
    math_constants.h \
    matrix3x3.h \
    matrix4x4.h \
    physicscomponent.h \
    playerinputcomponent.h \
    renderwindow.h \
    shader.h \
    mainwindow.h \
    texture.h \
    triangle.h \
    vector2d.h \
    vector3d.h \
    vertex.h \
    visualobject.h \
    xyz.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    Shaders/textureshader.frag \
    Shaders/textureshader.vert \
    Shaders\plainfragment.frag \
    Shaders\plainvertex.vert
