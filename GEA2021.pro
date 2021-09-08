QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = GEA2021

SOURCES += main.cpp \
    Components.cpp \
    GameObject.cpp \
    camera.cpp \
    cube.cpp \
    matrix3x3.cpp \
    matrix4x4.cpp \
    renderwindow.cpp \
    shader.cpp \
    mainwindow.cpp \
    sphere.cpp \
    texture.cpp \
    triangle.cpp \
    vector2d.cpp \
    vector3d.cpp \
    vertex.cpp \
    xyz.cpp

HEADERS += \
    Components.h \
    GameObject.h \
    camera.h \
    constants.h \
    cube.h \
    gltypes.h \
    input.h \
    math_constants.h \
    matrix3x3.h \
    matrix4x4.h \
    renderwindow.h \
    shader.h \
    mainwindow.h \
    sphere.h \
    texture.h \
    triangle.h \
    vector2d.h \
    vector3d.h \
    vertex.h \
    xyz.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    Shaders/textureshader.frag \
    Shaders/textureshader.vert \
    Shaders\plainfragment.frag \
    Shaders\plainvertex.vert
