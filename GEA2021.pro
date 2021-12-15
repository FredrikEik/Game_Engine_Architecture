QT          += core gui widgets opengl
QT          += core gui widgets qml

TEMPLATE    = app
CONFIG      += c++17

INCLUDEPATH += \
            ./include \
            ./gsl \
            ./OpenAL \
            ./help

PRECOMPILED_HEADER = help/PCH.h

TARGET      = GEA2021

mac {
    LIBS += -framework OpenAL
}

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
    enemy.cpp \
    gsl/matrix3x3.cpp \
    gsl/matrix4x4.cpp \
    gsl/vector2d.cpp \
    gsl/vector3d.cpp \
    gsl/vector4d.cpp \
    help/shader.cpp \
    help/texture.cpp \
    help/vertex.cpp \
    level.cpp \
    light.cpp \
    movementsystem.cpp \
    particlesystem.cpp \
    player.cpp \
    renderwindow.cpp \
    mainwindow.cpp \
    shapefactory.cpp \
    visualobject.cpp

HEADERS += \
    OpenAL/soundmanager.h \
    OpenAL/soundsource.h \
    OpenAL/wavfilehandler.h \
    camera.h \
    collisionsystem.h \
    component.h \
    enemy.h \
    gsl/constants.h \
    gsl/math_constants.h \
    gsl/matrix3x3.h \
    gsl/matrix4x4.h \
    gsl/vector2d.h \
    gsl/vector3d.h \
    gsl/vector4d.h \
    help/gltypes.h \
    help/PCH.h \
    help/shader.h \
    help/stb_image.h \
    help/texture.h \
    help/vertex.h \
    level.h \
    light.h \
    movementsystem.h \
    particlesystem.h \
    player.h \
    renderwindow.h \
    mainwindow.h \
    shapefactory.h \
    soundmanager.h \
    soundsource.h \
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
