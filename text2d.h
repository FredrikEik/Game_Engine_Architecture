#ifndef TEXT2D_H
#define TEXT2D_H

#include <vector>
#include "gsl/vector2d.h"


class Text2D
{
public:
    Text2D();
    ~Text2D();

    void initText2D(const char * texturePath);
    void printText2D(const char * text, int x, int y, int size);
    void cleanupText2D();

    std::vector<gsl::Vector2D> vertices;
    std::vector<gsl::Vector2D> UVs;

};

#endif // TEXT2D_H
