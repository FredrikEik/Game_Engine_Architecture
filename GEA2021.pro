QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = GEA2021

INCLUDEPATH += \
    ./gsl \
    ./Shaders \
    ./ResourceManager

macx {
    LIBS += -framework OpenAL
}

win32 {
    INCLUDEPATH += $(OPENAL_HOME)\\include\\AL
    LIBS *= $(OPENAL_HOME)\\libs\\Win64\\libOpenAL32.dll.a
}

SOURCES += main.cpp \
    camera.cpp \
    components.cpp \
    gsl/matrix3x3.cpp \
    gsl/matrix4x4.cpp \
    ResourceManager/shader.cpp \
    ResourceManager/wavfilehandler.cpp \
    ResourceManager/resourcemanager.cpp \
    ResourceManager/texture.cpp \
    renderwindow.cpp \
    mainwindow.cpp \
    soundmanager.cpp \
    soundsource.cpp \
    triangle.cpp \
    gsl/vector2d.cpp \
    gsl/vector3d.cpp \
    vertex.cpp \
    gameobject.cpp \
    xyz.cpp

HEADERS += \
    camera.h \
    components.h \
    constants.h \
    gltypes.h \
    input.h \
    gsl/math_constants.h \
    gsl/matrix3x3.h \
    gsl/matrix4x4.h \
    ResourceManager/shader.h \
    ResourceManager/wavfilehandler.h \
    ResourceManager/resourcemanager.h \
    ResourceManager/texture.h \
    renderwindow.h \
    mainwindow.h \
    soundmanager.h \
    soundsource.h \
    triangle.h \
    gsl/vector2d.h \
    gsl/vector3d.h \
    vertex.h \
    gameobject.h \
    xyz.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    Shaders/textureshader.frag \
    Shaders/textureshader.vert \
    Shaders/plainfragment.frag \
    Shaders/plainvertex.vert
