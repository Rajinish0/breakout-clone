#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

class Texture2D{
public:
    unsigned int ID, width, height, internalFormat, imgaeFormat,
                 wrapS, wrapT, filterMin, filterMag;
    Texture2D();
    ~Texture2D();
    void Generate(unsigned int width, unsigned int height, unsigned char *data,
                  unsigned int nChannels = 3, bool updateInternalFormat = true);
    void Bind() const;
};

#endif