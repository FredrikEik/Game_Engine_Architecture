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

//    ui->listWidget->addItem(mCoreEngine->player->objName);
//    ui->listWidget->addItem(mCoreEngine->enemy->objName);

    //sets the keyboard input focus to the RenderWindow when program starts
    // - can be deleted, but then you have to click inside the renderwindow to get the focus
    mRenderWindowContainer->setFocus();
}



//Example of a slot called from the button on the top of the program.
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

    if(checked)
        ui->pb_togglePlay->setText("Stop (R)");
    else
        ui->pb_togglePlay->setText("Play (R)");
}





void MainWindow::on_pushButton_clicked()
{
    //mTransformWidget->objectsInList +=1;

    mRenderSystem->mGameObjects.push_back(mCoreEngine->boss);
    mCoreEngine->boss->mSoundComponent->shouldPlay = true;

    ui->listWidget->addItem(mCoreEngine->boss->objName);




}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{



    //qDebug() << "DATA FOR ITEM " << item->data(currentIndex);
    if(clicked == true)
    {
        item->setSelected(true);
        mTransformWidget = new transformWidget();
        mTransformWidget->init(this, mRenderSystem, mCoreEngine, currentIndex);
        item = ui->listWidget->currentItem();
        item->setForeground(Qt::green);
        mCurrentSelectedItem = item;


        //qDebug() << "selected item " << mCurrentSelectedItem->

        ui->verticalLayout_2->addWidget(mTransformWidget);
        clicked = false;
    }
    else
    {
        item->setSelected(false);
        item->setText(ui->listWidget->currentItem()->text());
        item->setForeground(Qt::black);
        //ui->verticalLayout_2->removeWidget(mTransformWidget);
        //ui->verticalLayout_2->setEnabled(false);
        delete mTransformWidget;
        clicked = true;
    }


}

void MainWindow::on_actionAdd_Player_triggered()
{
    //mTransformWidget->objectsInList +=1;

    if(CoreEngine::getInstance()->isPlaying == false)
    {

    mRenderSystem->mGameObjects.push_back(mCoreEngine->player);
    ui->listWidget->addItem(mCoreEngine->player->objName);
    }
}

void MainWindow::on_actionAdd_Enemy_triggered()
{

    if(CoreEngine::getInstance()->isPlaying == false)
    {

    mRenderSystem->mGameObjects.push_back(mCoreEngine->enemy);
    ui->listWidget->addItem(mCoreEngine->enemy->objName);
    }
}

void MainWindow::on_actionAdd_XYZ_triggered()
{

    if(CoreEngine::getInstance()->isPlaying == false)
    {

    mRenderSystem->mGameObjects.push_back(mCoreEngine->axis);
    ui->listWidget->addItem(mCoreEngine->axis->objName);
    }
}

void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
//    if(!item)
//    {
//        mCurrentTreeItem = nullptr;
//        return;
//    }
    mCurrentTreeItem = item;
    item->setSelected(true);
    ui->treeWidget->scrollToItem(mCurrentTreeItem);
    currentIndex = item->parent()->indexOfChild(item);

    mTransformWidget = new transformWidget();
    mTransformWidget->init(this, mRenderSystem, mCoreEngine, currentIndex);
     ui->verticalLayout_2->addWidget(mTransformWidget);

}
