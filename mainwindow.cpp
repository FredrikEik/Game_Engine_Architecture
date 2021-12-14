#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSurfaceFormat>
#include <QDebug>
#include <QScreen>  //for resizing the program at start

#include "renderwindow.h"
#include "soundmanager.h"
#include "ResourceManager.h"
#include "coreengine.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    //this sets up what's in the mainwindow.ui
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete mRenderWindow;
    delete ui;
}

/**
 * MainWindow::makeObjList Gets the names of all '.obj' files in the scene, and
 * inserts them into the list to the right of renderWindow in when running the program.
 */
void MainWindow::makeObjList(std::vector<GameObject *> mGameObjects)
{
    std::string name;
    for(int i = 0; i<mGameObjects.size(); i++){
        name = mGameObjects[i]->mName;
        if(name.find(".obj") != std::string::npos)
            name.erase(name.find(".obj"));
        ui->objList->addItem(QString::fromStdString(name));
    }
}

/**
 * MainWindow::updateAmmo Is called when the player shoots a bullet.
 * The function removes 1 bullet from the magazine, and updates the Bullet number in the HUD.
 */
void MainWindow::updateAmmo()
{
    if(ammo > 0)
    {
        ammo--;
    }
    ui->bulletCount->display(ammo);
}

void MainWindow::init()
{
    //This will contain the setup of the OpenGL surface we will render into
    QSurfaceFormat format;

    //OpenGL v 4.1 - (Ole Flatens Mac does not support higher than this - sorry!)
    //you can try other versions, but then have to update RenderWindow and Shader
    //to inherit from other than QOpenGLFunctions_4_1_Core
    format.setVersion(4, 1);
    //Using the main profile for OpenGL - no legacy code permitted
    format.setProfile(QSurfaceFormat::CoreProfile);
    //A QSurface can be other types than OpenGL
    format.setRenderableType(QSurfaceFormat::OpenGL);

    //This should activate OpenGL debug Context used in RenderWindow::startOpenGLDebugger().
    //This line (and the startOpenGLDebugger() and checkForGLerrors() in RenderWindow class)
    //can be deleted, but it is nice to have some OpenGL debug info!
    format.setOption(QSurfaceFormat::DebugContext);

    // The renderer will need a depth buffer - (not requiered to set in glfw-tutorials)
    format.setDepthBufferSize(24);

    //Set the number of samples used for multisampling
    //Same as glfwWindowHint(GLFW_SAMPLES, 4);
    //https://learnopengl.com/Advanced-OpenGL/Anti-Aliasing
    format.setSamples(4);

    //Just prints out what OpenGL format we try to get
    // - this can be deleted
    qDebug() << "Requesting surface format: " << format;

    //We have a format for the OpenGL window, so let's make it:
    mRenderWindow = new RenderWindow(format, this);

    //Check if renderwindow did initialize, else prints error and quit
    if (!mRenderWindow->context()) {
        qDebug() << "Failed to create context. Can not continue. Quits application!";
        delete mRenderWindow;
        return;
    }

    //The OpenGL RenderWindow got made, so continuing the setup:
    //We put the RenderWindow inside a QWidget so we can put in into a
    //layout that is made in the .ui-file
    mRenderWindowContainer = QWidget::createWindowContainer(mRenderWindow);
    //OpenGLLayout is made in the .ui-file!
    ui->OpenGLLayout->addWidget(mRenderWindowContainer);

    //Set the size of the program in % of the actual screen size
    QSize tempSize = QGuiApplication::primaryScreen()->size();

    tempSize.rheight() *= 0.85;
    tempSize.rwidth() *= 0.65;
    resize(tempSize);

    mCoreEngine = new CoreEngine(mRenderWindow);

    //sets the keyboard input focus to the RenderWindow when program starts
    // - can be deleted, but then you have to click inside the renderwindow to get the focus
    mRenderWindowContainer->setFocus();

}

/**
 * MainWindow::on_pushButton_toggled
 * Is called when 'Show Wireframe'/'Hide Wireframe' button is pressed.
 * Updates text on the button and calls a function to toggle the wireframe.
 */
void MainWindow::on_pushButton_toggled(bool checked)
{
    if(!mCoreEngine->isPlaying()){
        mRenderWindow->toggleWireframe(checked);
        if(checked)
            ui->pushButton->setText("Hide Wireframe");
        else
            ui->pushButton->setText("Show Wireframe");
    }
    else
        ui->pushButton->setText("Wireframe is only available in editor mode");
}

