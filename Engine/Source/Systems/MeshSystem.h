#pragma once
#include "../CoreMinimal.h"
#include "BaseSystem.h"
#include <string>
#include <filesystem>
#include "../Components/Components.h"



//class MeshComponent;

class MeshSystem final : public BaseSystem
{
public:
	static bool loadMesh(const std::filesystem::path& filePath, struct MeshComponent& meshComponent);
    static bool MeshSystem::loadMeshLOD(const std::filesystem::path& filePath, MeshComponent& meshComponent, LODMeshType type);
    //static bool defaultMesh(enum DefaultAsset, class MeshComponent& meshComponent);
    static void draw(class Shader* shader, const std::string& uniformName, class ECSManager* manager, uint32 cameraEntity);

    static void drawOutline(Shader* shader, const std::string& uniformName, ECSManager* ECS);

    static void MeshSystem::drawRTSSelection(Shader* shader, const glm::vec3& startPoint, const  glm::vec3& endPoint,
        const std::string& uniformName, MeshComponent& meshComp, TransformComponent& transformComp);

    static void drawSelectableEditor(Shader* shader, const std::string& uniformName, ECSManager* manager);
    
    static void copyMesh(const struct MeshComponent& meshToCopy, struct MeshComponent& newMesh);


private:
	static void initialize(class MeshComponent& meshComponent);
	static bool readObj(const std::filesystem::path& filePath, struct MeshComponent& meshComponent);
    static std::vector<uint32> getMeshesToDraw(class ECSManager* ECS,
        const std::vector<class MeshComponent>& allMeshes, uint32 cameraEntity);

    static std::vector<uint32> GetMeshesToDrawAABB(class ECSManager* ECS,
        const std::vector<class MeshComponent>& allMeshes, uint32 cameraEntity);

    //static void loadCube(class MeshComponent& meshComponent);


    // Used to keep track of the indexes for a single vertex in a face from obj files
    struct ObjTriangleIndex
    {
        int posIndex{};
        int normalIndex{};
        int uvIndex{};

        bool operator ==(const ObjTriangleIndex& other) const
        {
            return posIndex == other.posIndex && normalIndex == other.normalIndex && uvIndex == other.uvIndex;
        }
    };

    // Simple hash function so ObjTriangleIndex can be used as a key in unordered_map
    struct HashObjTriangleIndex
    {
        std::size_t operator() (const ObjTriangleIndex& objTriangle) const
        {
            std::size_t hash1 = std::hash<int>()(objTriangle.posIndex);
            std::size_t hash2 = std::hash<int>()(objTriangle.normalIndex);
            std::size_t hash3 = std::hash<int>()(objTriangle.uvIndex);

            return hash1 ^ hash2 ^ hash3;
        }
    };
};