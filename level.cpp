#include "level.h"

Level::Level(Camera* cam)
{
    mCollisionSystem = new CollisionSystem();
    mMoveSys = new MovementSystem();
    mCam = cam;
    mShapeFactory.makeVertices();
    script = new Script();
    DrawBoard();

}

Level::~Level()
{
    for(auto i = 0; i<mVisualObjects.size(); i++)
    {
        delete mVisualObjects[i];
    }
    delete mLight; delete mFrustumSystem;

    mVisualObjects.clear();
    mTransformComp.clear();
    mNameComp.clear();
}

// 0 - coins
// 1 - wall
// 2 - Player
// 3 - ghost initial positions

int Level::GameBoard[Level::DIM_Z][Level::DIM_X] =
{  //0                1         5       19
   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},// 0
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},// 1
   {1,0,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,0,1},// 2
   {1,0,0,0,3,1,0,0,0,1,0,0,0,1,0,0,3,0,1},
   {1,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,1},// 4
   {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},// 5
   {1,0,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,0,1},
   {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1},
   {1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1},
   {1,1,1,1,0,1,0,0,0,0,0,0,0,1,0,1,1,1,1},
   {1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1},// 10
   {1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1},
   {1,1,1,1,0,1,0,1,1,3,1,1,0,1,0,1,1,1,1},// 12
   {1,1,1,1,0,1,0,0,0,0,0,0,0,1,0,1,1,1,1},// 13
   {1,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,1},// 14
   {1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1},
   {1,0,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,0,1},
   {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,3,1},
   {1,0,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,0,1}, //18
   {1,2,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,0,1},
   {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1},
   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}, //21


};



void Level::DrawBoard()
{

    VisualObject* temp{nullptr};
    Square* tempS{nullptr};
    Circle* tempC{nullptr};
    for(int i = 0; i<DIM_Z;i++)
    {
        for(int j = 0; j<DIM_X; j++)
        {
            if(GameBoard[i][j] == 0)
            {
                temp = mShapeFactory.createShape("Circle");
                tempC = static_cast<Circle*>(temp);
                tempC->init();
                tempC->mMaterial->mShaderProgram = 0;
                mMoveSys->move(tempC, j, CENTER_Y,i);
                mVisualObjects.push_back(temp);
                mTrophies.push_back(tempC);
                mTransformComp.push_back(tempC->mTransform);
                mNameComp.push_back(tempC->mNameComp);
            }
            else if(GameBoard[i][j] == 1){
                temp = mShapeFactory.createShape("Square");
                tempS = static_cast<Square*>(temp);
                tempS->init();
                tempS->mMaterial->mShaderProgram = 0;    //plain shader
                mMoveSys->move(tempS,j, CENTER_Y,i);
                mWall.push_back(tempS);
                mVisualObjects.push_back(temp);
                mTransformComp.push_back(tempS->mTransform);
                mNameComp.push_back(tempS->mNameComp);
            }
            else if(GameBoard[i][j] == 2){
                mPlayer = new Player(&mShapeFactory);
                mPlayer->mMaterial->mShaderProgram = 0; //plain shader
                mPlayer->init();
                mMoveSys->move(mPlayer, j, CENTER_Y,i);
                mVisualObjects.push_back(mPlayer);
                mTransformComp.push_back(mPlayer->mTransform);
                mNameComp.push_back(mPlayer->mNameComp);
            }
            else if(GameBoard[i][j] == 3){
                mEnemy = new Enemy(&mShapeFactory);
                mEnemy->init();
                mEnemy->mMaterial->mShaderProgram = 0;    //plain shader
                mMoveSys->move(mEnemy,j, CENTER_Y,i);
                mEnemies.push_back(mEnemy);
                mVisualObjects.push_back(mEnemy);
                mTransformComp.push_back(mEnemy->mTransform);
                mNameComp.push_back(mEnemy->mNameComp);
            }

        }
    }
    //------------------------Plain----------------------//
    temp = mShapeFactory.createShape("Plain");
    temp->init();
    temp->mMaterial->mShaderProgram = 0;   //plain shader
    mMoveSys->move(temp,9.5f, -0.5f, 11.0f);
    mVisualObjects.push_back(temp);
    mTransformComp.push_back(temp->mTransform);
    mNameComp.push_back(temp->mNameComp);

    for(auto i=0; i<3; i++)
    {
        temp = mShapeFactory.createShape("Heart");
        temp->init();
        temp->mMaterial->mShaderProgram = 0;   //plain shader
        temp->mTransform->mPosition = gsl::Vector3D(13.f, 9.f, 8.0f + 2*i);
        temp->mTransform->mMatrix.setPosition(temp->mTransform->mPosition);
        temp->mTransform->mMatrix.rotateY(180);
        hearts.push_back(temp);
        mVisualObjects.push_back(temp);
        mTransformComp.push_back(temp->mTransform);
    }


     //------------------------Frustum----------------------//
    mFrustumSystem = new FrustumSystem(mCam);
    mFrustumSystem->mMaterial->mShaderProgram = 0;    //plain shader
    mFrustumSystem->init();

    //------------------------Skybox----------------------//

    temp = mShapeFactory.createShape("Square");
    temp->init();
    temp->mMaterial->mShaderProgram = 3;   //plain shader
    mMoveSys->move(temp,9.f, 0.f, 9.f);
    temp->mTransform->mMatrix.scale(30);
    mVisualObjects.push_back(temp);
    mTransformComp.push_back(temp->mTransform);
    temp->mMaterial->mTextureUnit = 2;
    mNameComp.push_back(temp->mNameComp);

    //------------------------Light----------------------//
    mLight = new Light;
    mLight->mMaterial->mShaderProgram = 3;    //Phongshader
    mMoveSys->move(mLight,0.f, 0.f, 6.f);
    mLight->init();

    SoundHandler();


}


