#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSurfaceFormat>
#include <QDebug>
#include <QScreen>  //for resizing the program at start
#include <QTreeWidgetItem>
#include <QStyleFactory>

#include "renderwindow.h"
#include <detailswidget.h>
#include "factory.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    //this sets up what's in the mainwindow.ui
    ui->setupUi(this);
    qApp->setStyle(QStyleFactory::create("Fusion"));

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53,53,53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25,25,25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53,53,53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    qApp->setPalette(darkPalette);

    qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
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
    ui->PlayPause->setText("Editor Mode(TAB)");
    ui->createDropDownBox->addItem("Light");


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

void MainWindow::updateOutliner(std::vector<GameObject *> &GameObjectData)
{
    //clear all current items
    ui->outliner->clear();

    // Create the new tree root - since I use TreeWidget not listWidget
    mSceneOutlinerRoot = new QTreeWidgetItem(ui->outliner);
    mSceneOutlinerRoot->setText(0,  "Scene");//QString::fromStdString(mRenderWindow->mScene1->mSceneName));
    ui->outliner->addTopLevelItem(mSceneOutlinerRoot);
    ui->outliner->expandAll();

    for(auto gob : GameObjectData)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(mSceneOutlinerRoot);
        item->setText(0,QString::fromStdString(gob->mObjectName));
//        item->setFlags(item->flags() | Qt::ItemIsEditable);
    }
}

void MainWindow::selectObjectByIndex(int indexIn)
{
   if(mSceneOutlinerRoot)
    {
        if(mCurrentEditItem) mCurrentEditItem->setSelected(false);

        mCurrentEditItem = mSceneOutlinerRoot->child(indexIn);
        mCurrentEditItem->setSelected(true);

        on_outliner_itemClicked(mCurrentEditItem, 0);
    }

}


void MainWindow::clearLayout(QLayout *layout) {
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->widget()) {   //probably not neccesary
            delete item->widget();
        }
        delete item; //probably not neccesary - Qt should do it automatically
    }

  //  ui->outliner->clearSelection();
}

void MainWindow::on_outliner_itemClicked(QTreeWidgetItem *item, int column)
{
    clearLayout(ui->blDetailsContainer); //delete all widgets in the details panel

    //Top node selected or no selection:
    if (!item || item->text(0) == "Scene") //mRenderSystem->mScene1->mSceneName.c_str())
    {
        mRenderWindow->cancelPickedObject();
        ui->gobNameEdit->setText("no selection");
        mCurrentEditItem = nullptr;
        return;
    }

    mCurrentEditItem = item;
    ui->gobNameEdit->setText(mCurrentEditItem->text(0));
    item->setSelected(true);

    //scroll to selected item
    //ui->twSceneOutliner->scrollToItem(mCurrentEditItem);

    //getting the index of the selected item from the TreeWidget
    // = parent of this selected item
    mCurrentEditItemIndex = item->parent()->indexOfChild(item);
 //    qDebug() <<"Index" << mCurrentEditItemIndex;

    //tell RenderSystem to highlight selected object
    mRenderWindow->setPickedObject(mCurrentEditItemIndex);

    //Transform widget:
       mDetailsWidget = new DetailsWidget(this, mPositionStep, mRotationStep, mScaleStep);
       mDetailsWidget->setObjectName("DetailsWidget"); //not sure if this is necessary
       mDetailsWidget->init(mRenderWindow->getFactory(), mCurrentEditItemIndex);
       ui->blDetailsContainer->addWidget(mDetailsWidget);    //add to details pane
}

void MainWindow::on_PlayPause_clicked(bool checked)
{
    if(checked)
    {
        ui->PlayPause->setText("Editor Mode(TAB)");
        mRenderWindow->editorMode = true;
        mRenderWindow->bPause = false;
    }
    else if (!checked)
    {
        ui->PlayPause->setText("Playing (TAB)");
        mRenderWindow->editorMode = false;
        mRenderWindow->bPause = true;
    }

    mRenderWindow->playPausebutton(format);

}

void MainWindow::on_toggleFrustumCulling_clicked(bool checked)
{
    mRenderWindow->toggleFrustumCulling = checked;
}

void MainWindow::on_SaveLevel_clicked()
{
    mRenderWindow->saveLevel();
    qDebug() << "Saving current level";
}


void MainWindow::on_LoadLevel_clicked()
{
    mRenderWindow->loadLevel();
}


void MainWindow::on_SwitchTerrain_clicked()
{
    mRenderWindow->createTerrain();
}
