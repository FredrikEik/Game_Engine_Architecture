#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSurfaceFormat>
#include <QDebug>
#include <QScreen>  //for resizing the program at start
#include <QFile>
#include <QFileDialog>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QTreeWidgetItemIterator>
#include <gsl/vector3d.h>
#include <QMessageBox>

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

    ui->ObjectDetails->setVisible(false);
}

//Example of a slot called from the button on the top of the program.
void MainWindow::on_pushButton_toggled(bool checked)
{
    mRenderWindow->toggleWireframe(checked);
}

void MainWindow::on_actionCustom_OBJ_triggered()
{

}

void MainWindow::on_actionSuzanne_triggered()
{
    mRenderWindow->entitySys->constructSuzanne();
    updateViewPort();
}

void MainWindow::on_actionCube_triggered()
{
    mRenderWindow->entitySys->constructCube();
    updateViewPort();
}

void MainWindow::on_actionSphere_triggered()
{
    mRenderWindow->entitySys->constructSphere();
    updateViewPort();
}

void MainWindow::on_actionPlane_triggered()
{
    mRenderWindow->entitySys->constructPlane();
    updateViewPort();
}

void MainWindow::on_pushButton_2_clicked()
{
    if(mRenderWindow->isPaused){
        ui->pushButton_2->setText("Pause");
        mRenderWindow->isPaused = false;
        mRenderWindow->mSong->pause();
        mRenderWindow->togglePlayerCamera();


    }else{
        ui->pushButton_2->setText("Play");
        mRenderWindow->isPaused = true;
        mRenderWindow->mSong->play();
        mRenderWindow->togglePlayerCamera();
    }
}


void MainWindow::on_treeWidget_activated(const QModelIndex &index)
{

}

void MainWindow::on_treeWidget_viewportEntered()
{
    ui->treeWidget->clear();
    QTreeWidgetItem * QTWI = new QTreeWidgetItem(ui->treeWidget);
    ui->treeWidget->addTopLevelItem(QTWI);
    QTWI->setText(0, "Objects");
    QTWI->setExpanded(true);
    for(unsigned long i = 0; i < mRenderWindow->entities.size(); i++){
        QTreeWidgetItem * item = new QTreeWidgetItem(QTWI);

        for(unsigned long j = 0; j < mRenderWindow->DeetsVector.size(); j++){
            if(mRenderWindow->DeetsVector[j]->entity==mRenderWindow->entities[i]){
                QString temp;
                item->setText(0, temp.fromStdString(mRenderWindow->DeetsVector[j]->title));
                break;
            }
            else{
                item->setText(0, "Unknown");
            }
        }
    }
}

void MainWindow::updateViewPort()
{
    on_treeWidget_viewportEntered();
}

void MainWindow::on_treeWidget_itemActivated(QTreeWidgetItem *item, int column)
{
    ui->ObjectDetails->setVisible(true);
    ui->ObjectDetails->setEnabled(true);
    ui->checkBox->setEnabled(true);
    ui->label_6->setEnabled(true);
    QString itemName = item->text(column);
    std::string itemNameOS = itemName.toStdString();
    for(unsigned long i = 0; i < mRenderWindow->DeetsVector.size(); i++){
        if(mRenderWindow->DeetsVector[i]->title == itemNameOS){
            SelectedItem = mRenderWindow->DeetsVector[i]->entity;
            ui->ObjectDetails->setTitle("Object Details: " + item->text(column));
            ui->PosX->setValue(mRenderWindow->transformCompVec[SelectedItem]->mMatrix.getPosition().x);
            ui->PosY->setValue(mRenderWindow->transformCompVec[SelectedItem]->mMatrix.getPosition().y);
            ui->PosZ->setValue(mRenderWindow->transformCompVec[SelectedItem]->mMatrix.getPosition().z);

            break;
        }
    }

}

void MainWindow::on_PosX_valueChanged(double arg1)
{
    gsl::Vector3D tempPos =     mRenderWindow->transformCompVec[SelectedItem]->mMatrix.getPosition();
    mRenderWindow->transformCompVec[SelectedItem]->mMatrix.setPosition(arg1,tempPos.y, tempPos.z);
}

