#ifndef JSONSYSTEM_H
#define JSONSYSTEM_H
#include <iostream>
#include <fstream>
#include <ostream>
#include <istream>
#include "components.h"

/**
 * @brief The JSONSystem class - Uses nlohmann json library to create json objects that can be stored in a json file.
 */
class JSONSystem
{
public:
    /**
     * @brief JSONSystem - Empty Constructor
     */
    JSONSystem();

    /**
     * @brief JSONSystemInit - gets and sets renderwindow
     * @param inRW - renderwindow pointer
     */
    void JSONSystemInit(class RenderWindow * inRW);
    /**
     * @brief SaveLevel - Saves level entities as json file
     * @param filepath - file path to save to
     */
    void SaveLevel(std::string filepath);
    /**
     * @brief LoadLevel - Loads level entities from json file
     * @param filepath - file path to load from
     */
    void LoadLevel(std::string filepath);

private:
    /**
     * @brief RW - Used to access entities
     */
    class RenderWindow * RW;
};

#endif // JSONSYSTEM_H
