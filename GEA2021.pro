QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = GEA2021

INCLUDEPATH += \
   ./gsl \
   ./Shaders

win32 {
   INCLUDEPATH += $(OPENAL_HOME)\\include\\AL
   LIBS *= $(OPENAL_HOME)\\libs\\Win64\\libOpenAL32.dll.a
}

SOURCES += main.cpp \
    ResourceManager.cpp \
    coreengine.cpp \
    soundsource.cpp \
    soundsource.cpp \
    vector4d.cpp \
    wavfilehandler.cpp \
    wavfilehandler.cpp \
    GameObject.cpp \
    camera.cpp \
    matrix3x3.cpp \
    matrix4x4.cpp \
    renderwindow.cpp \
    shader.cpp \
    mainwindow.cpp \
    soundmanager.cpp \
    texture.cpp \
    vector2d.cpp \
    vector3d.cpp \
    vertex.cpp

HEADERS += \
    Components.h \
    ResourceManager.h \
    coreengine.h \
    soundsource.h \
    soundsource.h \
    vector4d.h \
    wavfilehandler.h \
    wavfilehandler.h \
    GameObject.h \
    camera.h \
    constants.h \
    gltypes.h \
    input.h \
    math_constants.h \
    matrix3x3.h \
    matrix4x4.h \
    renderwindow.h \
    shader.h \
    mainwindow.h \
    soundmanager.h \
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
