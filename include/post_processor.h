#ifndef POST_PROC_H
#define POST_PROC_H

#include "shader.h"
#include "texture.h"

class PostProcessor{

public:
    Shader& postProcShdr;
    Texture2D texture;
    unsigned int width, height;

    bool confuse, chaos, shake;

    PostProcessor(Shader& shader, unsigned int width, unsigned int height);

    void beingRender();
    void endRender();
    void render(float time);

private:
    unsigned int MSFBO, FBO;
    unsigned int RBO;
    unsigned int VAO;

    void initRenderData();

};


#endif