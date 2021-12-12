#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H
#include "components.h"
#include "camera.h"
#include <QOpenGLFunctions_4_1_Core>

class RenderSystem : public QOpenGLFunctions_4_1_Core{
public:
    RenderSystem();
    //virtual ~RenderSystem();
    void init(MeshComponent* mMesh);
    void init(std::vector<Vertex> * vertexData, GLuint * VAO, GLuint * VBO = 0);
    void draw(MeshComponent* mMesh);
    void draw(MeshComponent* mMesh,
              MaterialComponent* mMaterial,
              TransformComponent* mTrasform,
              GLint viewMatrix,
              GLint projectionMatrix,
              GLint modelMatrix,
              Camera* camera);


};

#endif // RENDERSYSTEM_H


