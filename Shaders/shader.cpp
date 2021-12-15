#include "shader.h"

//#include "GL/glew.h" - using QOpenGLFunctions instead

Shader::Shader(const GLchar *vertexPath, const GLchar *fragmentPath)
{
    initializeOpenGLFunctions();    //must do this to get access to OpenGL functions in QOpenGLFunctions

    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // Open files and check for errors
    vShaderFile.open( vertexPath );
    if(!vShaderFile)
        std::cout << "ERROR SHADER FILE " << vertexPath << " NOT SUCCESFULLY READ" << std::endl;
    fShaderFile.open( fragmentPath );
    if(!fShaderFile)
        std::cout << "ERROR SHADER FILE " << fragmentPath << " NOT SUCCESFULLY READ" << std::endl;
    std::stringstream vShaderStream, fShaderStream;
    // Read file's buffer contents into streams
    vShaderStream << vShaderFile.rdbuf( );
    fShaderStream << fShaderFile.rdbuf( );
    // close file handlers
    vShaderFile.close( );
    fShaderFile.close( );
    // Convert stream into string
    vertexCode = vShaderStream.str( );
    fragmentCode = fShaderStream.str( );

    const GLchar *vShaderCode = vertexCode.c_str( );
    const GLchar *fShaderCode = fragmentCode.c_str( );

    // 2. Compile shaders
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];
    // Vertex Shader
    vertex = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertex, 1, &vShaderCode, nullptr );
    glCompileShader( vertex );
    // Print compile errors if any
    glGetShaderiv( vertex, GL_COMPILE_STATUS, &success );
    if ( !success )
    {
        glGetShaderInfoLog( vertex, 512, nullptr, infoLog );
        std::cout << "ERROR SHADER VERTEX " << vertexPath << " COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment Shader
    fragment = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragment, 1, &fShaderCode, nullptr );
    glCompileShader( fragment );
    // Print compile errors if any
    glGetShaderiv( fragment, GL_COMPILE_STATUS, &success );
    if ( !success )
    {
        glGetShaderInfoLog( fragment, 512, nullptr, infoLog );
        std::cout << "ERROR SHADER FRAGMENT " << fragmentPath << " COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Shader Program linking
    this->m_program = glCreateProgram( );
    glAttachShader( this->m_program, vertex );
    glAttachShader( this->m_program, fragment );
    glLinkProgram( this->m_program );
    // Print linking errors if any
    glGetProgramiv( this->m_program, GL_LINK_STATUS, &success );

    //making nice output name:
    std::string shadername{vertexPath};
    shadername.resize(shadername.size()-5); //deleting ".vert"

    if (!success)
    {
        glGetProgramInfoLog( this->m_program, 512, nullptr, infoLog );
        qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << "  " << shadername.c_str() <<  "\n   " << infoLog;
    }
    else
    {
        qDebug() << "GLSL shader" << shadername.c_str() << "was successfully compiled!";
    }
    // Delete the shaders as they're linked into our program now and no longer needed
    // The shader program is now on the GPU and we reference it by using the m_program variable
    glDeleteShader( vertex );
    glDeleteShader( fragment );
}

void Shader::use()
{
    glUseProgram(this->m_program);
}

GLuint Shader::getProgram() const
{
    return m_program;
}

GLuint Shader::getModelMatrixUniform() const
{
    return m_modelMatrixUniform;
}

GLint Shader::getProjectionMatrixUniform() const
{
    return m_projectionMatrixUniform;
}

GLint Shader::getViewMatrixUniform() const
{
    return m_viewMatrixUniform;
}

GLint Shader::getLightColorUniform() const
{
    return m_lightColorUniform;
}

GLint Shader::getLightPositionUniform() const
{
    return m_lightPositionUniform;
}

GLint Shader::getAmbientStrengthUniform() const
{
    return m_ambientStrengthUniform;
}

GLint Shader::getLightStrengthUniform() const
{
    return m_lightStrengthUniform;
}

GLint Shader::getObjectColorUniform() const
{
    return m_objectColorUniform;
}

GLint Shader::getSpecularStrengthUniform() const
{
    return m_specularStrengthUniform;
}

GLint Shader::getSpecularExponentUniform() const
{
    return m_specularExponentUniform;
}

GLint Shader::getObjectAlphaUniform() const
{
    return m_objectAlphaUniform;
}

GLint Shader::getTextureUniform() const
{
    return m_textureUniform;
}
