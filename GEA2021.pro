QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17
CONFIG      += precompile_header

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER  = PreCompiledHeaders.h

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
    camera.cpp \
    components.cpp \
    cube.cpp \
    detailswidget.cpp \
    factory.cpp \
    gameobject.cpp \
    gsl/matrix3x3.cpp \
    gsl/matrix4x4.cpp \
    gsl/vector4d.cpp \
    mariocube.cpp \
    objreader.cpp \
    plane.cpp \
    quadtree.cpp \
    renderwindow.cpp \
    shader.cpp \
    mainwindow.cpp \
    SoundCode/scr/soundmanager.cpp \
    SoundCode/scr/soundsource.cpp \
    sphere.cpp \
    texture.cpp \
    gsl/vector2d.cpp \
    gsl/vector3d.cpp \
    SoundCode/scr/vector3.cpp \
    vertex.cpp \
    SoundCode/scr/wavfilehandler.cpp \
    xyz.cpp \
    triangle.cpp


HEADERS += \
    PreCompiledHeaders.h \
    camera.h \
    components.h \
    constants.h \
    cube.h \
    detailswidget.h \
    factory.h \
    gameobject.h \
    gltypes.h \
    gsl/vector4d.h \
    input.h \
    gsl/math_constants.h \
    gsl/matrix3x3.h \
    gsl/matrix4x4.h \
    mariocube.h \
    objreader.h \
    plane.h \
    quadtree.h \
    renderwindow.h \
    shader.h \
    mainwindow.h \
    sphere.h \
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
    detailswidget.ui \
    mainwindow.ui

DISTFILES += \
    Shaders/textureshader.frag \
    Shaders/textureshader.vert \
    Shaders/plainfragment.frag \
    Shaders/plainvertex.vert
