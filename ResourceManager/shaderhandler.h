#ifndef SHADERHANDLER_H
#define SHADERHANDLER_H

#include <QOpenGLFunctions_4_1_Core>
#include "math_constants.h"
#include "constants.h"
#include "vector2d.h"
#include "vector3d.h"
#include "matrix3x3.h"
#include "matrix4x4.h"
#include "components.h"

//#include "GL/glew.h" //We use QOpenGLFunctions instead, so no need for Glew (or GLAD)!

//This class is pretty much a copy of the shader class at
//https://github.com/SonarSystems/Modern-OpenGL-Tutorials/blob/master/%5BLIGHTING%5D/%5B8%5D%20Basic%20Lighting/Shader.h
//which is based on stuff from http://learnopengl.com/ and http://open.gl/.

//must inherit from QOpenGLFunctions_4_1_Core, since we use that instead of glfw/glew/glad
class ShaderHandler : protected QOpenGLFunctions_4_1_Core
{
public:
    //! @fn constructing shader
    //! @param shaderName - name of the shader that is beeing used
    ShaderHandler(const std::string &shaderName);

    // Use the current shader
    //! @fn use - using the shaderprogram called for
    virtual void use(gsl::Matrix4x4 &modelMatrixIn, struct ecs::Material *);
    virtual void setupShader(){}
    //! @var mName - name for shader
    const std::string mName;

    GLint mMatrixUniform{-1};       //when using the shader in drawcalls to different objects
    GLint vMatrixUniform{-1};
    GLint pMatrixUniform{-1};
    //! @param mCurrentCamera - contains pointer to camera
    class Camera *mCurrentCamera{nullptr};
    //! @param mRenderWindow - contains point to renderwindow
    class RenderWindow *mRenderWindow{nullptr};     //to be able to call checkForGLerrors()

    GLuint mProgram{999999};

private:
    GLuint program;
};

#endif
