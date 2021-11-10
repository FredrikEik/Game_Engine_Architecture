#include "skybox.h"
#include "resourcesystem.h"
/*
skybox::skybox(const QString& frontImagePath,
               const QString& backImagePath,
               const QString& topImagePath,
               const QString& bottomImagePath,
               const QString& leftImagePath,
               const QString& rightImagePath,
               QWidget *parent) :
QOpenGLWidget(parent),
mTexture(QOpenGLTexture::TargetCubeMap),
mVertexBuf(QOpenGLBuffer::VertexBuffer),
//mSpeed(0.0f),
mFrontImagePath(frontImagePath),
mBackImagePath(backImagePath),
mTopImagePath(topImagePath),
mBottomImagePath(bottomImagePath),
mLeftImagePath(leftImagePath),
mRightImagePath(rightImagePath)
{
mLookAt.eye =    {+0.0f, +0.0f, +0.0f};
mLookAt.center = {+0.0f, +0.0f, -1.0f};
mLookAt.up =     {+0.0f, +1.0f, +0.0f};
}
void skybox::loadImages()
{
    const QImage posx = QImage(mRightImagePath).convertToFormat(QImage::Format_RGBA8888);
    const QImage negx = QImage(mLeftImagePath).convertToFormat(QImage::Format_RGBA8888);

    const QImage posy = QImage(mTopImagePath).convertToFormat(QImage::Format_RGBA8888);
    const QImage negy = QImage(mBottomImagePath).convertToFormat(QImage::Format_RGBA8888);

    const QImage posz = QImage(mFrontImagePath).convertToFormat(QImage::Format_RGBA8888);
    const QImage negz = QImage(mBackImagePath).convertToFormat(QImage::Format_RGBA8888);

    mTexture.create();
    mTexture.setSize(posx.width(), posx.height(), posx.depth());
    mTexture.setFormat(QOpenGLTexture::RGBA8_UNorm);
    mTexture.allocateStorage();

    mTexture.setData(0, 0, QOpenGLTexture::CubeMapPositiveX,
                     QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                     posx.constBits(), Q_NULLPTR);

    mTexture.setData(0, 0, QOpenGLTexture::CubeMapPositiveY,
                     QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                     posy.constBits(), Q_NULLPTR);

    mTexture.setData(0, 0, QOpenGLTexture::CubeMapPositiveZ,
                     QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                     posz.constBits(), Q_NULLPTR);

    mTexture.setData(0, 0, QOpenGLTexture::CubeMapNegativeX,
                     QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                     negx.constBits(), Q_NULLPTR);

    mTexture.setData(0, 0, QOpenGLTexture::CubeMapNegativeY,
                     QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                     negy.constBits(), Q_NULLPTR);

    mTexture.setData(0, 0, QOpenGLTexture::CubeMapNegativeZ,
                     QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                     negz.constBits(), Q_NULLPTR);

    mTexture.setWrapMode(QOpenGLTexture::ClampToEdge);
    mTexture.setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    mTexture.setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
}

void skybox::initializeGL()
{
    initializeOpenGLFunctions();

    mProgram.addShaderFromSourceCode(
                QOpenGLShader::Vertex,
                R"(
                attribute vec3 aPosition;
                varying vec3 vTexCoord;
                uniform mat4 mvpMatrix;

                void main()
                {
                    gl_Position = mvpMatrix * vec4(aPosition, 1.0);
                    vTexCoord = aPosition;
                }
                )");

    mProgram.addShaderFromSourceCode(
                QOpenGLShader::Fragment,
                R"(
                uniform samplerCube uTexture;
                varying vec3 vTexCoord;

                void main()
                {
                    gl_FragColor = textureCube(uTexture, vTexCoord);
                }
                )");

    mProgram.link();
    mProgram.bind();

    loadImages();

    QVector3D vertices[] =
    {
        {-1.0f,  1.0f, -1.0f},
        {-1.0f, -1.0f, -1.0f},
        {+1.0f, -1.0f, -1.0f},
        {+1.0f, -1.0f, -1.0f},
        {+1.0f, +1.0f, -1.0f},
        {-1.0f, +1.0f, -1.0f},

        {-1.0f, -1.0f, +1.0f},
        {-1.0f, -1.0f, -1.0f},
        {-1.0f, +1.0f, -1.0f},
        {-1.0f, +1.0f, -1.0f},
        {-1.0f, +1.0f, +1.0f},
        {-1.0f, -1.0f, +1.0f},

        {+1.0f, -1.0f, -1.0f},
        {+1.0f, -1.0f, +1.0f},
        {+1.0f, +1.0f, +1.0f},
        {+1.0f, +1.0f, +1.0f},
        {+1.0f, +1.0f, -1.0f},
        {+1.0f, -1.0f, -1.0f},

        {-1.0f, -1.0f, +1.0f},
        {-1.0f, +1.0f, +1.0f},
        {+1.0f, +1.0f, +1.0f},
        {+1.0f, +1.0f, +1.0f},
        {+1.0f, -1.0f, +1.0f},
        {-1.0f, -1.0f, +1.0f},

        {-1.0f, +1.0f, -1.0f},
        {+1.0f, +1.0f, -1.0f},
        {+1.0f, +1.0f, +1.0f},
        {+1.0f, +1.0f, +1.0f},
        {-1.0f, +1.0f, +1.0f},
        {-1.0f, +1.0f, -1.0f},

        {-1.0f, -1.0f, -1.0f},
        {-1.0f, -1.0f, +1.0f},
        {+1.0f, -1.0f, -1.0f},
        {+1.0f, -1.0f, -1.0f},
        {-1.0f, -1.0f, +1.0f},
        {+1.0f, -1.0f, +1.0f}
    };

    mVertexBuf.create();
    mVertexBuf.bind();
    mVertexBuf.allocate(vertices, 36 * sizeof(QVector3D));

    mProgram.enableAttributeArray("aPosition");
    mProgram.setAttributeBuffer("aPosition",
                                GL_FLOAT,
                                0,
                                3,
                                sizeof(QVector3D));

    mProgram.setUniformValue("uTexture", 0);
}

void skybox::paintGL()
{
    mTexture.bind();

    mModelMat.setToIdentity();

    mViewMat.setToIdentity();
    mViewMat.lookAt(mLookAt.eye,
                    mLookAt.center,
                    mLookAt.up);

    mProjectionMat.setToIdentity();
    mProjectionMat.perspective(mPerspective.verticalAngle,
                               mPerspective.aspectRatio,
                               mPerspective.nearPlane,
                               mPerspective.farPlane);

    mProgram.setUniformValue("mvpMatrix", mProjectionMat * mViewMat * mModelMat);

    glDrawArrays(GL_TRIANGLES,
                 0,
                 36);
}

void skybox::resizeGL(int w, int h)
{
    mPerspective.verticalAngle = 60.0;
    mPerspective.nearPlane = 0.0;
    mPerspective.farPlane = 1.0;
    mPerspective.aspectRatio =
            static_cast<float>(w) / static_cast<float>(h ? h : 1.0f);
}

*/
