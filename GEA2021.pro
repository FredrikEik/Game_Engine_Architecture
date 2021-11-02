QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = GEA2021

INCLUDEPATH += \
    ./gsl \
    ./Assets \
    ./include

mac {
    LIBS += -framework OpenAL
}

win32 {
    INCLUDEPATH += $(OPENAL_HOME)\\include\\AL
    LIBS *= $(OPENAL_HOME)\\libs\\Win64\\libOpenAL32.dll.a
}

SOURCES += main.cpp \
    MathStuff/MathStuff.cpp \
    camera.cpp \
    cube.cpp \
    gameobject.cpp \
    gsl/matrix3x3.cpp \
    gsl/matrix4x4.cpp \
    mousepicker.cpp \
    objectfactory.cpp \
    objimport.cpp \
    player.cpp \
    renderwindow.cpp \
    shader.cpp \
    mainwindow.cpp \
    soundmanager.cpp \
    soundsource.cpp \
    spawner.cpp \
    texture.cpp \
    triangle.cpp \
    gsl/vector2d.cpp \
    gsl/vector3d.cpp \
    uimanager.cpp \
    vector3.cpp \
    vertex.cpp \
    visualobject.cpp \
    wavfilehandler.cpp \
    xyz.cpp

HEADERS += \
    MathStuff/MathStuff.h \
    camera.h \
    components.h \
    constants.h \
    cube.h \
    gameobject.h \
    gltypes.h \
    input.h \
    gsl/math_constants.h \
    gsl/matrix3x3.h \
    gsl/matrix4x4.h \
    mousepicker.h \
    objectfactory.h \
    objimport.h \
    player.h \
    renderwindow.h \
    shader.h \
    mainwindow.h \
    soundmanager.h \
    soundsource.h \
    spawner.h \
    texture.h \
    triangle.h \
    gsl/vector2d.h \
    gsl/vector3d.h \
    uimanager.h \
    vector3.h \
    vertex.h \
    visualobject.h \
    wavfilehandler.h \
    xyz.h

FORMS += \
    mainwindow.ui \
    transform.ui

DISTFILES += \
    Shaders/mousepicker.frag \
    Shaders/mousepicker.vert \
    Shaders/textureshader.frag \
    Shaders/textureshader.vert \
    Shaders\plainfragment.frag \
    Shaders\plainvertex.vert
