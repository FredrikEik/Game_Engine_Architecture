#include "mariocube.h"
#include "objreader.h"

MarioCube::MarioCube()
{
    ObjReader objReader;
    meshComp = new MeshComponent();
    objReader.readFile("../GEA2021/Assets/Meshes/mariocube.obj", *meshComp);

    transformComp = new TransformComponent();



    transformComp->mMatrix.setToIdentity();

}

MarioCube::~MarioCube() {}

void MarioCube::init(/*GLint matrixUniform[4]*/)
{
    initializeOpenGLFunctions();

       // Set what shader you want to use to render this object
       //mMaterial->setActiveShader(ShaderType::TEXTURE_SHADER);
       //mMaterial->setActiveTextureSlot(2);
       //mMaterial->setupModelMatrixUniform(mMatrixUniform, matrixUniform);

       glGenVertexArrays( 1, &meshComp->mVAO );
       glBindVertexArray( meshComp->mVAO );


       glGenBuffers( 1, &meshComp->mVBO );
       glBindBuffer( GL_ARRAY_BUFFER, meshComp->mVBO );

       glBufferData( GL_ARRAY_BUFFER,                     //what buffer type
                     meshComp->mVertices.size() * sizeof( Vertex ), //how big buffer do we need
                     meshComp->mVertices.data(),                    //the actual vertices
                     GL_STATIC_DRAW                       //should the buffer be updated on the GPU
                     );

       glBindBuffer(GL_ARRAY_BUFFER, meshComp->mVBO);

       // 1rst attribute buffer : coordinates

       glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(0));
       glEnableVertexAttribArray(0);

       // 2nd attribute buffer : colors
       glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)) );
       glEnableVertexAttribArray(1);

       // 3rd attribute buffer : uvs
       glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
       glEnableVertexAttribArray(2);

       glBindVertexArray(0);
}


void MarioCube::draw()
{
    glBindVertexArray( meshComp->mVAO );
    glUniformMatrix4fv( meshComp->mMatrixUniform, 1, GL_FALSE, transformComp->mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, meshComp->mVertices.size());
}
