#ifndef CUBEMAP_H
#define CUBEMAP_H

#include "texture.h"
#include "array"

class Cubemap : public Texture
{
public:
    Cubemap(const std::array<std::string, 6> &fileNames);

    virtual void setTexture() override;

    const std::array<std::string, 6> m_fileNames;
    std::array<unsigned char*, 6> m_bitmaps{};

protected:
    void readCubemap();

};

#endif // CUBEMAP_H
