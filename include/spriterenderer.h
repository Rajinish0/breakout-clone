#ifndef SPRITE_H
#define SPRITE_H

#include "shader.h"
#include "texture.h"

class SpriteRenderer{
public:
    Shader* shader;
    SpriteRenderer(Shader* shader);
    ~SpriteRenderer();

    void drawSprite(Texture2D* texture, glm::vec2 position, 
                    glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,
                    glm::vec3 color = glm::vec3(1.0f));

private:
    unsigned int quadVAO, VBO;
    float *verts;

    void initRenderData();
};

#endif