#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSurfaceFormat>
#include <QDebug>
#include <QScreen>  //for resizing the program at start

#include "renderwindow.h"
#include "gameobject.h"
#include "soundmanager.h"
#include "objectfactory.h"

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
    ui->comboBox->addItem("Cube");
    ui->comboBox->addItem("Triangle");
    ui->comboBox->addItem("Goat");
}

void MainWindow::addObjectToWorldList(std::string name)
{
    ui->WorldObjects->addItem(name.c_str());
}

void MainWindow::removeObjectFromWorldList()
{
    delete ui->WorldObjects->takeItem(ui->WorldObjects->currentRow());
}

int MainWindow::getCurrentRow()
{
    return ui->WorldObjects->currentRow();
}

void MainWindow::displayCurrentTransform(int index)
{
    if (index == -1)
        return;

    gsl::Matrix4x4 matrix = mRenderWindow->getTransform(index);
    gsl::Vector3D position = matrix.getPosition();
    gsl::Vector3D rotation = mRenderWindow->getRotation(index);
    gsl::Vector3D scale = mRenderWindow->getScale(index);

    ui->transformX->setValue(position.getX());
    ui->transformY->setValue(position.getY());
    ui->transformZ->setValue(position.getZ());

    ui->rotationX->setValue(rotation.getX());
    ui->rotationY->setValue(rotation.getY());
    ui->rotationZ->setValue(rotation.getZ());

    ui->scaleX->setValue(scale.getX());
    ui->scaleY->setValue(scale.getY());
    ui->scaleZ->setValue(scale.getZ());
}

//Example of a slot called from the button on the top of the program.
void MainWindow::on_pushButton_toggled(bool checked)
{
    mRenderWindow->toggleWireframe(checked);
}

void MainWindow::on_pushButton_2_clicked()
{
    mRenderWindow->ObjectButton(ObjectSpawn);
}

void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    ObjectSpawn = arg1.toStdString();
}

//void MainWindow::on_Value_x_valueChanged(double arg1)
//{

//}

void MainWindow::on_PauseSound_clicked()
{
    mRenderWindow->pauseSound();
}

void MainWindow::on_PlaySound_clicked()
{
    mRenderWindow->playSound();
}

void MainWindow::on_StopSound_clicked()
{
    mRenderWindow->stopSound();
}

void MainWindow::on_WorldObjects_currentRowChanged(int currentRow)
{
    qDebug() << "Set Index: " << currentRow;
    mRenderWindow->setSelectionIndex(currentRow);
    displayCurrentTransform(currentRow);
}

void MainWindow::on_deleteButton_clicked()
{
    qDebug() << "Delete Index: ";
    mRenderWindow->deleteSelection();
}

void MainWindow::on_transformX_valueChanged(double arg1)
{
    qDebug() << "Transform X: " << arg1;
    mRenderWindow->setPositionX(arg1);
}
void MainWindow::on_transformY_valueChanged(double arg1)
{
    qDebug() << "Transform Y: " << arg1;
    mRenderWindow->setPositionY(arg1);
}
void MainWindow::on_transformZ_valueChanged(double arg1)
{
    qDebug() << "Transform Z: " << arg1;
    mRenderWindow->setPositionZ(arg1);
}

void MainWindow::on_rotationX_valueChanged(double arg1)
{
    qDebug() << "Rotation X: " << arg1;
    mRenderWindow->setRotationX(arg1);
}
void MainWindow::on_rotationY_valueChanged(double arg1)
{
    qDebug() << "Rotation Y: " << arg1;
    mRenderWindow->setRotationY(arg1);
}
void MainWindow::on_rotationZ_valueChanged(double arg1)
{
    qDebug() << "Rotation Z: " << arg1;
    mRenderWindow->setRotationZ(arg1);
}

void MainWindow::on_scaleX_valueChanged(double arg1)
{
    mRenderWindow->setScaleX(arg1);
}
void MainWindow::on_scaleY_valueChanged(double arg1)
{
    mRenderWindow->setScaleY(arg1);
}
void MainWindow::on_scaleZ_valueChanged(double arg1)
{
    mRenderWindow->setScaleZ(arg1);
}
