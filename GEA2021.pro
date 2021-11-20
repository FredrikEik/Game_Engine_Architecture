QT          += core gui widgets opengl qml

TEMPLATE    = app
CONFIG      += c++17 precomile_header

PRECOMPILED_HEADER = pch.h

TARGET      = GEA2021

RC_ICONS    = INNgine.ico


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

# Copy required DLLs to output directory
    contains(QT_ARCH, x86_64)
    {
        CONFIG(debug, debug|release) {
            OpenAL32.commands = copy /Y \"$(OPENAL_HOME)\\bin\\Win64\\OpenAL32.dll\" debug
            OpenAL32.target = debug/OpenAL32.dll

            QMAKE_EXTRA_TARGETS += OpenAL32
            PRE_TARGETDEPS += debug/OpenAL32.dll
        } else:CONFIG(release, debug|release) {
            OpenAL32.commands = copy /Y \"$(OPENAL_HOME)\\bin\\Win64\\OpenAL32.dll\" release
            OpenAL32.target = release/OpenAL32.dll

            QMAKE_EXTRA_TARGETS += OpenAL32
            PRE_TARGETDEPS += release/OpenAL32.dll release/OpenAL32.dll
        } else {
            error(Unknown set of dependencies.)
        }
    }
}

SOURCES += main.cpp \
    ResourceManager/meshhandler.cpp \
    ResourceManager/shaderhandler.cpp \
    ResourceManager/soundhandler.cpp \
    ResourceManager/texturehandler.cpp \
    ResourceManager/gameobjectmanager.cpp \
    ResourceManager/physicshandler.cpp \
    coreengine.cpp \
    gameplaymechanics.cpp \
    gsl/vector4d.cpp \
    heigthmap.cpp \
    logger.cpp \
    pch.cpp \
    rendersystem.cpp \
    soundsource.cpp \
    soundsystem.cpp \
    camera.cpp \
    gsl/matrix3x3.cpp \
    gsl/matrix4x4.cpp \
    mainwindow.cpp \
    gsl/vector2d.cpp \
    gsl/vector3d.cpp \
    vertex.cpp \
    widgetmaterial.cpp \
    widgettransform.cpp

HEADERS += \
    ResourceManager/meshhandler.h \
    ResourceManager/physicshandler.h \
    ResourceManager/shaderhandler.h \
    ResourceManager/soundhandler.h \
    ResourceManager/texturehandler.h \
    ResourceManager/gameobjectmanager.h \
    altypes.h \
    coreengine.h \
    gameplaymechanics.h \
    gsl/vector4d.h \
    heigthmap.h \
    logger.h \
    pch.h \
    rendersystem.h \
    soundsource.h \
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
    vertex.h \
    widgetmaterial.h \
    widgettransform.h

FORMS += \
    mainwindow.ui \
    widgetmaterial.ui \
    widgettransform.ui
    widgettransform.ui
    widgetmaterial.ui

DISTFILES += \
    Shaders/plainshader.frag \
    Shaders/plainshader.vert \
    Shaders/skybox.frag \
    Shaders/skybox.vert \
    Shaders/textureshader.frag \
    Shaders/textureshader.vert \
    Assets/materials.json

RESOURCES += \
    AppIconResources.qrc

OTHER_FILES += \
    SceneScript.js
