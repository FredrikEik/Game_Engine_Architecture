#ifndef TEXT2D_H
#define TEXT2D_H

#include "gameobject.h"


class Text2D : public GameObject
{
public:
    Text2D(const char * text, int x, int y);
    ~Text2D();

    void draw() override;
    void init() override;

    void printText2D(const char * text, int x, int y, int size);
    void updateText(const char * text);

    int mX { 0 };
    int mY { 0 };

    std::vector<gsl::Vector2D> vertices;
    std::vector<gsl::Vector2D> UVs;

};

#endif // TEXT2D_H
