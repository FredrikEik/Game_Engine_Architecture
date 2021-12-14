#ifndef PCH_H
#define PCH_H
#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QElapsedTimer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>
#include <iostream>

#include "input.h"
#include "constants.h"
#include "movementsystem.h"
#include "component.h"
#include "shapefactory.h"
#include "soundmanager.h"
#include "soundsource.h"

#include "shader.h"
#include "mainwindow.h"
#include "camera.h"
#include "constants.h"
#include "texture.h"
#include "light.h"

/**
@class PCH - PreCompiledHeader kompilerer filer som ikke skal endres. Dette gjør at kompilering går raskere..
 */

class QOpenGLContext;
class Shader;
class MainWindow;
class VisualObject;
class Camera;
class Texture;

#endif // PCH_H