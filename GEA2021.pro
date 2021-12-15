QT          += core gui widgets opengl
QT          += core gui widgets qml

TEMPLATE    = app
CONFIG      += c++17

TARGET      = GEA2021

            #stop std::fopen deprecation warning
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

#INCLUDEPATH += C:\\prog\\openAL\\openal-soft-1.21.1-bin\\openal-soft-1.21.1-bin\\include\\AL
#LIBS *= C:\\prog\\openAL\\openal-soft-1.21.1-bin\\openal-soft-1.21.1-bin\\libs\\Win64\\libOpenAL32.dll.a
}

SOURCES += main.cpp \
    ResourceManager/colorshader.cpp \
    ResourceManager/mousepickershader.cpp \
    ResourceManager/particleshader.cpp \
    camera.cpp \
    coreengine.cpp \
    gsl/matrix3x3.cpp \
    gsl/matrix4x4.cpp \
    gsl/vector2d.cpp \
    gsl/vector3d.cpp \
    ResourceManager/resourcemanager.cpp \
    ResourceManager/shaderhandler.cpp \
    ResourceManager/soundhandler.cpp \
    ResourceManager/texturehandler.cpp \
    ResourceManager/meshhandler.cpp \
    gsl/vector4d.cpp \
    light.cpp \
    particle.cpp \
    rendersystem.cpp \
    mainwindow.cpp \
    scene.cpp \
    scriptsystem.cpp \
    skybox.cpp \
    soundsystem.cpp \
    soundsource.cpp \
    texture.cpp \
    textureshader.cpp \
    triangle.cpp \
    vertex.cpp \
    gameobject.cpp \
    xyz.cpp

HEADERS += \
    ResourceManager/colorshader.h \
    ResourceManager/entity.h \
    ResourceManager/mousepickershader.h \
    ResourceManager/particleshader.h \
    altypes.h \
    camera.h \
    components.h \
    constants.h \
    coreengine.h \
    gltypes.h \
    gsl/vector4d.h \
    input.h \
    gsl/math_constants.h \
    gsl/matrix3x3.h \
    gsl/matrix4x4.h \
    gsl/vector2d.h \
    gsl/vector3d.h \
    ResourceManager/resourcemanager.h \
    ResourceManager/shaderhandler.h \
    ResourceManager/soundhandler.h \
    ResourceManager/texturehandler.h \
    ResourceManager/meshhandler.h \
    light.h \
    particle.h \
    rendersystem.h \
    mainwindow.h \
    scene.h \
    scriptsystem.h \
    skybox.h \
    soundsystem.h \
    soundsource.h \
    texture.h \
    textureshader.h \
    triangle.h \
    vertex.h \
    gameobject.h \
    xyz.h

FORMS += \
    mainwindow.ui

DISTFILES += \
#    Enemy.js \
    Shaders/colorshader.frag \
    Shaders/colorshader.vert \
    Shaders/mousepickershader.frag \
    Shaders/mousepickershader.vert \
    Shaders/particle.frag \
    Shaders/particle.vert \
    Shaders/skybox.frag \
    Shaders/skybox.vert \
    Shaders/textureshader.frag \
    Shaders/textureshader.vert \
    Shaders/plainfragment.frag \
    Shaders/plainvertex.vert \
    mousepickershader.frag \
    mousepickershader.vert \
#    player.js

OTHER_FILES += \
    Enemy.js \
    player.js
