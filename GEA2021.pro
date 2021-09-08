QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = GEA2021

SOURCES += main.cpp \
    camera.cpp \
    objects/cube.cpp \
    matrix3x3.cpp \
    matrix4x4.cpp \
    objects/projectile.cpp \
    objects/objmesh.cpp \
    renderwindow.cpp \
    systems/ecs/entity.cpp \
    systems/ecs/resourcemanager.cpp \
    shader.cpp \
    mainwindow.cpp \
    texture.cpp \
    objects/triangle.cpp \
    vector2d.cpp \
    vector3d.cpp \
    vertex.cpp \
    visualobject.cpp \
    objects/xyz.cpp \
    widget.cpp \
    widgettransform.cpp

HEADERS += \
    systems/ecs/Components.h \
    camera.h \
    constants.h \
    objects/cube.h \
    gltypes.h \
    input.h \
    math_constants.h \
    matrix3x3.h \
    matrix4x4.h \
    objects/projectile.h \
    objects/objmesh.h \
    renderwindow.h \
    systems/ecs/entity.h \
    systems/ecs/resourcemanager.h \
    shader.h \
    mainwindow.h \
    texture.h \
    objects\triangle.h \
    vector2d.h \
    vector3d.h \
    vertex.h \
    visualobject.h \
    objects/xyz.h \
    widget.h \
    widgettransform.h

FORMS += \
    WidgetTransform.ui \
    mainwindow.ui \
    widgettransform.ui

DISTFILES += \
    Shaders/textureshader.frag \
    Shaders/textureshader.vert \
    Shaders\plainfragment.frag \
    Shaders\plainvertex.vert
