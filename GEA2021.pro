QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = GEA2021

INCLUDEPATH += \
    ./gsl \
    ./Shaders \
    ./openal \
    ./libs/nlohmann \
    ./systems \
    ./helper


win32 {
    INCLUDEPATH += $(OPENAL_HOME)\include\AL
    LIBS *= $(OPENAL_HOME)\libs\Win64\libOpenAL32.dll.a
}

SOURCES += main.cpp \
    camera.cpp \
    gsl/matrix3x3.cpp \
    gsl/matrix4x4.cpp \
    helper/cameracomponent.cpp \
    helper/components.cpp \
    helper/shader.cpp \
    helper/texture.cpp \
    helper/vertex.cpp \
    openal/soundmanager.cpp \
    openal/soundsource.cpp \
    openal/vector3.cpp \
    openal/wavfilehandler.cpp \
    renderwindow.cpp \
    mainwindow.cpp \
    systems/collisionsystem.cpp \
    systems/entitysystem.cpp \
    systems/jsonsystem.cpp \
    systems/physicssystem.cpp \
    systems/rendersystem.cpp \
    systems/resourcesystem.cpp \
    gsl/vector2d.cpp \
    gsl/vector3d.cpp

HEADERS += \
    camera.h \
    helper/cameracomponent.h \
    helper/components.h \
    helper/constants.h \
    helper/gltypes.h \
    helper/input.h \
    helper/shader.h \
    helper/texture.h \
    helper/vertex.h \
    gsl/math_constants.h \
    gsl/matrix3x3.h \
    gsl/matrix4x4.h \
    openal/soundmanager.h \
    openal/soundsource.h \
    openal/vector3.h \
    openal/wavfilehandler.h \
    renderwindow.h \
    mainwindow.h \
    systems/collisionsystem.h \
    systems/entitysystem.h \
    systems/jsonsystem.h \
    systems/physicssystem.h \
    systems/rendersystem.h \
    systems/resourcesystem.h \
    gsl/vector2d.h \
    gsl/vector3d.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    Shaders/Skyboxshader.frag \
    Shaders/Skyboxshader.vert \
    Shaders/phongfragment.frag \
    Shaders/phongvertex.vert \
    Shaders/textureshader.frag \
    Shaders/textureshader.vert \
    Shaders/plainfragment.frag \
    Shaders/plainvertex.vert
