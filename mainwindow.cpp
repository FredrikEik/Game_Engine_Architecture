#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSurfaceFormat>
#include <QDebug>
#include <QScreen>  //for resizing the program at start

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
        ui->pushButton_2->setText("Stop");
        ui->dockWidget->hide();
        ui->dockWidget_4->hide();
    }else{
        ui->pushButton_2->setText("Play");
        ui->dockWidget->show();
        ui->dockWidget_4->show();
    }
}

void MainWindow::on_actionAdd_Circle_triggered()
{
    mRenderWindow->mLvl->createShapes("Circle");
    run();
}

void MainWindow::on_actionAdd_Square_triggered()
{
    mRenderWindow->mLvl->createShapes("Square");
    run();
    //    QTreeWidgetItem * item = new QTreeWidgetItem(Widgetitem);
    //    QString temp;
    //    item->setText(0, temp.fromStdString(a));
}

void MainWindow::on_actionAdd_Monkey_triggered()
{
    mRenderWindow->mLvl->createShapes("Monkey.obj");
    run();
}

void MainWindow::on_treeWidget_itemActivated(QTreeWidgetItem *item, int column)
{
    QString itemName = item->text(column);
    std::string itemToString = itemName.toStdString();
    ui->DetailsGroupBox->setTitle("Details: "+itemName);
    for(auto i = 0;i<mRenderWindow->mLvl->mNameComps.back()->objectID+1; i++){
        if(mRenderWindow->mLvl->mNameComps[i]->mName == itemToString)
        {
            if(mRenderWindow->mousePickCollide == true)
            {
                selectWithMousePick(mRenderWindow->MousePickindex);
                mRenderWindow->mousePickCollide = false;
            }else
                objectID = mRenderWindow->mLvl->mNameComps[i]->objectID;

            ui->doubleSpinBoxX->setValue(mRenderWindow->mLvl->mTransComps[objectID]->mMatrix.getPosition().x);
            ui->doubleSpinBoxY->setValue(mRenderWindow->mLvl->mTransComps[objectID]->mMatrix.getPosition().y);
            ui->doubleSpinBoxZ->setValue(mRenderWindow->mLvl->mTransComps[objectID]->mMatrix.getPosition().z);
        }
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

void MainWindow::on_treeWidget_viewportEntered()
{
    ui->treeWidget->addTopLevelItem(Widgetitem);
    Widgetitem->setText(0, "Objects");
    Widgetitem->setExpanded(true);

    for(auto i = 0; i <= mRenderWindow->mLvl->mNameComps.back()->objectID; i++){
        QTreeWidgetItem * item = new QTreeWidgetItem(Widgetitem);
        QString temp;
        item->setText(0, temp.fromStdString(mRenderWindow->mLvl->mNameComps[i]->mName));
    }
}

void MainWindow::selectWithMousePick(int index)
{
    objectID = mRenderWindow->mLvl->mNameComps[index]->objectID;
}

void MainWindow::on_doubleSpinBoxX_valueChanged(double arg1)
{
    gsl::Vector3D temp = mRenderWindow->mLvl->mTransComps[objectID]->mMatrix.getPosition();
    mRenderWindow->mLvl->mTransComps[objectID]->mMatrix.setPosition(arg1, temp.y, temp.z);
}

void MainWindow::on_doubleSpinBoxY_valueChanged(double arg1)
{
    gsl::Vector3D temp = mRenderWindow->mLvl->mTransComps[objectID]->mMatrix.getPosition();
    mRenderWindow->mLvl->mTransComps[objectID]->mMatrix.setPosition(temp.x, arg1, temp.z);
}

void MainWindow::on_doubleSpinBoxZ_valueChanged(double arg1)
{
    gsl::Vector3D temp = mRenderWindow->mLvl->mTransComps[objectID]->mMatrix.getPosition();
    mRenderWindow->mLvl->mTransComps[objectID]->mMatrix.setPosition(temp.x,temp.y, arg1);
}

void MainWindow::on_RotateX_clicked()
{
    mRenderWindow->mLvl->mTransComps[objectID]->mMatrix.rotateX(5.f);
}

void MainWindow::on_RotateY_clicked()
{
    mRenderWindow->mLvl->mTransComps[objectID]->mMatrix.rotateY(5.f);
}

void MainWindow::on_RotateZ_clicked()
{
    mRenderWindow->mLvl->mTransComps[objectID]->mMatrix.rotateZ(5.f);
}

void MainWindow::on_ScalePlus_clicked()
{
    mRenderWindow->mLvl->mTransComps[objectID]->mMatrix.scale(1.5f);
}

void MainWindow::on_ScaleMinus_clicked()
{
    mRenderWindow->mLvl->mTransComps[objectID]->mMatrix.scale(0.5f);
}
