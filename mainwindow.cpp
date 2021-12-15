#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSurfaceFormat>
#include <QDebug>
#include <QScreen>  //for resizing the program at start

#include "rendersystem.h"
#include "soundsystem.h"
#include "resourcemanager.h"
#include "coreengine.h"
#include "transformwidget.h"
#include "resourcemanager.h"
#include "resourcemanager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    //this sets up what's in the mainwindow.ui

    ui->setupUi(this);


    init();

}

MainWindow::~MainWindow()
{
    delete mRenderSystem;
    delete ui;
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
    mRenderSystem = new RenderSystem(format, this);

    //Check if renderwindow did initialize, else prints error and quit
    if (!mRenderSystem->context()) {
        qDebug() << "Failed to create context. Can not continue. Quits application!";
        delete mRenderSystem;
        return;
    }

    //The OpenGL RenderWindow got made, so continuing the setup:
    //We put the RenderWindow inside a QWidget so we can put in into a
    //layout that is made in the .ui-file
    mRenderWindowContainer = QWidget::createWindowContainer(mRenderSystem);
    //OpenGLLayout is made in the .ui-file!
    ui->OpenGLLayout->addWidget(mRenderWindowContainer);

    //Set the size of the program in % of the actual screen size
    QSize tempSize = QGuiApplication::primaryScreen()->size();

    tempSize.rheight() *= 0.75;
    tempSize.rwidth() *= 0.65;
    resize(tempSize);

    mCoreEngine = new CoreEngine(mRenderSystem);

    //sets the keyboard input focus to the RenderWindow when program starts
    // - can be deleted, but then you have to click inside the renderwindow to get the focus
    mRenderWindowContainer->setFocus();

    ui->progressBar->setValue(0);
}

///toggler wireframe
void MainWindow::on_pb_toggleWireframe_toggled(bool checked)
{
    mRenderSystem->toggleWireframe(checked);
    if(checked)
        ui->pb_toggleWireframe->setText("Show Solid");
    else
        ui->pb_toggleWireframe->setText("Show Wireframe");
}

void MainWindow::on_pb_togglePlay_toggled(bool checked)
{

    mCoreEngine->togglePlayMode(checked);
    mRenderSystem->toggleFrustum(checked);
    ///bytter mellom spill og editormodus
    if(checked)
        ui->pb_togglePlay->setText("Stop (R)");
    else
        ui->pb_togglePlay->setText("Play (R)");
}

void MainWindow::on_pushButton_clicked()
{
    ///spawner boss
    if(!mRenderSystem->isPlaying)
    mCoreEngine->loadBoss("boss.json");

}

void MainWindow::UpdateScore(int value)
{
    ///oppdaterer scoreprogress
    ui->progressBar->setValue(value);
}
void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{

///setter opp lister med items

    if(clicked)
    {
        mCurrentSelectedItem = item;
        mCurrentSelectedItem->setSelected(true);
        mTransformWidget = new transformWidget();
        mTransformWidget->init(this, mRenderSystem, mCoreEngine);
        mCurrentSelectedItem = ui->listWidget->currentItem();
        mCurrentSelectedItem->setForeground(Qt::green);

        if(!mCoreEngine->isPlaying)
        ui->verticalLayout_2->addWidget(mTransformWidget);
        clicked = false;

    }
    else
    {
        mCurrentSelectedItem->setSelected(false);
        item->setText(ui->listWidget->currentItem()->text());
        mCurrentSelectedItem->setForeground(Qt::black);
        delete mTransformWidget;
        clicked = true;
        mCurrentSelectedItem = nullptr;
    }

}


void MainWindow::on_actionAdd_Player_triggered()
{
    //legger til spilleren

    if(CoreEngine::getInstance()->isPlaying == false)
    {

//        mCoreEngine->player = mResourceManager->addObject("suzanne.obj");
//        mCoreEngine->player->mMaterial->mShaderProgram = 0;
//        mCoreEngine->player->mMaterial->mTextureUnit = 0;
//        mCoreEngine->player->mTransform->mMatrix.rotateY(180.f);
//        mCoreEngine->player->mTransform->mMatrix.scale(0.5f);
//        mCoreEngine->player->mTransform->mMatrix.translate(0.f, 0, -20);
//        //legger til sphere kollisjon
//        mResourceManager->addCollider("sphere", mCoreEngine->player);
//        //legger til musikk til spilleren:
//        mResourceManager->addComponent("neon_stereo.wav", mCoreEngine->player);
//        mCoreEngine->player->mSoundComponent->shouldPlay = true;
//        mCoreEngine->player->objName = "Player";
        mRenderSystem->mGameObjects.push_back(CoreEngine::getInstance()->player);
       //mCoreEngine->addEntity("suzanne.obj");
        ui->listWidget->addItem(CoreEngine::getInstance()->player->objName);
        CoreEngine::getInstance()->playerSpawned = true;
    }
}

void MainWindow::on_actionAdd_Enemy_triggered()
{

    //legger til fiende
    if(CoreEngine::getInstance()->isPlaying == false)
    {

    mRenderSystem->mGameObjects.push_back(mCoreEngine->enemy);
    ui->listWidget->addItem(mCoreEngine->enemy->objName);
    CoreEngine::getInstance()->enemySpawned = true;
    }
}

void MainWindow::on_actionAdd_XYZ_triggered()
{
    //legger til akse
    if(CoreEngine::getInstance()->isPlaying == false)
    {

    mRenderSystem->mGameObjects.push_back(mCoreEngine->axis);
    ui->listWidget->addItem(mCoreEngine->axis->objName);
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    //legger til en scene
    if(!sceneLoaded)
    {
        mCoreEngine->testScene();
        sceneLoaded = true;
        for(unsigned int i = 0; i < mCoreEngine->enemies.size(); i ++)
        {

            ui->listWidget->addItem(mCoreEngine->enemies[i]->objName);
        }
    }
    else
        qDebug() << "scene already loaded";



}


void MainWindow::on_progressBar_valueChanged(int value)
{
 ///sjekker om man har full score
 if(value == 100)
 {
     mCoreEngine->spawnBoss = true;
     qDebug() << " Boss Spawned!";
 }
}

void MainWindow::on_pushButton_3_clicked()
{
    ///reset scene
    mCoreEngine->reset();
}
