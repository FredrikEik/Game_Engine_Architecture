QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = GEA2021


DEFINES     += _CRT_SECURE_NO_WARNINGS

INCLUDEPATH += \
    ./gsl \
    ./Shaders \
    ./ResourceManager

macx {
    LIBS += -framework OpenAL

    # QT not updated for Xcode 11.
    CONFIG+=sdk_no_version_check
}

win32 {
    INCLUDEPATH += $(OPENAL_HOME)\\include\\AL
    LIBS *= $(OPENAL_HOME)\\libs\\Win64\\libOpenAL32.dll.a
}

SOURCES += main.cpp \
    ResourceManager/meshhandler.cpp \
    ResourceManager/shaderhandler.cpp \
    ResourceManager/soundhandler.cpp \
    ResourceManager/texturehandler.cpp \
    ResourceManager/gameobjectmanager.cpp \
    coreengine.cpp \
    rendersystem.cpp \
    soundsystem.cpp \
    camera.cpp \
    gsl/matrix3x3.cpp \
    gsl/matrix4x4.cpp \
    mainwindow.cpp \
    gsl/vector2d.cpp \
    gsl/vector3d.cpp \
    vertex.cpp

HEADERS += \
    ResourceManager/meshhandler.h \
    ResourceManager/shaderhandler.h \
    ResourceManager/soundhandler.h \
    ResourceManager/texturehandler.h \
    ResourceManager/gameobjectmanager.h \
    altypes.h \
    coreengine.h \
    rendersystem.h \
    soundsystem.h \
    camera.h \
    components.h \
    constants.h \
    gameobject.h \
    gltypes.h \
    input.h \
    gsl/math_constants.h \
    gsl/matrix3x3.h \
    gsl/matrix4x4.h \
    mainwindow.h \
    gsl/vector2d.h \
    gsl/vector3d.h \
    vertex.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    Shaders/textureshader.frag \
    Shaders/textureshader.vert \
    Shaders/plainfragment.frag \
    Shaders/plainvertex.vert
