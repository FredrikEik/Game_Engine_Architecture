#ifndef PCH_H
#define PCH_H

/**
   I PCH (PreCompiledHeader) inkluderer vi alle typiske includes
   som <iostream> og header-filer som vi ikke endrer på lenger.
 */

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


class QOpenGLContext;
class Shader;
class MainWindow;
class VisualObject;
class Camera;
class Texture;

#endif // PCH_H
