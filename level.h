#ifndef LEVEL_H
#define LEVEL_H
#include <QFile>        //Reading from file
#include <QJSEngine>    //The script engine itself!
#include <QObject>
#include <vector>
#include "camera.h"
#include "shapefactory.h"

class Script : public QObject
{
    //  for å kalle funksjoner fra javascript
    Q_OBJECT
public:
    Script(QObject *parent = nullptr);
    void sendSignal();
    std::vector<gsl::Vector3D> trophyPos;
signals:
    void signalOne();
public slots:
    //  en public slot kan kalles fra javascript, uten Q_INVOKABLE
    void scriptFunction(float x, float y, float z);
};
class Level : public QObject
{
public:
    Level(Camera* cam);
    ~Level();
    void initObjects();
    std::string createShapes(string shapeID);
    void readJS();
    void winner();

    std::vector<NameComponent*> mNameComps;
    std::vector<TransformComponent*> mTransComps;
    std::vector<VisualObject*> mVisualObjects;
    ShapeFactory mShapeFactory;
    FrustumSystem* mFrustumSystem;
    SoundSource* mLaserSound{};
    Player *mPlayer;
    Skybox *mSkyBox;
    Light *mLight{nullptr};
    Enemy *mEnemy;
    int trophies{0};
private:
    Script *script{nullptr};
    Camera* mCam;
};

#endif // LEVEL_H
