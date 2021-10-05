#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSurfaceFormat>
#include <QDebug>
#include <QScreen>  //for resizing the program at start
#include <QMenu>

#include "renderwindow.h"

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
}

//Example of a slot called from the button on the top of the program.
void MainWindow::on_pushButton_toggled(bool checked)
{
    mRenderWindow->toggleWireframe(checked);
}


void MainWindow::on_pushButton_2_toggled(bool checked)
{
    mRenderWindow->playMode(checked);
    if(checked)
        ui->pushButton_2->setText("Stop");
    else
        ui->pushButton_2->setText("Play");
}

void MainWindow::on_actionAdd_Triangle_triggered()
{
    mRenderWindow->toggleShapes(1);
}

void MainWindow::on_actionAdd_Circle_triggered()
{
    mRenderWindow->toggleShapes(2);
}

void MainWindow::on_actionAdd_Square_triggered()
{
    mRenderWindow->toggleShapes(3);
}

void MainWindow::on_actionAdd_Monkey_triggered()
{
    mRenderWindow->toggleShapes(0);
}

void MainWindow::on_doubleSpinBoxX(double x,VisualObject *mVisualObject)
{

    gsl::Vector3D getObjectT = mVisualObject->mTransform->mMatrix.getPosition();
    ui->doubleSpinBoxX->setValue(getObjectT.x);

    gsl::Vector3D ChangeObjectT;
    ChangeObjectT.x = ui->doubleSpinBoxX->value();
}

void MainWindow::on_doubleSpinBoxY(double y, VisualObject *mVisualObject)
{
    gsl::Vector3D getObjectT = mVisualObject->mTransform->mMatrix.getPosition();
    ui->doubleSpinBoxY->setValue(getObjectT.y);

    gsl::Vector3D ChangeObjectT;
    ChangeObjectT.y = ui->doubleSpinBoxY->value();

}

void MainWindow::on_doubleSpinBoxZ(double z, VisualObject *mVisualObject)
{

    gsl::Vector3D getObjectT = mVisualObject->mTransform->mMatrix.getPosition();
    ui->doubleSpinBoxZ->setValue(getObjectT.z);

    gsl::Vector3D ChangeObjectT;
    ChangeObjectT.z = ui->doubleSpinBoxZ->value();
}



