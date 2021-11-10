#include "level.h"

Level::Level(Camera* cam)
{
    mCam = cam;
    mShapeFactory.makeVertices();
    script = new Script();
    initObjects();
    readJS();
}

Level::~Level()
{
    for(auto i = 0; i<mVisualObjects.size(); i++)
    {
        delete mVisualObjects[i];
    }
    delete mLight; delete mSkyBox; delete mFrustumSystem;

    mVisualObjects.clear();
    mTransComps.clear();
    mNameComps.clear();
}

void Level::initObjects()
{
    mPlayer = new Player(&mShapeFactory);
    mPlayer->mMaterial->mShaderProgram = 0; //plain shader
    mPlayer->init();
    mPlayer->move(0,1,5);
    mVisualObjects.push_back(mPlayer);
    mTransComps.push_back(mPlayer->mTransform);
    mNameComps.push_back(mPlayer->mNameComp);

    VisualObject* temp = mShapeFactory.createShape("Circle");
    temp->init();
    temp->mMaterial->mShaderProgram = 0;    //plain shader
    temp->move(2.f, 1.f, .5f);
    mVisualObjects.push_back(temp);
    mTransComps.push_back(temp->mTransform);
    mNameComps.push_back(temp->mNameComp);

    temp = mShapeFactory.createShape("Square");
    temp->init();
    temp->mMaterial->mShaderProgram = 0;    //plain shader
    temp->move(-1.5f, 0.5f, -1.5f);
    mVisualObjects.push_back(temp);
    mTransComps.push_back(temp->mTransform);
    mNameComps.push_back(temp->mNameComp);

    temp = mShapeFactory.createShape("Plain");
    temp->init();
    temp->mMaterial->mShaderProgram = 0;   //plain shader
    mVisualObjects.push_back(temp);
    mTransComps.push_back(temp->mTransform);
    mNameComps.push_back(temp->mNameComp);

    //    temp = mShapeFactory.createShape("BigWall");
    //    temp = mShapeFactory.createShape("SmallWall");
    //    temp = mShapeFactory.createShape("Monkey.obj");
    //    temp = mShapeFactory.createShape("Pacman.obj");

    //    temp->init();
    //    temp->mMaterial->mShaderProgram = 0;    //plain shader
    //    temp->move(4.f, 0.5f, .5f);
    //    mVisualObjects.push_back(temp);
    //    mTransComps.push_back(temp->mTransform);
    //    mNameComps.push_back(temp->mNameComp);

    for(int i{1}; i<4; i++){
        temp = mShapeFactory.createShape("Enemy");
        temp->init();
        temp->mMaterial->mShaderProgram = 0;    //plain shader
        temp->move(-i/2 + 3, 0.f, (i*3) -5);
        mVisualObjects.push_back(temp);
        mTransComps.push_back(temp->mTransform);
        mNameComps.push_back(temp->mNameComp);}

    mFrustumSystem = new FrustumSystem(mCam);
    mFrustumSystem->mMaterial->mShaderProgram = 0;    //plain shader
    mFrustumSystem->init();

    //------------------------Skybox----------------------//
    mSkyBox = new Skybox();
    mSkyBox->setTexture();
    mSkyBox->mMaterial->mShaderProgram = 3;    //plain shader
    mSkyBox->init();

    //------------------------Light----------------------//
    mLight = new Light;
    mLight->mMaterial->mShaderProgram = 2;    //Phongshader
    mLight->move(0.f, 0.f, 6.f);
    mLight->init();

    //    for(int i=0; i<10; i++)
    //    {
    //        for(int y=0; y<10; y++)
    //        {
    //            temp = mShapeFactory.createShape("Monkey.obj");
    //            temp->init();
    //            temp->move((i-y), 0.5, y-5);
    //            temp->mMaterial->mShaderProgram = 0;    //plain shader
    //            mTransComps.push_back(temp->mTransform);
    //            mNameComps.push_back(temp->mNameComp);
    //            mVisualObjects.push_back(temp);
    //        }
    //    }

    //makes the soundmanager
    //it is a Singleton!!!
    SoundManager::getInstance()->init();
    mLaserSound = SoundManager::getInstance()->createSource(
                "Laser", gsl::Vector3D(20.0f, 0.0f, 0.0f),
                "../GEA2021/Assets/laser.wav", true, 0.7f);
}

std::string Level::createShapes(string shapeID)
{
    VisualObject* temp = mShapeFactory.createShape(shapeID);
    temp->init();
    temp->move(1,1,0.5);
    temp->mMaterial->mShaderProgram = 0;    //plain shader
    mTransComps.push_back(temp->mTransform);
    mNameComps.push_back(temp->mNameComp);
    mVisualObjects.push_back(temp);
    return temp->mNameComp->mName;
}

void Level::readJS()
{
    //Make the Script engine itself
    QJSEngine engine;

    //Make variable to the path
    //We also use this in the engine.evaluate() call later
    QString fileName = "../GEA2021/Assets/Trophies.js";
    //Make a QFile for it
    QFile scriptFile(fileName);

    //Try to open file and give error if something is wrong
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
    engine.evaluate(contents, fileName);

    //    //Makes a script-version for the script engine:
    QJSValue objectTest = engine.newQObject(script);
    //    //Make a name for the object in the script engine
    engine.globalObject().setProperty("cObject", objectTest);

    //4. Calls a function in script that calls the C-function: ***************************
    //Make a variable for the function
    QJSValue directCCall = engine.evaluate("callCFunction");
    //Call the function
    QJSValue speed = directCCall.call();

    //Bruker script til å sette Trofè-posisjon
    for(int i{0}; i<(int)script->trophyPos.size(); i++){
        VisualObject* temp = mShapeFactory.createShape("Circle");
        temp->init();
        temp->mMaterial->mShaderProgram = 0;    //plain shader
        temp->move(script->trophyPos[i].getX(), script->trophyPos[i].getY(), script->trophyPos[i].getZ());
        mVisualObjects.push_back(temp);
        mTransComps.push_back(temp->mTransform);
        mNameComps.push_back(temp->mNameComp);}
}

void Level::winner()
{

}

Script::Script(QObject *parent) : QObject(parent)
{

}

void Script::sendSignal()
{
    emit signalOne();
}

//void Script::readScript()
//{

//}

void Script::scriptFunction(float x, float y, float z)
{
    gsl::Vector3D temp(x, y, z);
    trophyPos.push_back(temp);
}
