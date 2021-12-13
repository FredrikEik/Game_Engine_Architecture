QT          += core gui widgets opengl
QT          += core gui widgets qml

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
    enemy.cpp \
    level.cpp \
    light.cpp \
    matrix3x3.cpp \
    matrix4x4.cpp \
    movementsystem.cpp \
    player.cpp \
    renderwindow.cpp \
    shader.cpp \
    mainwindow.cpp \
    shapefactory.cpp \
    skybox.cpp \
    soundmanager.cpp \
    soundsource.cpp \
    texture.cpp \
    vector2d.cpp \
    vector3d.cpp \
    vector4d.cpp \
    vertex.cpp \
    visualobject.cpp \
    wavfilehandler.cpp

HEADERS += \
    PCH.h \
    camera.h \
    collisionsystem.h \
    component.h \
    constants.h \
    enemy.h \
    gltypes.h \
    input.h \
    level.h \
    light.h \
    math_constants.h \
    matrix3x3.h \
    matrix4x4.h \
    movementcomponent.h \
    movementsystem.h \
    player.h \
    renderwindow.h \
    shader.h \
    mainwindow.h \
    shapefactory.h \
    skybox.h \
    soundmanager.h \
    soundsource.h \
    stb_image.h \
    texture.h \
    vector2d.h \
    vector3d.h \
    vector4d.h \
    vertex.h \
    visualobject.h \
    wavfilehandler.h

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
