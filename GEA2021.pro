QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = GEA2021

win32 {
    INCLUDEPATH += $(OPENAL_HOME)\\include\\AL
    LIBS *= $(OPENAL_HOME)\\libs\\Win64\\libOpenAL32.dll.a
}

SOURCES += main.cpp \
    camera.cpp \
    components.cpp \
    gameobject.cpp \
    matrix3x3.cpp \
    matrix4x4.cpp \
    objmesh.cpp \
    renderwindow.cpp \
    resourcemanager.cpp \
    shader.cpp \
    mainwindow.cpp \
    soundhandler.cpp \
    soundmanager.cpp \
    soundsource.cpp \
    texture.cpp \
    triangle.cpp \
    vector2d.cpp \
    vector3d.cpp \
    vertex.cpp \
    visualobject.cpp \
    xyz.cpp

HEADERS += \
    camera.h \
    components.h \
    constants.h \
    gameobject.h \
    gltypes.h \
    input.h \
    math_constants.h \
    matrix3x3.h \
    matrix4x4.h \
    objmesh.h \
    renderwindow.h \
    resourcemanager.h \
    shader.h \
    mainwindow.h \
    soundhandler.h \
    soundmanager.h \
    soundsource.h \
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
