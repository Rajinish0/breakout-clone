#ifndef GAMEOBJ_H
#define GAMEOBJ_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "spriterenderer.h"

class GameObject{
public:
    glm::vec2 pos, size, vel;
    glm::vec3 color;
    float rotation;
    bool isSolid;
    bool destroyed;

    Texture2D sprite;
    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, 
    glm::vec3 color = glm::vec3 (1.0f), glm::vec2 vel = glm::vec2(0.0f));

    virtual void draw(SpriteRenderer* renderer);
    virtual ~GameObject() {}
};

#endif