void MainWindow::on_PosY_valueChanged(double arg1)
{
    gsl::Vector3D tempPos =     mRenderWindow->transformCompVec[SelectedItem]->mMatrix.getPosition();
    mRenderWindow->transformCompVec[SelectedItem]->mMatrix.setPosition(tempPos.x, arg1, tempPos.z);
}

void MainWindow::on_PosZ_valueChanged(double arg1)
{
    gsl::Vector3D tempPos =     mRenderWindow->transformCompVec[SelectedItem]->mMatrix.getPosition();
    mRenderWindow->transformCompVec[SelectedItem]->mMatrix.setPosition(tempPos.x, tempPos.y, arg1);
}



void MainWindow::on_pushButton_3_clicked()
{

}


void MainWindow::updateDetails(){
    ui->PosX->setValue(mRenderWindow->transformCompVec[SelectedItem]->mMatrix.getPosition().getX());
    ui->PosY->setValue(mRenderWindow->transformCompVec[SelectedItem]->mMatrix.getPosition().getY());
    ui->PosZ->setValue(mRenderWindow->transformCompVec[SelectedItem]->mMatrix.getPosition().getZ());
    ui->checkBox->setChecked(mRenderWindow->meshCompVec[SelectedItem]->isDrawable);

}

void MainWindow::setSelectedItem(int i)
{
    ui->ObjectDetails->setVisible(true);
    ui->ObjectDetails->setEnabled(true);
    ui->checkBox->setEnabled(true);
    ui->label_6->setEnabled(true);
    SelectedItem = i;
    std::string text = mRenderWindow->DeetsVector[SelectedItem]->title;
    QString Qtext = QString::fromStdString(text);
    ui->ObjectDetails->setTitle("Object Details: " + Qtext);
    ui->PosX->setValue(mRenderWindow->transformCompVec[SelectedItem]->mMatrix.getPosition().x);
    ui->PosY->setValue(mRenderWindow->transformCompVec[SelectedItem]->mMatrix.getPosition().y);
    ui->PosZ->setValue(mRenderWindow->transformCompVec[SelectedItem]->mMatrix.getPosition().z);

}

void MainWindow::on_pushButton_5_clicked()
{
    mRenderWindow->transformCompVec[SelectedItem]->mMatrix.rotateX(-10.f);
}

void MainWindow::on_pushButton_6_clicked()
{

    mRenderWindow->transformCompVec[SelectedItem]->mMatrix.rotateX(10.f);
}

void MainWindow::on_pushButton_7_clicked()
{
    mRenderWindow->transformCompVec[SelectedItem]->mMatrix.rotateY(-10.f);
}

void MainWindow::on_pushButton_8_clicked()
{
    mRenderWindow->transformCompVec[SelectedItem]->mMatrix.rotateY(10.f);
}

void MainWindow::on_pushButton_9_clicked()
{
    mRenderWindow->transformCompVec[SelectedItem]->mMatrix.rotateZ(-10.f);
}

void MainWindow::on_pushButton_10_clicked()
{
    mRenderWindow->transformCompVec[SelectedItem]->mMatrix.rotateZ(10.f);
}

void MainWindow::on_checkBox_toggled(bool checked)
{
    mRenderWindow->meshCompVec[SelectedItem]->isDrawable=checked;
}

void MainWindow::on_actionControls_triggered()
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText("Move and rotate camera by holding down right mouse button. \nUse WASD to move around. \nQ = down, \nE = up. \nUse ScrollWheel to adjust speed.");
    msgBox.exec();
}

void MainWindow::on_actionSave_World_triggered()
{
      /*QString filename =  QFileDialog::getOpenFileName(
            this,
            "Select Level (JSON)",
            QDir::currentPath(),
            "JSON File (*.json)");*/

      QString filename1 =  QFileDialog::getSaveFileName(this, "Select path and name for storage!", QDir::currentPath(), "JSON File (*.json)");

      if( !filename1.isNull() )
      {
        qDebug() << "selected file path : " << filename1.toUtf8();
        mRenderWindow->JSS->SaveLevel(filename1.toStdString());
      }
}
