#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H
#include "components.h"
#include "camera.h"
#include <QOpenGLFunctions_4_1_Core>

/**
 * @brief The RenderSystem class - Responsible for rendering objects and initializing vertex data to opengl.
 */
class RenderSystem : public QOpenGLFunctions_4_1_Core{
public:
    /**
     * @brief RenderSystem - Empty Constructor.
     */
    RenderSystem();

    /**
     * @brief init - standard init. Sends vertex data to opengl and prepares it for drawing. This returns a VAO which we can use to reference vertex data in the VRAM. This function is supposedly deprecated.
     * @param mMesh - mesh component used for accessing required mesh data and contains the VAO index container that will be set after init.
     */
    void init(MeshComponent* mMesh);
    /**
     * @brief init - Overload of standard init. Sends vertex data to opengl and prepares it for drawing. This returns a VAO which we can use to reference vertex data in the VRAM. Function is individualized from mesh components. This is used as an optimised alternative to save VRAM.
     * @param vertexData - Vertex data to init.
     * @param VAO - VAOs to be set.
     * @param VBO - VBOs to be set.
     */
    void init(std::vector<Vertex> * vertexData, GLuint * VAO, GLuint * VBO = 0);
    /**
     * @brief draw - Standard draw. Gets VAO index, draw type and vertex vector size for a draw call.
     * @param mMesh - Mesh component to get data from
     */
    void draw(MeshComponent* mMesh);
    /**
     * @brief draw - Advanced overload from standard draw. This is used to have one single draw function for everything. One size fits all.
     * @param mMesh - Mesh component to get data required from.
     * @param mTrasform - required to get matrix data.
     * @param viewMatrix - required to reference correct matrix data.
     * @param projectionMatrix - required to reference correct matrix data.
     * @param modelMatrix - required to reference correct matrix data.
     * @param camera - required to get matrix data.
     */
    void draw(MeshComponent* mMesh,
              TransformComponent* mTrasform,
              GLint viewMatrix,
              GLint projectionMatrix,
              GLint modelMatrix,
              Camera* camera);



};

#endif // RENDERSYSTEM_H


