#ifndef SURFACE_H
#define SURFACE_H

#include "visualobject.h"
#include "gameobject.h"


class Surface : public GameObject
{
public:
    Surface(std::string filename);
    ~Surface() override;

    std::string filename;
    void readFile(std::string filename);

    void init() override;
    void draw() override;
};

/*class ContourLines : public GameObject
{
public:
    ContourLines();
    ~ContourLines() {};
    void createContourLines();
    void draw();
    void init();
    void move(float x, float y, float z) {0;};
    std::vector<Vertex> contourLineVertexData;
};*/

#endif // SURFACE_H
