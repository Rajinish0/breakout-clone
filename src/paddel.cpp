#include "gameobjects/paddle.h"

Paddle::Paddle(glm::vec2 position, glm::vec2 size, 
               Texture2D sprite, float speed)
    :GameObject(position, size, sprite), speed(speed)
{}

void Paddle::move(glm::vec2 direc, float dt){
    pos += direc*dt*speed;
}

glm::vec2 Paddle::getPos(){
    return this->pos;
}