void Level::createShapes(string shapeID)
{
    VisualObject* temp = mShapeFactory.createShape(shapeID);
    mMoveSys->move(temp,1,4,1);
    temp->init();
    temp->mMaterial->mShaderProgram = 0;    //plain shader
    mTransformComp.push_back(temp->mTransform);
    mNameComp.push_back(temp->mNameComp);
    mVisualObjects.push_back(temp);
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
//    for(int i{0}; i<(int)script->trophyPos.size(); i++){
//        VisualObject* temp = mShapeFactory.createShape("Circle");
//        temp->init();
//        temp->mMaterial->mShaderProgram = 0;    //plain shader
//        temp->move(script->trophyPos[i].getX(), script->trophyPos[i].getY(), script->trophyPos[i].getZ());
//        mVisualObjects.push_back(temp);
//        mTransformComp.push_back(temp->mTransform);
//        mNameComp.push_back(temp->mNameComp);}
}

void Level::checkCollision()
{
    for(int i{0}; i<static_cast<int>(mTrophies.size()); i++){
        //kollisjon mot trofeer
        if(mCollisionSystem->CheckSphCol(mPlayer->mCollision, mTrophies[i]->mCollision))
        {
            if(trophies < static_cast<int>(mTrophies.size())){
                if(mTrophies[i]->drawMe == true){
                    trophies++;
                    playSound(1);
                    mTrophies[i]->drawMe = false;} //for å ikke tegne opplukket trofè
                else
                    continue;
            }else{
                qDebug() << "You Win";
                winCondition = true;
                resetGame();}
        }
    }
    for(int i{0}; i<static_cast<int>(mEnemies.size()); i++){
        if(mCollisionSystem->CheckSphCol(mPlayer->mCollision, mEnemies[i]->mCollision))
        {
            mLives--;
            gsl::Vector3D playerP = {1,CENTER_Y,20};
            gsl::Vector3D currP = mPlayer->mTransform->mPosition;
            VisualObject* vPlayer = static_cast<VisualObject*>(mPlayer);
            mMoveSys->move(vPlayer, playerP.x-currP.x, 0, playerP.z-currP.z);

            hearts[mLives]->drawMe = false;
            if(mLives == 0)
            {
                resetGame();playSound(0); //death sound
            }
            qDebug() << mLives;
        }
    }
}

