#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

struct MeshData
{
    //according to https://www.learncpp.com/cpp-tutorial/arrays-part-ii/
    //an empty {} should initialize all elements to 0
    GLuint mVAO[3]{};
    GLuint mVBO[3]{};
    GLuint mEAB[3]{};

    //want all these to be at -1 so I do it explicitly
    GLint mVertexCount[3]{-1, -1, -1};
    GLint mIndexCount[3]{-1, -1, -1};
    GLenum mDrawType{GL_TRIANGLES};
    std::vector<Vertex> mVertices[3];
    std::vector<GLuint> mIndices[3];

    //BoundingSphere:
    float mColliderRadius{0};
};

class ResourceManager : public QOpenGLFunctions_4_1_Core
{
    friend class ECScoordinator;

public:
    int LoadAsset(std::string Filepath);
    gsl::AssetType FindAssetType(std::string AssetName);
    int readObj(std::string filepath);
    void initMesh(MeshData &tempMesh, int lodLevel);

    MeshData getMeshdata(int meshIndex);

private:
    //Private constructor called from Create(), only possible from ECScoordinator.
    ResourceManager();

    static std::unique_ptr<ResourceManager> Create()
    { return std::unique_ptr<ResourceManager>( new ResourceManager() ); }

    std::vector<MeshData> m_meshes;
    std::map<std::string, unsigned int> m_meshMap;

};

#endif // RESOURCEMANAGER_H
