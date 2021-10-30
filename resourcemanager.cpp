#include "resourcemanager.h"

#include "qdebug.h"
#define EXISTS(x) storedMeshes.find(x) != storedMeshes.end()

ResourceManager::ResourceManager()
{

}

void ResourceManager::addMesh(std::string filePath)
{
    if(EXISTS(filePath))
    {
        qDebug() << "Mesh already exists.";
    }
    else
    {
        std::vector<Vertex> tempVertices;
        std::vector<GLuint> tempIndices;
        OBJreader.readFile(filePath, &tempVertices, &tempIndices);

        Mesh meshToInsert;
        initMesh(meshToInsert, tempVertices);
        meshToInsert.mDrawType = GL_TRIANGLES;
        meshToInsert.mVertexCount = tempVertices.size();
        meshToInsert.mIndexCount = tempIndices.size();
        storedMeshes.insert({filePath, meshToInsert});
    }

}

Mesh ResourceManager::getMesh(std::string filePath)
{
    if(EXISTS(filePath))
    {
        qDebug() << "filepath exists";
        return storedMeshes["filePath"];
    }
    else{
        qDebug() << "filepath not found";
    }
}

void ResourceManager::initMesh(Mesh mesh, std::vector<Vertex> vertices)
{
    initializeOpenGLFunctions();

    glGenVertexArrays( 1, &mesh.mVAO );
    glBindVertexArray( mesh.mVAO );


    glGenBuffers( 1, &mesh.mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mesh.mVBO );

    glBufferData( GL_ARRAY_BUFFER,                     //what buffer type
                  vertices.size() * sizeof( Vertex ), //how big buffer do we need
                  vertices.data(),                    //the actual vertices
                  GL_STATIC_DRAW                       //should the buffer be updated on the GPU
                );

    glBindBuffer(GL_ARRAY_BUFFER, mesh.mVBO);

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

