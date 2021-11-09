#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSurfaceFormat>
#include <QDebug>
#include <QScreen>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QDebug>
#include <QScreen>
#include <QString>
#include <QCoreApplication>
#include <QDebug>       //Using qDebug
#include <QFile>        //Reading from file
//#include <QJSEngine>    //i get error message on QJSEngine

#include "rendersystem.h"
#include "soundsystem.h"
#include "resourcemanager.h"
#include "coreengine.h"
#include "meshhandler.h"
#include "entity.h"
#include "texturehandler.h"
#include "gameobject.h"
#include "scriptsystem.h"


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

    QCoreApplication app();

//    QJSEngine engine; //can't run QJSEngine, because error on declaration

    QString fileName = "../GEA2021/player.js";
    QString fileName1 = "../GEA2021/enemy.js";

    QFile scriptFile(fileName);

    if (!scriptFile.open(QIODevice::ReadOnly))
        qDebug() << "Error - NO FILE HERE: " << fileName;

    //reads the file
    QTextStream stream(&scriptFile);
    QString contents = stream.readAll();
    //now "contents" holds the whole JavaScript

    //close the file, because we don't need it anymore
    scriptFile.close();

    //Loads the whole script into script engine:
    //The important part! fileName is used to report bugs in the file
//    engine.evaluate(contents, fileName); //again can't use, declaration error...

//    scriptsystem *scriptsys = new scriptsystem;

//    QJSValue objectTest = engine.newQObject(scriptsys);
////    //Make a name for the object in the script engine
//    engine.globalObject().setProperty("cObject", objectTest);

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
}
void MainWindow::updateUI(const std::vector<class CoreEngine> &GameObjectData)
{
    ui->treeWidgetSceneOutliner->clear();
    QTreeWidgetItem* root = new QTreeWidgetItem(ui->treeWidgetSceneOutliner);
    root->setText(0, mCoreEngine->mGameObject->mName.c_str());
    ui->treeWidgetSceneOutliner->addTopLevelItem(root);
    ui->treeWidgetSceneOutliner->expandAll();

    for(unsigned i = 0; i<GameObjectData.size(); i++)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(root);
        item->setText(0, mCoreEngine->mGameObject->mName.c_str());
    }
}

void MainWindow::on_treeWidgetSceneOutliner_itemClicked(QTreeWidgetItem *item, int)
{
    clearLayout(ui->verticalLayoutDetails);

    if(!item || item->text(0) == mCoreEngine->mGameObject->mName.c_str())
    {
        mRenderSystem->cancelPickedObject();
        ui->lineEditGOName->setText("no selection");
        return;
    }
    mCurrentEditItem = item;
    ui->lineEditGOName->setText(mCurrentEditItem->text(0));
    item->setSelected(true);


}
void MainWindow::selectObjectByName(QString nameIn)
{
    if(nameIn== "")
        on_treeWidgetSceneOutliner_itemClicked(nullptr, 0);
    else
    {
        QList<QTreeWidgetItem *> list = ui->treeWidgetSceneOutliner->findItems(nameIn, Qt::MatchFixedString|Qt::MatchRecursive);
        on_treeWidgetSceneOutliner_itemClicked(list.first(), 0);
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

//Example of a slot called from the button on the top of the program.
void MainWindow::on_pb_toggleWireframe_toggled(bool checked)
{
    mRenderSystem->toggleWireframe(checked);
    if(checked)
        ui->pb_toggleWireframe->setText("Show Solid");
    else
        ui->pb_toggleWireframe->setText("Show Wireframe");

}
void MainWindow::on_actionRender_MousePick_toggled(bool arg1)
{
    renderMousePick = arg1;
}
void MainWindow::clearLayout(class QLayout *layout)
{
    QLayoutItem *item;
    while((item = layout->takeAt(0))){
        if (item->widget()){
            delete item->widget();
        }
        delete item;
    }
    ui->treeWidgetSceneOutliner->clearSelection();
}

void MainWindow::on_pb_togglePlay_toggled(bool checked)
{
    mCoreEngine->togglePlayMode(checked);

    if(checked)
        ui->pb_togglePlay->setText("Stop (R)");
    else
        ui->pb_togglePlay->setText("Play (R)");

}

