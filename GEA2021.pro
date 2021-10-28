QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

INCLUDEPATH += .
INCLUDEPATH += ./include

PRECOMPILED_HEADER = PCH.h

TARGET      = GEA2021

mac {
    LIBS += -framework OpenAL
}

win32 {
    INCLUDEPATH += $(OPENAL_HOME)\\include\\AL
    LIBS *= $(OPENAL_HOME)\\libs\\Win64\\libOpenAL32.dll.a
}

SOURCES += main.cpp \
    camera.cpp \
    collisionsystem.cpp \
    component.cpp \
    inputsystem.cpp \
    matrix3x3.cpp \
    matrix4x4.cpp \
    player.cpp \
    renderwindow.cpp \
    resourcemanager.cpp \
    shader.cpp \
    mainwindow.cpp \
    shapefactory.cpp \
    soundmanager.cpp \
    soundsource.cpp \
    texture.cpp \
    vector2d.cpp \
    vector3d.cpp \
    vector4d.cpp \
    vertex.cpp \
    visualobject.cpp \
    wavfilehandler.cpp \
    xyz.cpp

HEADERS += \
    PCH.h \
    camera.h \
    collisionsystem.h \
    component.h \
    constants.h \
    gltypes.h \
    input.h \
    inputcomponent.h \
    inputsystem.h \
    math_constants.h \
    matrix3x3.h \
    matrix4x4.h \
    player.h \
    renderwindow.h \
    resourcemanager.h \
    shader.h \
    mainwindow.h \
    shapefactory.h \
    soundmanager.h \
    soundsource.h \
    texture.h \
    vector2d.h \
    vector3d.h \
    vector4d.h \
    vertex.h \
    visualobject.h \
    wavfilehandler.h \
    xyz.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    Shaders/textureshader.frag \
    Shaders/textureshader.vert \
    Shaders\plainfragment.frag \
    Shaders\plainvertex.vert
