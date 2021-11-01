QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = GEA2021

SOURCES += main.cpp \
    ECS/ResourceManager/resourcemanager.cpp \
    ECS/entitymanager.cpp \
    ECS/resourcemanager.cpp \
    Shaders/cubemapshader.cpp \
    Shaders/phongshader.cpp \
    Shaders/plainshader.cpp \
    Shaders/shader.cpp \
    Shaders/textureshader.cpp \
    camera.cpp \
    cubemap.cpp \
    interactiveobject.cpp \
    lightsource.cpp \
    mathfunctions.cpp \
    plane.cpp \
    renderwindow.cpp \
    mainwindow.cpp \
    skybox.cpp \
    texture.cpp \
    vertex.cpp \
    visualobject.cpp \

HEADERS += \
    ECS/ECScoordinator.h \
    ECS/ResourceManager/resourcemanager.h \
    ECS/compmanager.h \
    ECS/componentarray.h \
    ECS/entitymanager.h \
    ECS/resourcemanager.h \
    ECS/systembase.h \
    ECS/systemmanager.h \
    ECS/ECScoordinator.h \
    Shaders/cubemapshader.h \
    Shaders/phongshader.h \
    Shaders/plainshader.h \
    Shaders/shader.h \
    Shaders/textureshader.h \
    camera.h \
    components.h \
    constants.h \
    cubemap.h \
    gltypes.h \
    input.h \
    interactiveobject.h \
    lightsource.h \
    math_constants.h \
    mathfunctions.h \
    plane.h \
    renderwindow.h \
    mainwindow.h \
    skybox.h \
    texture.h \
    vertex.h \
    visualobject.h \

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
