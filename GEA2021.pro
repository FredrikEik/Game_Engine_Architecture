QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = GEA2021

RC_ICONS    = INNgine.ico

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
    camera.cpp \
    coreengine.cpp \
    gsl/matrix3x3.cpp \
    gsl/matrix4x4.cpp \
    gsl/vector2d.cpp \
    gsl/vector3d.cpp \
    gsl/vector4d.cpp \
    ResourceManager/resourcemanager.cpp \
    ResourceManager/shaderhandler.cpp \
    ResourceManager/soundhandler.cpp \
    ResourceManager/texturehandler.cpp \
    ResourceManager/meshhandler.cpp \
    rendersystem.cpp \
    mainwindow.cpp \
    soundsystem.cpp \
    soundsource.cpp \
    triangle.cpp \
    vertex.cpp \
    gameobject.cpp \
    widgettransform.cpp \
    xyz.cpp

HEADERS += \
    altypes.h \
    camera.h \
    components.h \
    constants.h \
    coreengine.h \
    gltypes.h \
    input.h \
    gsl/math_constants.h \
    gsl/matrix3x3.h \
    gsl/matrix4x4.h \
    gsl/vector2d.h \
    gsl/vector3d.h \
    gsl/vector4d.h \
    ResourceManager/resourcemanager.h \
    ResourceManager/shaderhandler.h \
    ResourceManager/soundhandler.h \
    ResourceManager/texturehandler.h \
    ResourceManager/meshhandler.h \
    rendersystem.h \
    mainwindow.h \
    soundsystem.h \
    soundsource.h \
    triangle.h \
    vertex.h \
    gameobject.h \
    widgettransform.h \
    xyz.h

FORMS += \
    mainwindow.ui \
    widgettransform.ui

DISTFILES += \
    Shaders/textureshader.frag \
    Shaders/textureshader.vert \
    Shaders/plainfragment.frag \
    Shaders/plainvertex.vert

RESOURCES += \
    AppIconResources.qrc
