#include "texture.h"
#include <iostream>

Texture2D::Texture2D()
    :width(0), height(0), internalFormat(GL_RGB), imgaeFormat(GL_RGB),
     wrapS(GL_REPEAT), wrapT(GL_REPEAT), filterMin(GL_LINEAR), filterMag(GL_LINEAR){
        glGenTextures(1, &this->ID);
        std::cout << "TEXTURE new id: "<< this->ID << std::endl;
     }

void Texture2D::Generate(unsigned int width, unsigned int height, unsigned char *data, unsigned int nChannels,
                         bool updateInternalFormat){
    this->width = width;
    this->height = height;
    GLenum format;
    switch (nChannels){
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
    }
    this->imgaeFormat = format;
    if (updateInternalFormat)
        internalFormat = format;
    
    glBindTexture(GL_TEXTURE_2D, this->ID);
    glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat, width, height, 0, this->imgaeFormat, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filterMag);    
	glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const{
    glBindTexture(GL_TEXTURE_2D, this->ID);
}

Texture2D::~Texture2D(){
    // glDeleteTextures(1, &this->ID);
}