#ifndef SKYBOX_H
#define SKYBOX_H
/*
#include <QOpenGLFunctions>
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include<iostream>

#include <vector>
#include "texture.h"
#include "constants.h"
#include "vertex.h"
#include "shader.h"
#include "resourcesystem.h"
#include "gsl/matrix4x4.h"

///////https://amin-ahmadi.com/2019/07/28/creating-a-skybox-using-c-qt-and-opengl///////
class skybox: public QOpenGLWidget, public QOpenGLFunctions
{
public:
    explicit skybox(const QString& frontImagePath,
                    const QString& backImagePath,
                    const QString& topImagePath,
                    const QString& bottomImagePath,
                    const QString& leftImagePath,
                    const QString& rightImagePath,
                    QWidget *parent = Q_NULLPTR);

    unsigned int loadCubemap(std::vector<std::string> faces);
    void draw(resourceSystem* ResSys, RenderSystem * RenderSys);
    void paintGL() override;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;

private:
    void loadImages();

    QOpenGLShaderProgram mProgram;
    QOpenGLTexture mTexture;
    QOpenGLBuffer  mVertexBuf;
    std::vector<Vertex> mVertices;

    struct
    {
        float verticalAngle;
        float aspectRatio;
        float nearPlane;
        float farPlane;
    } mPerspective;

    struct
    {
        QVector3D eye;
        QVector3D center;
        QVector3D up;
    } mLookAt;

    QMatrix4x4 mModelMat, mViewMat, mProjectionMat;

    QString mFrontImagePath;
    QString mBackImagePath;
    QString mTopImagePath;
    QString mBottomImagePath;
    QString mLeftImagePath;
    QString mRightImagePath;

};
*/
#endif // SKYBOX_H
