#pragma once
#include "../CoreMinimal.h"
#include "BaseSystem.h"
#include <string>
#include <filesystem>

//class MeshComponent;
class MeshSystem final : public BaseSystem
{
public:
	static bool loadMesh(const std::filesystem::path& filePath, class MeshComponent& meshComponent);

private:
	static void initialize(class MeshComponent& meshComponent);
	static bool readObj(const std::filesystem::path& filePath, class MeshComponent& meshComponent);


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