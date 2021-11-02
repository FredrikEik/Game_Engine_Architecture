#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QOpenGLFunctions_4_1_Core>

struct TransformComponent;
struct MeshComponent;
struct MaterialComponent;


class GameObject : public QOpenGLFunctions_4_1_Core
{
public:
    TransformComponent* m_Transform{nullptr};
    MeshComponent* m_Mesh{nullptr};
    MaterialComponent* m_Material{nullptr};

    std::string m_Name{"Nameless"};
};

#endif // GAMEOBJECT_H
