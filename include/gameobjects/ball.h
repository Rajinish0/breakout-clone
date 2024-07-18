#ifndef BALL_H
#define BALL_H

#include "gameobjects/gameobject.h"

class Ball : public GameObject {
public:
    float radius;
    bool stuckToTheBoard;
    bool sticky, passThrough;

    Ball();
    Ball(glm::vec2 pos, float radius, glm::vec2 vel, Texture2D sprite);
    glm::vec2 move(float dt, unsigned int window_width);
    void reset(glm::vec2 pos, glm::vec2 vel);
};

#endif