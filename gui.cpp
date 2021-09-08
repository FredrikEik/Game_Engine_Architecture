#include "gui.h"

gui::DropDownList::DropDownList(float x, float y, float width, float height)
{
    x = 0.5;
    y = 0.5;
    height = 50;
    width = 100;
}
gui::DropDownList::~DropDownList()
{
    delete this;
}
