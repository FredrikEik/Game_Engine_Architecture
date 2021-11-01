#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <unordered_map>

class ResourceManager
{
public:
    ResourceManager();

    void readObj();
    void requestObj(std::string object);

private:
    std::unordered_map<std::string, class Texture*> m_textureMap;
};

#endif // RESOURCEMANAGER_H
