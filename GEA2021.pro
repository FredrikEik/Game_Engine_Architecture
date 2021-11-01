QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = GEA2021

SOURCES += main.cpp \
    camera.cpp \
    cubemap.cpp \
    cubemapshader.cpp \
    entitymanager.cpp \
    interactiveobject.cpp \
    lightsource.cpp \
    mathfunctions.cpp \
    matrix3x3.cpp \
    matrix4x4.cpp \
    phongshader.cpp \
    plainshader.cpp \
    plane.cpp \
    renderwindow.cpp \
    resourcemanager.cpp \
    shader.cpp \
    mainwindow.cpp \
    skybox.cpp \
    texture.cpp \
    textureshader.cpp \
    vertex.cpp \
    visualobject.cpp \
    xyz.cpp

HEADERS += \
    ECScoordinator.h \
    camera.h \
    compmanager.h \
    componentarray.h \
    components.h \
    constants.h \
    cubemap.h \
    cubemapshader.h \
    entitymanager.h \
    gltypes.h \
    input.h \
    interactiveobject.h \
    lightsource.h \
    math_constants.h \
    mathfunctions.h \
    matrix3x3.h \
    matrix4x4.h \
    phongshader.h \
    plainshader.h \
    plane.h \
    renderwindow.h \
    resourcemanager.h \
    shader.h \
    mainwindow.h \
    skybox.h \
    systembase.h \
    systemmanager.h \
    texture.h \
    textureshader.h \
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
