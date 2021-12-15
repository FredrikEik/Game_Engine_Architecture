#ifndef ENTITYSYSTEM_H
#define ENTITYSYSTEM_H
#include "components.h"
#include <QVector3D>
#include "resourcesystem.h"
#include <vector>



/**
 * @brief The EntitySystem class - Object Factory
 */
class EntitySystem
{
public:

    /**
     * @brief EntitySystem - Empty Constructor
     */
    EntitySystem();

    //!
    //! \brief EntitySystem - Constructor used to get a pointer to renderwindow.
    //! \param inputRW - RenderWindow Pointer Parameter.
    //!
    EntitySystem(class RenderWindow * inputRW);

    //! Empty Destructor
    ~EntitySystem();



    /**
     * @brief construct - This is our main object factory function. This creates objects by creating an entity and pushing it to our entity vector. It also creates components and links them to our entity by entityID.
     * @param ObjReader - The name of our object. Character sensitive, asset must be in appropriate folder.
     * @param StartPos - Where we want our mesh to spawn.
     * @param shader - The shader unit. If forexample 0, then plain shader is used.
     * @param texture - This is the texture unit, we use only if the appropriate shader unit is used.
     * @param EntityId - This is actually quite pointless as we normally just... auto generate our entity ids. But if you want to make a test mesh and give it specific ids then thats where you set it.
     * If -1 it will auto generate an ID for you. Auto generated ids will be the largest entity id in the vector + 1.
     * @param drawType - This is our draw type, in case we want to daw object as a lines or any other thing. This was really helpful in VISIM.
     * @param inType - This is not currently properly utilized but this sets the object type. This is important for distinguishing what to remove or keep during saves or vector clearing. (which is also not implemented).
     */
    void construct(std::string ObjReader,
                   QVector3D StartPos,
                   GLuint shader = 0,
                   GLint texture = 0,
                   int EntityId = -1,
                   GLenum drawType = GL_TRIANGLES,
                   objectType inType = prop
                   );

    /**
     * @brief construcRay  - Used to create our helper for mouse clicking. (Simulates the raw thats cast from monitor to object in a 3D plane).
     * @param LineVec - Object Position/To vector.
     * @param CameraPos - Camera Position/From Vector.
     * @param Length - max legnth.
     */
    void construcRay(QVector3D LineVec, QVector3D CameraPos, float Length = 25.0f);


    /**
     * @brief constructCube - Predifined Object Construct Call - Uses EntitySystem::construct - These functions are used mostly by the UI to spawn in predefined objects.
     */
    void constructCube();

    /**
     * @brief constructSphere - Predifined Object Construct Call - Uses EntitySystem::construct - These functions are used mostly by the UI to spawn in predefined objects.
     */
    void constructSphere();

    /**
     * @brief constructPlane - Predifined Object Construct Call - Uses EntitySystem::construct - These functions are used mostly by the UI to spawn in predefined objects.
     */
    void constructPlane();

    /**
     * @brief constructSuzanne - Predifined Object Construct Call - Uses EntitySystem::construct - These functions are used mostly by the UI to spawn in predefined objects.
     */
    void constructSuzanne();

    /**
     * @brief DestroyEntity - None functioning function - intended to be used for destruction or killing of entities.
     * @param i - target entity and component index in appropriate vectors.
     */
    void DestroyEntity(int i);

    /*!
     * \brief LODSwithcer - Potentially deprecated. This is used to switch LODs on mesh components by switching active VAOs.
     * \param mesh - Target Mesh Component.
     */
    void LODSwithcer(MeshComponent *mesh);

    /**
     * @brief constructSkybox - This function constructs and adds skybox as an entity. This isnt the skybox itself but the mesh holding the skybox.
     * @param shader - skybox shader program.
     * @param texture - skybox texture(s).
     */
    void constructSkybox(GLuint shader ,GLint texture );
private:

    /**
     * @brief TransComp - Temporary Container for a tansform component pointer.
     */
    TransformComponent *TransComp;
    /**
     * @brief MeshComp - Temporary Container for a mesh component pointer.
     */
    MeshComponent *MeshComp;
    /**
     * @brief MatComp  - Temporary Container for a material component pointer.
     */
    MaterialComponent *MatComp;
    /**
     * @brief DetailsComp - Temporary Container for a details component pointer.
     */
    DetailsComponent * DetailsComp;
    /**
     * @brief ResourceSys - Temporary Container for a resource system pointer. This is used by entity system to get object VBOs.
     */
    resourceSystem * ResourceSys;
    /**
     * @brief inRW - Temporary container for renderwindow. used to access entites and components vectos.
     */
    RenderWindow * inRW;

    /**
     * @brief LastPos - Deprecated
     */
    QVector3D LastPos = QVector3D(0.0f, 0.0f, 0.0f);
    /**
    * @brief offsetLastPos - Deprecated
    */
    void offsetLastPos();
};

#endif // ENTITYSYSTEM_H