void Level::resetGame()
{
    gsl::Vector3D playerP = {1,CENTER_Y,20};
    gsl::Vector3D currP = mPlayer->mTransform->mPosition;

    VisualObject* vPlayer = static_cast<VisualObject*>(mPlayer);
    mMoveSys->move(vPlayer, playerP.x-currP.x, 0, playerP.z-currP.z);

    if(mLives == 0 || winCondition ==true){
        for(int i{0}; i<static_cast<int>(mVisualObjects.size()); i++)
        {
            mVisualObjects[i]->drawMe = true;
        }
        trophies = 0;
        mLives = 3;
        int eID = 0;
        winCondition = false;
        for(int i = 0; i<DIM_Z;i++)
        {
            for(int j = 0; j<DIM_X; j++){
                if(GameBoard[i][j] == 4){
                    gsl::Vector3D enemyP = {static_cast<GLfloat>(j), CENTER_Y,static_cast<GLfloat>(i)};
                    gsl::Vector3D currEP = mEnemies[eID]->mTransform->mPosition;
                    mMoveSys->move(mEnemies[eID], enemyP.x-currEP.x, CENTER_Y, enemyP.z-currEP.z);
                    eID++;}
            }
        }}
}

void Level::spawnParticle()
{
    Particle *mParticle{nullptr};

    mParticle = new Particle(&mShapeFactory, mPlayer);
    mParticle->init();
    mMoveSys->move(mParticle, mPlayer->mTransform->mPosition.x,mPlayer->mTransform->mPosition.y, mPlayer->mTransform->mPosition.z);
    mParticle->mMaterial->mShaderProgram = 0;   //plain shader
    mParticles.push_back(mParticle);
    mTransformComp.push_back(mParticle->mTransform);
    mNameComp.push_back(mParticle->mNameComp);

}


void Level::SoundHandler()
{
    SoundManager::getInstance()->init();
    SoundSource* tempSound;

    tempSound = SoundManager::getInstance()->createSource(
                "Laser", gsl::Vector3D(0.0f, 0.0f, 20.0f),
                "../GEA2021/Assets/Sound/Pac-ManDeath.wav", false, 1.0f);
    mSound.push_back(tempSound);


    tempSound = mDeathSound = SoundManager::getInstance()->createSource(
                "Laser", gsl::Vector3D(0.0f, 0.0f, 20.0f),
                "../GEA2021/Assets/Sound/pacman_chomp.wav", false, 1.0f);
    mSound.push_back(tempSound);

}

void Level::playSound(int SoundID)
{
    mSound[SoundID]->play();

}

bool Level::wallCheck(int x, int z)
{
    if(GameBoard[z][x] == 1)
        return true;
    else
        return false;
}

void Level::updatePlayer()
{
    mMoveSys->moveUnit(mPlayer, mPlayer->mMoveComp);
    if(wallCheck(mPlayer->mMoveComp->posX, mPlayer->mMoveComp->posZ))
    {
        mMoveSys->centerPlayer();
    }
    else
        mMoveSys->moveForward(mPlayer, mPlayer->mMoveComp);
}

void Level::updateParticles(gsl::Vector3D mColor)
{
    mShapeFactory.mColor = mColor;
    for(auto i{0}; i<mParticles.size(); i++)
        mMoveSys->moveParticles(mColor, mParticles[i]);
}

void Level::update(Camera* dc, Input di, int randO, gsl::Vector3D col, int fC)
{
    mMoveSys->update(dc, mPlayer, di);
    dc->update();
    checkCollision();

    if(playM)
    {
        updateEnemy(randO);
        updatePlayer();
        updateParticles(col);
        if(mParticles.size()<20)
            spawnParticle();
        for(int i{0}; i < mParticles.size(); i++)
        {
            mParticleSystem->update(fC, mParticles[i]->mPC);
        }
    }
}

void Level::updateEnemy(int randNum)
{
    for(int i{0}; i<static_cast<int>(mEnemies.size()); i++){
     mMoveSys->moveUnit(mEnemies[i], mEnemies[i]->mMoveComp);
     if(wallCheck(mEnemies[i]->mMoveComp->posX, mEnemies[i]->mMoveComp->posZ))
     {
         if(randNum<5){
             mMoveSys->rotateForward(mEnemies[i]->mMoveComp);}
         else{
             mMoveSys->rotateForward(mEnemies[i]->mMoveComp);mMoveSys->rotateForward(mEnemies[i]->mMoveComp);mMoveSys->rotateForward(mEnemies[i]->mMoveComp);}
     }
     else
         mMoveSys->moveForward(mEnemies[i], mEnemies[i]->mMoveComp);
    }
}


Script::Script(QObject *parent) : QObject(parent)
{

}

void Script::sendSignal()
{
    emit signalOne();
}


void Script::scriptFunction(float x, float y, float z)
{
    gsl::Vector3D temp(x, y, z);
    trophyPos.push_back(temp);
}
