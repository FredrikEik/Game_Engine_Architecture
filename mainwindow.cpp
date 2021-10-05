#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSurfaceFormat>
#include <QDebug>
#include <QScreen>  //for resizing the program at start
#include <QTreeWidgetItem>

#include "renderwindow.h"
#include "detailswidget.h"

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

    //sets the keyboard input focus to the RenderWindow when program starts
    // - can be deleted, but then you have to click inside the renderwindow to get the focus
    mRenderWindowContainer->setFocus();
    ui->createDropDownBox->addItem("Cube");
    ui->createDropDownBox->addItem("Plane");
    ui->createDropDownBox->addItem("Triangle");
    ui->createDropDownBox->addItem("MarioCube");
    ui->createDropDownBox->addItem("Camera");
    ui->createDropDownBox->addItem("Sphere");
}

//Example of a slot called from the button on the top of the program.
void MainWindow::on_pushButton_toggled(bool checked)
{
    mRenderWindow->toggleWireframe(checked);
}



void MainWindow::on_createDropDownBox_currentTextChanged(const QString &arg1)
{
            itemToSpawn = arg1.toStdString();
}

void MainWindow::on_CreateObject_clicked()
{
        mRenderWindow->createObjectbutton(itemToSpawn);
}

void MainWindow::selectObjectByIndex(int indexIn)
{
   if(mSceneOutlinerRoot)
    {
        if(mCurrentEditItem)
            mCurrentEditItem->setSelected(false);
        mCurrentEditItem = mSceneOutlinerRoot->child(indexIn);
        mCurrentEditItem->setSelected(true);
        on_twSceneOutliner_itemClicked(mCurrentEditItem, 0);
    }
}

void MainWindow::on_twSceneOutliner_itemClicked(QTreeWidgetItem *item, int column)
{
/*
    clearLayout(ui->blDetailsContainer); //delete all widgets in the details panel

    //Top node selected or no selection:
    if (!item || item->text(0) == "Scene") //mRenderSystem->mScene1->mSceneName.c_str())
    {
        mRenderWindow->cancelPickedObject();
        ui->gobNameEdit->setText("no selection");
        mCurrentEditItem = nullptr;
        return;
    }*/

    //Transform widget:
    mTransformWidget = new WidgetTransform(this, mPositionStep, mRotationStep, mScaleStep);
    mTransformWidget->setObjectName("TransformWidget"); //not sure if this is necessary
    mTransformWidget->init(mRenderSystem, mCurrentEditItemIndex);
    ui->blDetailsContainer->addWidget(mTransformWidget);    //add to details pane
}

void MainWindow::clearLayout(QLayout *layout) {
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->widget()) {   //probably not neccesary
            delete item->widget();
        }
        delete item; //probably not neccesary - Qt should do it automatically
    }
//    mTransformWidget = nullptr;
  //  ui->twSceneOutliner->clearSelection();
}
