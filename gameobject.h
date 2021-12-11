#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
/**
  "GameObject" er entitiene i ECS systemet og omtrent alt i spillscenen er bygd med gameobjects som utgangspunkt.
  Idet en lager gameobjectet er det tomt, men en kan legge til ferdige komponenter for å få ønskede egenskaper av gameobjectet.
 */
class GameObject : public QOpenGLFunctions_4_1_Core {
public:
    std::string mName{"DefaultName"};

    TransformComponent* mTransform{nullptr};
    MeshComponent* mMesh{nullptr};
    MaterialComponent* mMaterial{nullptr};
    SoundComponent* mSoundComponent{nullptr};
    PhysicsComponent* mPhysicsComponent{nullptr};
};
#endif // GAMEOBJECT_H
