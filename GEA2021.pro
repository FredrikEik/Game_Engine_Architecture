QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = GEA2021

SOURCES += main.cpp \
    camera.cpp \
    camerainputcomponent.cpp \
    component.cpp \
    gameobject.cpp \
    inputcomponent.cpp \
    inputsystem.cpp \
    matrix3x3.cpp \
    matrix4x4.cpp \
    player.cpp \
    playerinputcomponent.cpp \
    renderwindow.cpp \
    shader.cpp \
    mainwindow.cpp \
    shapefactory.cpp \
    texture.cpp \
    vector2d.cpp \
    vector3d.cpp \
    vertex.cpp \
    visualobject.cpp \
    xyz.cpp

HEADERS += \
    camera.h \
    camerainputcomponent.h \
    component.h \
    constants.h \
    gameobject.h \
    gltypes.h \
    input.h \
    inputcomponent.h \
    inputsystem.h \
    math_constants.h \
    matrix3x3.h \
    matrix4x4.h \
    player.h \
    playerinputcomponent.h \
    renderwindow.h \
    shader.h \
    mainwindow.h \
    shapefactory.h \
    texture.h \
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
