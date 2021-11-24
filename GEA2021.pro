QT          += core gui widgets opengl \
            core qml
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
    ./Shaders \
    ./Saves


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
    lassurface.cpp \
    level.cpp \
    light.cpp \
    mariocube.cpp \
    objreader.cpp \
    octahedronball.cpp \
    plane.cpp \
    player.cpp \
    quadtree.cpp \
    renderwindow.cpp \
    rollingball.cpp \
    shader.cpp \
    mainwindow.cpp \
    SoundCode/scr/soundmanager.cpp \
    SoundCode/scr/soundsource.cpp \
    skybox.cpp \
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
    lassurface.h \
    level.h \
    light.h \
    mariocube.h \
    objreader.h \
    octahedronball.h \
    plane.h \
    player.h \
    quadtree.h \
    renderwindow.h \
    rollingball.h \
    shader.h \
    mainwindow.h \
    skybox.h \
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
    Shaders/skyboxfragment.frag \
    Shaders/skyboxvertex.vert \
    Shaders/textureshader.frag \
    Shaders/lightShader.frag \
    Shaders/lightShader.vert \
    Shaders/textureshader.frag \
    Shaders/textureshader.vert \
    Shaders/plainfragment.frag \
    Shaders/plainvertex.vert \
    Scripts/levelscript.js \
    Saves/testLevel.json \
    Saves/savedLevel.json
