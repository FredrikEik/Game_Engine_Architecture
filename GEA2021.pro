QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = GEA2021

SOURCES += main.cpp \
    camera.cpp \
    componentarray.cpp \
    componentmanager.cpp \
    componenttypes.cpp \
    ecs.cpp \
    entitymanager.cpp \
    matrix3x3.cpp \
    matrix4x4.cpp \
    rendersystem.cpp \
    resourcemanager.cpp \
    shader.cpp \
    mainwindow.cpp \
    texture.cpp \
    vector2d.cpp \
    vector3d.cpp \
    vertex.cpp

HEADERS += \
    camera.h \
    componentarray.h \
    componentmanager.h \
    componenttypes.h \
    constants.h \
    ecs.h \
    entitymanager.h \
    gltypes.h \
    input.h \
    math_constants.h \
    matrix3x3.h \
    matrix4x4.h \
    rendersystem.h \
    resourcemanager.h \
    shader.h \
    mainwindow.h \
    texture.h \
    vector2d.h \
    vector3d.h \
    vertex.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    Shaders/textureshader.frag \
    Shaders/textureshader.vert \
    Shaders\plainfragment.frag \
    Shaders\plainvertex.vert
