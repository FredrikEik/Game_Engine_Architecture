#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSurfaceFormat>
#include <QDebug>
#include <QScreen>  //for resizing the program at start
#include <QMessageBox>  //For Help menu messages
#include <QTreeWidgetItem> //Scene Outliner

#include "rendersystem.h"
#include "soundsystem.h"
#include "resourcemanager.h"
#include "coreengine.h"
#include "gameobject.h"
#include "widgettransform.h"
#include "widgetmaterial.h"

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
    delete mCoreEngine;
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

    //Log same to ui:
    //QString tempString;
    //QDebug tempDebug(&tempString);
    //tempDebug << format;
    //ui->outputLog->appendPlainText("Program startup...\n Requesting surface format: " + tempString);

    //We have a format for the OpenGL window, so let's make it:
    mRenderSystem = new RenderSystem(format, this);

    //Check if renderwindow did initialize, else prints error and quit
    if (!mRenderSystem->context()) {
        qDebug() << "Failed to create context. Can not continue. Closing application!";
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

    mCoreEngine = new CoreEngine(mRenderSystem, this);

    //sets the keyboard input focus to the RenderWindow when program starts
    // - can be deleted, but then you have to click inside the renderwindow to get the focus
    mRenderWindowContainer->setFocus();

    //read what is set in the GUI
    mPositionStep = (float)ui->positionStep->value();
    mRotationStep = (float)ui->rotationStep->value();
    mScaleStep = (float)ui->scaleStep->value();
}

void MainWindow::updateOutliner(const std::vector<GameObject*> &GameObjectData)
{
    //clear all current items
    ui->twSceneOutliner->clear();

    // Create the new tree root - since I use TreeWidget not listWidget
    mSceneOutlinerRoot = new QTreeWidgetItem(ui->twSceneOutliner);
    mSceneOutlinerRoot->setText(0,  "Scene");//QString::fromStdString(mRenderWindow->mScene1->mSceneName));
    ui->twSceneOutliner->addTopLevelItem(mSceneOutlinerRoot);
    ui->twSceneOutliner->expandAll();

    for(auto gob : GameObjectData)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(mSceneOutlinerRoot);
        item->setText(0, QString::fromStdString(gob->mName));
//        item->setFlags(item->flags() | Qt::ItemIsEditable);
    }
}

void MainWindow::selectObjetByIndex(int indexIn)
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

void MainWindow::on_actionAdd_Triangle_triggered()
{
    GameObject *temp = ResourceManager::getInstance().addObject("triangle");
    mRenderSystem->mGameObjects.push_back(temp);
}

void MainWindow::on_actionAdd_Suzanne_triggered()
{
    GameObject *temp = ResourceManager::getInstance().addObject("suzanne.obj");
    mRenderSystem->mGameObjects.push_back(temp);
}

void MainWindow::on_actionToggle_backside_culling_toggled(bool checked)
{
    mRenderSystem->toggleBacksideCulling(checked);
}

void MainWindow::on_actionFrustum_Culling_triggered(bool checked)
{
    mRenderSystem->mUseFrustumCulling = checked;
}

void MainWindow::on_actionCull_With_EditorCam_toggled(bool checked)
{
    mRenderSystem->mGameCamAsFrustumCulling = !checked;
}

//void MainWindow::on_pb_togglefrustumcam_toggled(bool checked)
//{
//    mRenderSystem->mGameCamAsFrustumCulling = checked;
//    (checked) ? ui->pb_togglefrustumcam->setText("Cull with GameCam") : ui->pb_togglefrustumcam->setText("Cull with EditorCam");
//}

void MainWindow::on_actionBasic_Editor_Controls_triggered()
{
    QMessageBox::about(this, "Basic Controls",
                       "Move and rotate camera by holding down right mouse button,"
                       "\nand move with WASD and Q = down, E = up."
                       "\nUse ScrollWheel to adjust speed." // and Left Shift
                       "\nUse F to move camera to selected object.");
}

void MainWindow::on_actionWireframe_triggered(bool checked)
{
    mRenderSystem->toggleWireframe(checked);
    if(checked)
        ui->actionWireframe->setText("Show Solid");
    else
        ui->actionWireframe->setText("Show Wireframe");
}

void MainWindow::on_actionPlay_triggered(bool checked)
{
    mCoreEngine->togglePlayMode(checked);

    if(checked)
        ui->actionPlay->setText("Stop");
    else
        ui->actionPlay->setText("Play");
}

void MainWindow::on_actionKudos_to_triggered()
{
    QMessageBox::about(this, "Kudos to:",
                       "Some icons in this app are from https://icons8.com");
}

void MainWindow::on_actionAxis_triggered(bool checked)
{
    //Toggle axis on/off
}

void MainWindow::on_actionGrid_triggered(bool checked)
{
    //Toggle grid on/off
}

void MainWindow::on_twSceneOutliner_itemClicked(QTreeWidgetItem *item, int column)
{
    clearLayout(ui->blDetailsContainer); //delete all widgets in the details panel

    //Top node selected or no selection:
    if (!item || item->text(0) == "Scene") //mRenderSystem->mScene1->mSceneName.c_str())
    {
        mRenderSystem->cancelPickedObject();
        ui->gobNameEdit->setText("no selection");
        mCurrentEditItem = nullptr;
        return;
    }

    mCurrentEditItem = item;
    ui->gobNameEdit->setText(mCurrentEditItem->text(0));
    item->setSelected(true);

    //scroll to selected item
    ui->twSceneOutliner->scrollToItem(mCurrentEditItem);

    //getting the index of the selected item from the TreeWidget
    // = parent of this selected item
    mCurrentEditItemIndex = item->parent()->indexOfChild(item);
//    qDebug() <<"Index" << mCurrentEditItemIndex;

    //tell RenderSystem to highlight selected object
    mRenderSystem->setPickedObject(mCurrentEditItemIndex);

    //Transform widget:
    mTransformWidget = new WidgetTransform(this, mPositionStep, mRotationStep, mScaleStep);
    mTransformWidget->setObjectName("TransformWidget"); //not sure if this is necessary
    mTransformWidget->init(mRenderSystem, mCurrentEditItemIndex);
    ui->blDetailsContainer->addWidget(mTransformWidget);    //add to details pane

    //Material widget:
    mMaterialWidget = new WidgetMaterial(this);
    mMaterialWidget->setObjectName("MaterialWidget");
    ui->blDetailsContainer->addWidget(mMaterialWidget);    //add to details pane
//    mMaterialWidget->indexInSceneArray = mCurrentEditItemIndex;
//    mMaterialWidget->mCurrentScene = mRenderWindow->mScene1;
    mMaterialWidget->readMaterialData();
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
    ui->twSceneOutliner->clearSelection();
}

void MainWindow::on_positionStep_valueChanged(double arg1)
{
    mPositionStep = arg1;
    if(mTransformWidget)
    {
        mTransformWidget->setPositionStep(arg1);
    }
}

void MainWindow::on_rotationStep_valueChanged(double arg1)
{
    mRotationStep = arg1;
    if(mTransformWidget)
    {
        mTransformWidget->setRotationStep(arg1);
    }
}

void MainWindow::on_scaleStep_valueChanged(double arg1)
{
    mScaleStep = arg1;
    if(mTransformWidget)
    {
        mTransformWidget->setScaleStep(arg1);
    }
}


