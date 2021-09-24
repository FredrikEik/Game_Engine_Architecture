QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = GEA2021

INCLUDEPATH += \
    ./gsl \
    ./SoundCode/h \
    ./SoundCode/scr \
    ./Shaders


mac {
    LIBS += -framework OpenAL
}

win32 {
    INCLUDEPATH += $(OPENAL_HOME)\\include\\AL
    LIBS *= $(OPENAL_HOME)\\libs\\Win64\\libOpenAL32.dll.a
}


SOURCES += main.cpp \
    bigplane.cpp \
    camera.cpp \
    components.cpp \
    cube.cpp \
    factory.cpp \
    gameobject.cpp \
    gsl/matrix3x3.cpp \
    gsl/matrix4x4.cpp \
    mariocube.cpp \
    objreader.cpp \
    plane.cpp \
    renderwindow.cpp \
    shader.cpp \
    mainwindow.cpp \
    SoundCode/scr/soundmanager.cpp \
    SoundCode/scr/soundsource.cpp \
    texture.cpp \
    gsl/vector2d.cpp \
    gsl/vector3d.cpp \
    SoundCode/scr/vector3.cpp \
    vertex.cpp \
    SoundCode/scr/wavfilehandler.cpp \
    xyz.cpp \
    triangle.cpp


HEADERS += \
    bigplane.h \
    camera.h \
    components.h \
    constants.h \
    cube.h \
    factory.h \
    gameobject.h \
    gltypes.h \
    input.h \
    gsl/math_constants.h \
    gsl/matrix3x3.h \
    gsl/matrix4x4.h \
    mariocube.h \
    objreader.h \
    plane.h \
    renderwindow.h \
    shader.h \
    mainwindow.h \
    texture.h \
    gsl/vector2d.h \
    gsl/vector3d.h \
    vertex.h \
    xyz.h \
    triangle.h \
    SoundCode/h/vector3.h \
    SoundCode/h/wavfilehandler.h \
    SoundCode/h/soundmanager.h \
    SoundCode/h/soundsource.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    Shaders/textureshader.frag \
    Shaders/textureshader.vert \
    Shaders/plainfragment.frag \
    Shaders/plainvertex.vert
