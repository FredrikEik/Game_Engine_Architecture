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
    if(checked){
        ui->pushButton_2->setText("Restart");
    ui->dockWidget_2->hide();
    ui->DetailGroupBox->hide();
    ui->ScaleGroupBox->hide();
    ui->ToggleWF->hide();


    }
    else{
        ui->pushButton_2->setText("Play");
        ui->dockWidget_2->show();
        ui->DetailGroupBox->show();
        ui->ScaleGroupBox->show();
        ui->ToggleWF->show();
    }
}

void MainWindow::run()
{
    if(Widgetitem == nullptr)
        Widgetitem = new QTreeWidgetItem(ui->treeWidget);
    else
        Widgetitem->treeWidget()->clear();

    on_treeWidget_viewportEntered();
}



void MainWindow::LiveCount(int lives)
{
    ui->LiveCount->display(lives);
}

void MainWindow::PointCount(int points)
{
    ui->PointCount->display(points);
}

void MainWindow::on_actionAdd_Circle_triggered()
{
    mRenderWindow->mLvl->createShapes("Circle");
}

void MainWindow::on_actionAdd_Square_triggered()
{
   mRenderWindow->mLvl->createShapes("Square");
}

void MainWindow::on_actionAdd_Monkey_triggered()
{
   mRenderWindow->mLvl->createShapes("Monkey.obj");
}

void MainWindow::on_treeWidget_itemActivated(QTreeWidgetItem *item, int column)
{
    QString itemName = item->text(column);
    std::string itemToString = itemName.toStdString();
    ui->DetailGroupBox->setTitle("Details: "+itemName);
    for(auto i = 0;i<mRenderWindow->mLvl->mNameComp.back()->ObjectID+1; i++){
        if(mRenderWindow->mLvl->mNameComp[i]->ObjectName == itemToString)
        {

            if(mRenderWindow->mousePickCollide == true)
            {
                SelectWithMousePick(mRenderWindow->MousePickindex);
                ui->doubleSpinBoxX->setValue(mRenderWindow->mLvl->mTransformComp[ActiveObject]->mMatrix.getPosition().x);
                ui->doubleSpinBoxY->setValue(mRenderWindow->mLvl->mTransformComp[ActiveObject]->mMatrix.getPosition().y);
                ui->doubleSpinBoxZ->setValue(mRenderWindow->mLvl->mTransformComp[ActiveObject]->mMatrix.getPosition().z);
                mRenderWindow->mousePickCollide = false;
            }else{
                ActiveObject = mRenderWindow->mLvl->mNameComp[i]->ObjectID;
                ui->doubleSpinBoxX->setValue(mRenderWindow->mLvl->mTransformComp[ActiveObject]->mMatrix.getPosition().x);
                ui->doubleSpinBoxY->setValue(mRenderWindow->mLvl->mTransformComp[ActiveObject]->mMatrix.getPosition().y);
                ui->doubleSpinBoxZ->setValue(mRenderWindow->mLvl->mTransformComp[ActiveObject]->mMatrix.getPosition().z);
            }
        }
    }
}

void MainWindow::SelectWithMousePick(int index)
{

    ActiveObject = mRenderWindow->mLvl->mNameComp[index]->ObjectID;
}



void MainWindow::on_treeWidget_viewportEntered()
{
    Widgetitem = new QTreeWidgetItem(ui->treeWidget);
    ui->treeWidget->addTopLevelItem(Widgetitem);
    Widgetitem->setText(0, "Objects");
    Widgetitem->setExpanded(true);

    for(auto i = 0; i <= mRenderWindow->mLvl->mNameComp.back()->ObjectID; i++){
        QTreeWidgetItem * item = new QTreeWidgetItem(Widgetitem);
        QString temp;
        item->setText(0, temp.fromStdString(mRenderWindow->mLvl->mNameComp[i]->ObjectName));
    }
}


void MainWindow::on_doubleSpinBoxX_valueChanged(double arg1)
{
    gsl::Vector3D temp = mRenderWindow->mLvl->mTransformComp[ActiveObject]->mMatrix.getPosition();
    mRenderWindow->mLvl->mTransformComp[ActiveObject]->mMatrix.setPosition(arg1,temp.y, temp.z);
}


void MainWindow::on_doubleSpinBoxY_valueChanged(double arg1)
{
    gsl::Vector3D temp = mRenderWindow->mLvl->mTransformComp[ActiveObject]->mMatrix.getPosition();
    mRenderWindow->mLvl->mTransformComp[ActiveObject]->mMatrix.setPosition(temp.x,arg1, temp.z);

}


void MainWindow::on_doubleSpinBoxZ_valueChanged(double arg1)
{
    gsl::Vector3D temp = mRenderWindow->mLvl->mTransformComp[ActiveObject]->mMatrix.getPosition();
    mRenderWindow->mLvl->mTransformComp[ActiveObject]->mMatrix.setPosition(temp.x,temp.y, arg1);
}



void MainWindow::on_RotateXP_clicked()
{
    mRenderWindow->mLvl->mTransformComp[ActiveObject]->mMatrix.rotateX(5.f);
}


void MainWindow::on_RotateYP_clicked()
{
    mRenderWindow->mLvl->mTransformComp[ActiveObject]->mMatrix.rotateY(5.f);
}


void MainWindow::on_RotateZP_clicked()
{
    mRenderWindow->mLvl->mTransformComp[ActiveObject]->mMatrix.rotateZ(5.f);
}


void MainWindow::on_ScaleP_clicked()
{
    mRenderWindow->mLvl->mTransformComp[ActiveObject]->mMatrix.scale(1.5f);
}


void MainWindow::on_ScaleM_clicked()
{
    mRenderWindow->mLvl->mTransformComp[ActiveObject]->mMatrix.scale(0.50f);
}


void MainWindow::on_RotateYM_clicked()
{
    mRenderWindow->mLvl->mTransformComp[ActiveObject]->mMatrix.rotateY(-5.f);
}


void MainWindow::on_RotateZM_clicked()
{
    mRenderWindow->mLvl->mTransformComp[ActiveObject]->mMatrix.rotateZ(-5.f);
}


void MainWindow::on_RoatetXM_clicked()
{
    mRenderWindow->mLvl->mTransformComp[ActiveObject]->mMatrix.rotateX(-5.f);
}




