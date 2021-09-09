QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = GEA2021

SOURCES += main.cpp \
    camera.cpp \
    cubemapshader.cpp \
    entitymanager.cpp \
    interactiveobject.cpp \
    lightsource.cpp \
    matrix3x3.cpp \
    matrix4x4.cpp \
    phongshader.cpp \
    plainshader.cpp \
    plane.cpp \
    player.cpp \
    renderwindow.cpp \
    shader.cpp \
    mainwindow.cpp \
    texture.cpp \
    textureshader.cpp \
    triangle.cpp \
    vector2d.cpp \
    vector3d.cpp \
    vertex.cpp \
    visualobject.cpp \
    xyz.cpp

HEADERS += \
    camera.h \
    componentmanager.h \
    components.h \
    constants.h \
    cubemapshader.h \
    entitymanager.h \
    gltypes.h \
    input.h \
    interactiveobject.h \
    lightsource.h \
    math_constants.h \
    matrix3x3.h \
    matrix4x4.h \
    phongshader.h \
    plainshader.h \
    plane.h \
    renderwindow.h \
    shader.h \
    mainwindow.h \
    texture.h \
    textureshader.h \
    triangle.h \
    vector2d.h \
    vector3d.h \
    vertex.h \
    visualobject.h \
    xyz.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    Assets/Skybox/1.bmp \
    Assets/Skybox/2.bmp \
    Assets/Skybox/3.bmp \
    Assets/Skybox/4.bmp \
    Assets/Skybox/5.bmp \
    Assets/Skybox/6.bmp \
    Assets/Textures/doge.bmp \
    Assets/Textures/dogedoge.bmp \
    Assets/Textures/hm2.bmp \
    Assets/Textures/hund.bmp \
    Shaders/cubemap.frag \
    Shaders/cubemap.vert \
    Shaders/phong.frag \
    Shaders/phong.vert \
    Shaders/plainshader.frag \
    Shaders/plainshader.vert \
    Shaders/texture.frag \
    Shaders/texture.vert \
    Shaders/textureshader.frag \
    Shaders/textureshader.vert \
    Shaders\plainfragment.frag \
    Shaders\plainvertex.vert
