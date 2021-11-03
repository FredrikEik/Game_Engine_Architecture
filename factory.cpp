#include "camera.h"
#include "factory.h"
#include "objreader.h"
#include "constants.h"
#include <QDebug>

#define EXISTS(x) mMeshMap.find(x) != mMeshMap.end()

Factory::Factory()
{
    for (uint32_t ID = 0; ID < gsl::MAX_OBJECTS; ++ID)
    {
        mAvailableIDs.push(ID);
    }
}

GameObject* Factory::createObject(std::string objectName)
{

    GameObject *objectToCreate = new GameObject();

    if(objectName == "Cube")
    {
        if(EXISTS("Cube")) //If cube mesh exists
        {
        uint32_t index = mMeshMap["Cube"];
        objectToCreate->meshComp = storedMeshes[index];
        }
        objectToCreate->materialComp.mShaderProgram = 1;
        objectToCreate->materialComp.mTextureUnit = 1;
        objectToCreate->sphereCollisionComp.center = gsl::Vector3D( 0.0f,  0.0f,  0.0f);
        objectToCreate->sphereCollisionComp.radius = 0.5;
        cubecounter++;
        objectToCreate->mObjectName = "Cube " + std::to_string(cubecounter);
    }

    else if(objectName == "Plane")
    {
        if(EXISTS("Plane")) //If Plane mesh exists
        {
            uint32_t index = mMeshMap["Cube"];
            objectToCreate->meshComp = storedMeshes[index];
        }
        objectToCreate->materialComp.mShaderProgram = 1;
        objectToCreate->materialComp.mTextureUnit = 1;
        planecounter++;
        objectToCreate->mObjectName = "Plane " + std::to_string(planecounter);
    }

    else if(objectName == "Triangle")
    {
        if(EXISTS("Triangle")) //If Triangle mesh exists
        {
            uint32_t index = mMeshMap["Cube"];
            objectToCreate->meshComp = storedMeshes[index];
        }
        objectToCreate->materialComp.mShaderProgram = 1;
        objectToCreate->materialComp.mTextureUnit = 1;
        trianglecounter++;
        objectToCreate->mObjectName = "Triangle " + std::to_string(trianglecounter);
    }

    else if(objectName == "MarioCube")
    {
        if(EXISTS("MarioCube")) //If MarioCube mesh exists
        {
            uint32_t index = mMeshMap["Cube"];
            objectToCreate->meshComp = storedMeshes[index];
        }
        objectToCreate->materialComp.mShaderProgram = 1;
        objectToCreate->materialComp.mTextureUnit = 1;
        objectToCreate->sphereCollisionComp.center = gsl::Vector3D( 0.0f,  0.0f,  0.0f);
        objectToCreate->sphereCollisionComp.radius = 0.5;
        mariocounter++;
        objectToCreate->mObjectName = "MarioCube " + std::to_string(mariocounter);
    }

    else if(objectName == "Sphere")
    {
        if(EXISTS("Sphere")) //If Sphere mesh exists
        {
            uint32_t index = mMeshMap["Cube"];
            objectToCreate->meshComp = storedMeshes[index];
        }
        objectToCreate->materialComp.mShaderProgram = 1;
        objectToCreate->materialComp.mTextureUnit = 1;
        objectToCreate->sphereCollisionComp.center = gsl::Vector3D( 0.0f,  0.0f,  0.0f);
        objectToCreate->sphereCollisionComp.radius = 0.25;
        spherecounter++;
        objectToCreate->mObjectName = "Sphere " + std::to_string(spherecounter);
    }
    //else if(objectName == "Camera")
    //{
    //    objectToCreate = *new Camera(45, 4/3);
    //    objectToCreate.getMeshComponent();
    //    objectToCreate.getMaterialComponent()->mShaderProgram = 0;
    //    objectToCreate.getMaterialComponent()->mTextureUnit = 0;
    //}
    else
    {
        //Default
//        objectToCreate->meshComp = storedMeshes["Cube"];
//        objectToCreate->materialComp.mShaderProgram = 1;
//        objectToCreate->materialComp.mTextureUnit = 1;
//        objectToCreate->mObjectName = "Default ";
    }

    objectToCreate->ID = mAvailableIDs.front(); //Give ID to GameObject
    mAvailableIDs.pop();                        //Pop ID from queue of available IDs

    mGameObjects.push_back(objectToCreate);
    return objectToCreate;
}

void Factory::saveMesh(std::string fileName, std::string nickName)
{
    if(EXISTS(nickName))
    {
        qDebug() << "Mesh already exists";
    }
    else
    {
        //Read the vertices and indices from the file
        ObjReader objReader;
        std::vector<Vertex> tempVertices;
        std::vector<GLuint> tempIndices;
        objReader.readFile(fileName, &tempVertices, &tempIndices);

        //Create a new mesh
        MeshComponent newMesh;
        newMesh.mDrawType = GL_TRIANGLES;
        newMesh.mVertexCount = tempVertices.size();
        newMesh.mIndexCount = tempIndices.size();
        initMesh(tempVertices, tempIndices, newMesh);

        //qDebug() << "vertices & indices" << QString::number(tempVertices.size()) << QString::number(tempIndices.size());

        storedMeshes.emplace_back(newMesh);
        mMeshMap.insert(std::pair<std::string, uint32_t>{nickName, storedMeshes.size()-1});
    }
}

void Factory::saveMesh(std::vector<Vertex> vertices, std::string nickName)
{
    if(EXISTS(nickName))
    {
        qDebug() << "Mesh already exists";
    }
    else
    {
        MeshComponent newMesh;
        newMesh.mVertexCount = vertices.size();
        newMesh.mDrawType = GL_TRIANGLES;
        initMesh(vertices, newMesh);

        storedMeshes.emplace_back(newMesh);
        mMeshMap.insert(std::pair<std::string, uint32_t>{nickName, storedMeshes.size()-1});
    }
}

void Factory::initMesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, MeshComponent &mesh)
{
    initializeOpenGLFunctions();

    // Set what shader you want to use to render this object
    //mMaterial->setActiveShader(ShaderType::TEXTURE_SHADER);
    //mMaterial->setActiveTextureSlot(2);
    //mMaterial->setupModelMatrixUniform(mMatrixUniform, matrixUniform);

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

    //Second buffer - holds the indices (Element Array Buffer - EAB):
    glGenBuffers(1, &mesh.mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Factory::initMesh(std::vector<Vertex> vertices, MeshComponent &mesh)
{
    initializeOpenGLFunctions();

    // Set what shader you want to use to render this object
    //mMaterial->setActiveShader(ShaderType::TEXTURE_SHADER);
    //mMaterial->setActiveTextureSlot(2);
    //mMaterial->setupModelMatrixUniform(mMatrixUniform, matrixUniform);


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
