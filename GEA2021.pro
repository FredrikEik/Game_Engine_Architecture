QT          += core gui widgets opengl
QT          += core gui widgets qml

TEMPLATE    = app
CONFIG      += c++17

INCLUDEPATH += \
            ./include \
            ./gsl \
            ./OpenAL

PRECOMPILED_HEADER = PCH.h

TARGET      = GEA2021

win32 {
    INCLUDEPATH += $(OPENAL_HOME)\\include\\AL
    LIBS *= $(OPENAL_HOME)\\libs\\Win64\\libOpenAL32.dll.a
}

SOURCES += main.cpp \
    OpenAL/soundmanager.cpp \
    OpenAL/soundsource.cpp \
    OpenAL/wavfilehandler.cpp \
    camera.cpp \
    collisionsystem.cpp \
    component.cpp \
    enemy.cpp \
    gsl/matrix3x3.cpp \
    gsl/matrix4x4.cpp \
    gsl/vector2d.cpp \
    gsl/vector3d.cpp \
    gsl/vector4d.cpp \
    level.cpp \
    light.cpp \
    movementsystem.cpp \
    particlesystem.cpp \
    player.cpp \
    renderwindow.cpp \
    shader.cpp \
    mainwindow.cpp \
    shapefactory.cpp \
    texture.cpp \
    vertex.cpp \
    visualobject.cpp

HEADERS += \
    OpenAL/soundmanager.h \
    OpenAL/soundsource.h \
    OpenAL/wavfilehandler.h \
    PCH.h \
    camera.h \
    collisionsystem.h \
    component.h \
    enemy.h \
    gltypes.h \
    gsl/constants.h \
    gsl/math_constants.h \
    gsl/matrix3x3.h \
    gsl/matrix4x4.h \
    gsl/vector2d.h \
    gsl/vector3d.h \
    gsl/vector4d.h \
    input.h \
    level.h \
    light.h \
    movementsystem.h \
    particlesystem.h \
    player.h \
    renderwindow.h \
    shader.h \
    mainwindow.h \
    shapefactory.h \
    stb_image.h \
    texture.h \
    vertex.h \
    visualobject.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    Shaders/phongfragment.frag \
    Shaders/phongvertex.vert \
    Shaders/skyboxfragment.frag \
    Shaders/skyboxvertex.vert \
    Shaders/textureshader.frag \
    Shaders/textureshader.vert \
    Shaders\plainfragment.frag \
    Shaders\plainvertex.vert
