#ifndef FACTORY_H
#define FACTORY_H

#include "iostream"
#include "gameobject.h"
#include <unordered_map>

class Factory
{
public:
    Factory();
    std::vector<GameObject*> mGameObjects;
    void createObject(std::string objectName);
};

template <typename R>
class resourceManager
{
public:

private:
    int resourceId;
    std::unordered_map<std::string, std::pair<int, std::shared_ptr<R>>> resources;

protected:
};

#endif // FACTORY_H
