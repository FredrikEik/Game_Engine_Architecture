#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <QVector3D>
#include <QVector2D>
#include <QMatrix4x4>
#include <vector>
#include "vertex.h"
#include "shader.h"
#include "texture.h"
#include <sstream>
#include <fstream>
#include <string>

class VisualObject : public QOpenGLFunctions_4_1_Core {
public:
    VisualObject();
    virtual ~VisualObject();
    virtual void init() = 0;
    // move for interaktive objekter
//    virtual void move(float x, float y, float z) {  }

//    //New movement system
//    virtual void move(){}

//    virtual void setMoveRight(bool inValue){}
//    virtual void setMoveLeft(bool inValue){}
//    virtual void setMoveForwards(bool inValue){}
//    virtual void setMoveBackwards(bool inValue){}

//    virtual void setSpeed(float inValue){}
//    virtual void setHoverDistance(float inValue){}

//    // move for simulering (for eksempel NPC)
//    virtual void move(float dt) { }

//    //well... now all of our objects can read obj files... Doesnt hurt right?
//    void readObjMesh(std::string filename);

//    void readObjMesh(std::string filename, QVector3D ColorOverride); // aint really necessary, but its cool to have a color override in case we want a single single for all the vertices.

//    virtual void move(QVector3D inVec) {}

//    std::pair<float,float> getPosition2D();
//    QVector3D getPostion3D();
//    void setPosition3D(QVector3D inPos);
//    void setName(std::string name);
//    std::string getName() const;

//    //Barysentric coordinates
//    QVector3D* GetSurfacePos(int ix,int iy){return SurfacePosArrxzy[ix][iy];}

//    void setHeight(float value);


//    void setIsDrawable(bool value);


//    QVector3D topRightPoint;
//    QVector3D bottomLeftPoint;

//    virtual QVector3D getDirection(){};

//    void collisionVecCalc(float scale)
//    {
//        QVector3D temp{scale, scale, scale};
//        topRightPoint = temp;
//        bottomLeftPoint = temp * -1;
//    }

//    void collisionVecCalc(QVector3D scale)
//    {
//        topRightPoint = scale;
//        bottomLeftPoint = scale * -1;
//    }

//    bool isColliding(VisualObject * otherObject, QVector3D amount)
//    {
//        QVector3D position = this->getPostion3D();
//        QVector3D otherPosition = otherObject->getPostion3D();
//        bool checkX =
//                position.x() + topRightPoint.x() + amount.x() > otherPosition.x() + otherObject->bottomLeftPoint.x() - amount.x()
//                &&
//                position.x() + bottomLeftPoint.x() + amount.x() < otherPosition.x() + otherObject->topRightPoint.x() - amount.x();
//        bool checkZ =
//                position.z() + topRightPoint.z() + amount.z()>otherPosition.z() + otherObject->bottomLeftPoint.z() - amount.z()
//                &&
//                position.z() + bottomLeftPoint.z() + amount.z() < otherPosition.z() + otherObject->topRightPoint.z() - amount.z();
//        bool checkY =
//                position.y() + topRightPoint.y() + amount.y() > otherPosition.y() + otherObject->bottomLeftPoint.y() - amount.y()
//                &&
//                position.y() + bottomLeftPoint.y() + amount.y() < otherPosition.y() + otherObject->topRightPoint.y() - amount.y();

//        if(checkX)
//        {
//            if(checkZ)
//            {
//                if(checkY)
//                {
//                    return true;
//                }
//            }
//        }
//        return false;
//    }

//    void rotateX(GLfloat degrees);
//    void rotateY(GLfloat degrees);
//    void rotateZ(GLfloat degrees);
//    QVector3D GetForwardVector();
//    QVector3D CalculateNormalVector(QVector3D v1, QVector3D v2);
//    void SetBillboardvectors(QVector3D pos,QVector3D rightV, QVector3D upV);

//    QVector3D getRightVector();
//    QVector3D getUpVector();

//    bool getIsDrawable();

    std::string mName;

    struct TransformComponent* mTransform{nullptr};
    struct MeshComponent* mMesh{nullptr};
    struct MaterialComponent* mMaterial{nullptr};

 protected:



//    // Vertices and indices
//    std::vector<Vertex> mVertices;
//    std::vector<GLuint> mIndices;

//    GLuint mVAO{0};
//    GLuint mVBO{0};
//    GLuint mEAB{0}; //holds the indices (Element Array Buffer - EAB)

//    GLint mMatrixUniform{0};
//    QMatrix4x4 mMatrix;

//    // Velger å lagre posisjon, rotasjon og translasjon
//    // i hver sin 4x4 matrise
//    QMatrix4x4 mPosition;
//    QMatrix4x4 mRotation;
//    QMatrix4x4 mScale;
//    // Legger til rette for simulering
//    QVector3D mVelocity;

//    std::string mName; // really rarely used... but should be used for identifying object.

//    //barysentric coordinates
//    QVector3D *SurfacePosArrxzy[260][260];
//    QVector3D VertexNormals[260][260];

//    //This combined into draw()
//    bool isDrawable{true};
//    bool hasObj{false};


};
#endif // VISUALOBJECT_H

