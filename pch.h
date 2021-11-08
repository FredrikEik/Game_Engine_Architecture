#ifndef PCH_H
#define PCH_H

//STL
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <ctime>


//Qt
#include <QObject>
#include <QOpenGLFunctions_4_1_Core>
#include <QMainWindow>
#include <QWindow>
#include <QOpenGLFunctions>
#include <QTimer>
#include <QElapsedTimer>
#include <QWidget>
#include <QDebug>
#include <QSurfaceFormat>
#include <QScreen>          //for resizing the program at start
#include <QMessageBox>      //For Help menu messages
#include <QTreeWidgetItem>  //Scene Outliner
#include <QStatusBar>
#include <QKeyEvent>
#include <QColorDialog>
#include <QTreeWidget>

//own
#include "input.h"
#include "altypes.h"
#include "gltypes.h"
#include "matrix3x3.h"
#include "matrix4x4.h"
#include "vector3d.h"
#include "vector2d.h"
#include "vector4d.h"
#include "math_constants.h"
#include "components.h"
#include "vertex.h"
#include "constants.h"
#include "logger.h"

#endif // PCH_H