/**
 * MainWindow::on_startGameButton_toggled Is called when the 'start game'/'stop game'
 * button is pressed. The function updates button text and either resets calls a function
 * to reset the scene, or plays a 'start game' sound.
 */
void MainWindow::on_startGameButton_toggled(bool checked)
{
    mCoreEngine->swapCurrentCamera();
    if(checked){
        ui->startGameButton->setText("Stop Game / Open Editor");
        mCoreEngine->playStartGameSound();
        mRenderWindowContainer->setFocus();
    }
    else{
        ui->startGameButton->setText("Start Game");
        mCoreEngine->resetScene();
    }

    ui->pushButton->setText("Show Wireframe");
}

/**
 * MainWindow::on_LODButton_toggled Is called when the 'Enable LOD'/'Disable LOD' button is pressed.
 * The function updates button text and calls a function to toggle the LOD on/off.
 */
void MainWindow::on_LODButton_toggled(bool checked)
{
    mRenderWindow->toggleLOD();
    if(checked)
        ui->LODButton->setText("Enable LOD");
    else{
        ui->LODButton->setText("Disable LOD");
    }

}

/**
 * MainWindow::on_FrustumToggle_toggled Is called when the 'Enable Frustum'/'Disable Frustum' button is pressed.
 * The function updates button text and calls a function to toggle the Frustum culling on/off.
 */
void MainWindow::on_FrustumToggle_toggled(bool checked)
{
    mRenderWindow->toggleFrustum();
    if(checked)
        ui->FrustumToggle->setText("Enable FRUSTUM");
    else{
        ui->FrustumToggle->setText("Disable FRUSTUM");
    }
}

/**
 * MainWindow::on_CreateObjectButton_clicked is called when the 'Create object' button is pressed.
 * The function calls a function in coreEngine called 'CreateObjectButton' and sends in the name
 * of the object selected in the dropdown menu as a parameter.
 */
void MainWindow::on_CreateObjectButton_clicked()
{
    mCoreEngine->CreateObjectButton(spawnObject);
}

/**
 * MainWindow::on_ObjectDropdown_currentTextChanged is called when a new object is clicked in the
 * object dropdown menu. Function gets the name and adds '.obj' at the end of the string. If the
 * user now clicks the 'create object' button. This is the object it will create.
 */
void MainWindow::on_ObjectDropdown_currentTextChanged(const QString &arg1)
{
    spawnObject = arg1.toStdString() + ".obj";
}

/**
 * MainWindow::on_xPos_valueChanged is called when an objects x-position is changed using the
 * 'spinbox' for x position in the UI.
 * The function updates the position of the object with the new value, and also updates the position
 * of the selection arrow with the same values. (So it is placed above the object).
 */
void MainWindow::on_xPos_valueChanged(double value)
{
    int index = mRenderWindow->getSelectedObject();
    if(index != -1){
        gsl::Matrix4x4 *objMatrix = &mRenderWindow->getGameObjects()[index]->transform->mMatrix;
        gsl::Vector3D pos = objMatrix->getPosition();
        objMatrix->setPosition(value, pos.y, pos.z);

        pos.x = value;
        mCoreEngine->MoveSelectionArrow(pos);
    }
}

/**
 * MainWindow::on_yPos_valueChanged is called when an objects y-position is changed using the
 * 'spinbox' for y position in the UI.
 * The function updates the position of the object with the new value, and also updates the position
 * of the selection arrow with the same values. (So it is placed above the object).
 */
void MainWindow::on_yPos_valueChanged(double value)
{
    int index = mRenderWindow->getSelectedObject();
    if(index != -1){
        gsl::Matrix4x4 *objMatrix = &mRenderWindow->getGameObjects()[index]->transform->mMatrix;
        gsl::Vector3D pos = objMatrix->getPosition();
        objMatrix->setPosition(pos.x, value, pos.z);

        pos.y = value;
        mCoreEngine->MoveSelectionArrow(pos);
    }
}

/**
 * MainWindow::on_zPos_valueChanged is called when an objects z-position is changed using the
 * 'spinbox' for z position in the UI.
 * The function updates the position of the object with the new value, and also updates the position
 * of the selection arrow with the same values. (So it is placed above the object).
 */
void MainWindow::on_zPos_valueChanged(double value)
{
    int index = mRenderWindow->getSelectedObject();
    if(index != -1){
        gsl::Matrix4x4 *objMatrix = &mRenderWindow->getGameObjects()[index]->transform->mMatrix;
        gsl::Vector3D pos = objMatrix->getPosition();
        objMatrix->setPosition(pos.x, pos.y, value);

        pos.z = value;
        mCoreEngine->MoveSelectionArrow(pos);
    }
}

