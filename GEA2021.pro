QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = GEA2021

SOURCES += main.cpp \
    camera.cpp \
    collisionshapes.cpp \
    collisionsystem.cpp \
    components.cpp \
    gameobject.cpp \
    matrix3x3.cpp \
    matrix4x4.cpp \
    renderwindow.cpp \
    resourcemanageer.cpp \
    shader.cpp \
    mainwindow.cpp \
    texture.cpp \
    triangle.cpp \
    triangletest.cpp \
    vector2d.cpp \
    vector3d.cpp \
    vertex.cpp \
    visualobject.cpp \
    xyz.cpp

HEADERS += \
    camera.h \
    collisionshapes.h \
    collisionsystem.h \
    components.h \
    constants.h \
    gameobject.h \
    gltypes.h \
    input.h \
    math_constants.h \
    matrix3x3.h \
    matrix4x4.h \
    renderwindow.h \
    resourcemanageer.h \
    shader.h \
    mainwindow.h \
    texture.h \
    triangle.h \
    triangletest.h \
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
