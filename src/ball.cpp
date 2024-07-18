#include "gameobjects/ball.h"

Ball::Ball()
    :GameObject(), radius(0.0f), stuckToTheBoard(false)
{}

Ball::Ball(glm::vec2 pos, float radius, glm::vec2 vel, Texture2D sprite)
    :GameObject(pos, glm::vec2(radius) * 2.0f, sprite, glm::vec3(1.0f), vel), 
     radius(radius), stuckToTheBoard(true), sticky(false), passThrough(false){}

glm::vec2 Ball::move(float dt, unsigned int window_width){
    if (!stuckToTheBoard){
        this->pos += this->vel * dt;

        if (this->pos.x <= 0.0f){
            this->vel.x *= -1;
            this->pos.x = 0.0f;
        }

        if (this->pos.x + this->size.x >= window_width){
            this->vel.x *= -1;
            this->pos.x = window_width - this->size.x;
        }

        if (this->pos.y <= 0){
            this->vel.y *= -1;
            this->pos.y = 0.0f;
        }
    }

    return this->pos;
}

void Ball::reset(glm::vec2 pos, glm::vec2 vel){
    this->pos = pos;
    this->vel = vel;
}