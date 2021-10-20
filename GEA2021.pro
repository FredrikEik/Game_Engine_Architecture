QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = GEA2021

INCLUDEPATH += \
    ./gsl \
    ./Shaders \
    ./openal


win32 {
    INCLUDEPATH += $(OPENAL_HOME)\include\AL
    LIBS *= $(OPENAL_HOME)\libs\Win64\libOpenAL32.dll.a
}

SOURCES += main.cpp \
    camera.cpp \
    cameracomponent.cpp \
    collisionsystem.cpp \
    components.cpp \
    entitysystem.cpp \
    gsl/matrix3x3.cpp \
    gsl/matrix4x4.cpp \
    openal/soundmanager.cpp \
    openal/soundsource.cpp \
    openal/vector3.cpp \
    openal/wavfilehandler.cpp \
    physicssystem.cpp \
    rendersystem.cpp \
    renderwindow.cpp \
    resourcesystem.cpp \
    shader.cpp \
    mainwindow.cpp \
    texture.cpp \
    triangle.cpp \
    gsl/vector2d.cpp \
    gsl/vector3d.cpp \
    vertex.cpp \
    visualobject.cpp \
    xyz.cpp

HEADERS += \
    camera.h \
    cameracomponent.h \
    collisionsystem.h \
    components.h \
    constants.h \
    entitysystem.h \
    gltypes.h \
    input.h \
    gsl/math_constants.h \
    gsl/matrix3x3.h \
    gsl/matrix4x4.h \
    openal/soundmanager.h \
    openal/soundsource.h \
    openal/vector3.h \
    openal/wavfilehandler.h \
    physicssystem.h \
    rendersystem.h \
    renderwindow.h \
    resourcesystem.h \
    shader.h \
    mainwindow.h \
    texture.h \
    triangle.h \
    gsl/vector2d.h \
    gsl/vector3d.h \
    vertex.h \
    visualobject.h \
    xyz.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    Shaders/textureshader.frag \
    Shaders/textureshader.vert \
    Shaders/plainfragment.frag \
    Shaders/plainvertex.vert
