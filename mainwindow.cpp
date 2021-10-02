#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSurfaceFormat>
#include <QDebug>
#include <QScreen>  //for resizing the program at start
#include <QListWidget>

#include "renderwindow.h"
#include "gameengine.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    //this sets up what's in the mainwindow.ui
    ui->setupUi(this);

    listWidget = ui->listWidget;

    init();
}

MainWindow::~MainWindow()
{
    delete mRenderWindow;
    delete ui;
}

void MainWindow::initList()
{
    if(mRenderWindow)
    {
        GameObjects.clear();
        GameObjects = mRenderWindow->getAllGameObject();
        for(auto it : GameObjects)
        {
            new QListWidgetItem(tr((*it).name.c_str()), listWidget);
        }
    }

}

void MainWindow::updateList()
{
    if(mRenderWindow)
    {
        GameObjects.clear();
        GameObjects = mRenderWindow->getAllGameObject();
        new QListWidgetItem(tr( GameObjects[GameObjects.size()-1]->name.c_str() ), listWidget);
    }
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

    tempSize.rheight() *= 0.90; //original value 0.85
    tempSize.rwidth() *= 0.90; //original value 0.65
    resize(tempSize);

    GameEngine::getInstance()->setRenderPointer(mRenderWindow , this);

    //sets the keyboard input focus to the RenderWindow when program starts
    // - can be deleted, but then you have to click inside the renderwindow to get the focus
    mRenderWindowContainer->setFocus();

}

//Example of a slot called from the button on the top of the program.



void MainWindow::on_ToggleLOD_toggled(bool checked)
{
    mRenderWindow->renderLOD(checked);
    if(checked)
        ui->ToggleLOD->setText("LOD OFF");
    else
        ui->ToggleLOD->setText("LOD ON");
}


void MainWindow::on_pb_toggleWireframe_toggled(bool checked)
{
    mRenderWindow->toggleWireframe(checked);
    if(checked)
        ui->pb_toggleWireframe->setText("Show Solid");
    else
        ui->pb_toggleWireframe->setText("Show Wireframe");
}


void MainWindow::on_ToggleFrustumCulling_toggled(bool checked)
{
    mRenderWindow->toggleFrustumCulling(checked);
    if(checked)
        ui->ToggleFrustumCulling->setText("Frustum Culling OFF");
    else
        ui->ToggleFrustumCulling->setText("Frustum Culling ON");
}


void MainWindow::on_PlayStop_toggled(bool checked)
{
        mGameEngine->getInstance()->playMusic(checked);

        if(checked)
            ui->PlayStop->setText("Stop");
        else
            ui->PlayStop->setText("Play");

}


void MainWindow::on_pb_toggleCollisionBox_toggled(bool checked)
{
    mRenderWindow->toggleShowCollsionBox(checked);
    if(checked)
        ui->pb_toggleCollisionBox->setText("Hide All Collision Boxes");
    else
        ui->pb_toggleCollisionBox->setText("Show All Collision Boxes");
}

void MainWindow::on_listWidget_currentRowChanged(int currentRow)
{
    ObjectListIndex = currentRow;
    ui->TranslateXspinBox->setValue(GameObjects[ObjectListIndex]->mTransformComp->mMatrix.getPosition().x);
    ui->TranslateYspinBox->setValue(GameObjects[ObjectListIndex]->mTransformComp->mMatrix.getPosition().y);
    ui->TranslateZspinBox->setValue(GameObjects[ObjectListIndex]->mTransformComp->mMatrix.getPosition().z);

    ui->RotateXspinBox->setValue(GameObjects[ObjectListIndex]->mTransformComp->rotationDeg.x);
    ui->RotateYspinBox->setValue(GameObjects[ObjectListIndex]->mTransformComp->rotationDeg.y);
    ui->RotateZspinBox->setValue(GameObjects[ObjectListIndex]->mTransformComp->rotationDeg.z);

//    ui->ScaleXspinBox->setValue(GameObjects[ObjectListIndex]->mTransformComp->scale.x);
//    ui->ScaleYspinBox->setValue(GameObjects[ObjectListIndex]->mTransformComp->scale.y);
//    ui->ScaleZspinBox->setValue(GameObjects[ObjectListIndex]->mTransformComp->scale.z);


    // --Visible Selection in 3D window--

    // Turn off collision box of the last selected
    GameObjects[lastIndex]->mCollisionComp->bShowCollisionBox = false;

    GameObjects[ObjectListIndex]->mCollisionComp->bShowCollisionBox = true;
    lastIndex = currentRow;
}

void MainWindow::on_TranslateXspinBox_valueChanged(double arg1)
{
    QVector3D pos = GameObjects[ObjectListIndex]->mTransformComp->mMatrix.getPosition().getQVector();
    GameObjects[ObjectListIndex]->mTransformComp->mMatrix.setPosition(arg1,pos.y(),pos.z());
}


void MainWindow::on_TranslateYspinBox_valueChanged(double arg1)
{
    QVector3D pos = GameObjects[ObjectListIndex]->mTransformComp->mMatrix.getPosition().getQVector();
    GameObjects[ObjectListIndex]->mTransformComp->mMatrix.setPosition(pos.x(),arg1,pos.z());
}

void MainWindow::on_TranslateZspinBox_valueChanged(double arg1)
{
    QVector3D pos = GameObjects[ObjectListIndex]->mTransformComp->mMatrix.getPosition().getQVector();
    GameObjects[ObjectListIndex]->mTransformComp->mMatrix.setPosition(pos.x(),pos.y(),arg1);
}


void MainWindow::on_RotateXspinBox_valueChanged(double arg1)
{
    // for the text in the editor
    GameObjects[ObjectListIndex]->mTransformComp->rotationDeg.x = arg1;



    //lastrot needs to be pr object
    GameObjects[ObjectListIndex]->mTransformComp->mMatrix.rotateX((arg1-lastXrot));
    lastXrot = arg1;
}


void MainWindow::on_actionCube_triggered()
{
    mGameEngine->getInstance()->CreateCube();
    //listWidget->currentRowChanged(GameObjects.size()-1);
    updateList();
    listWidget->setCurrentRow(GameObjects.size()-1);
}


void MainWindow::on_actionPyramid_triggered()
{
    mGameEngine->getInstance()->CreatePyramid();
    updateList();
    listWidget->setCurrentRow(GameObjects.size()-1);
}


void MainWindow::on_actionSphere_triggered()
{
    mGameEngine->getInstance()->CreateSphere();
    updateList();
    listWidget->setCurrentRow(GameObjects.size()-1);
}


void MainWindow::on_actionSuzanne_triggered()
{
    mGameEngine->getInstance()->CreateSuzanne();
    updateList();
    listWidget->setCurrentRow(GameObjects.size()-1);
}


void MainWindow::on_actionCube_hovered()
{
    //ObjectListIndex = 0;
}

