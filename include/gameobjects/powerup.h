#ifndef POW_UP_H
#define POW_UP_H

#include "gameobjects/gameobject.h"

const glm::vec2 POWUP_SIZE(60.0f, 20.0f);
const glm::vec2 POWUP_VEL(0.0f, 150.0);

class Powerup : public GameObject{
public:
    enum TYPES{
        SPEED,
        STICKY,
        PASS_THROUGH,
        PAD_SZ_INCR,
        CONFUSE,
        CHAOS
    };
    TYPES type;
    float duration;
    bool activated;

    Powerup(TYPES type, glm::vec3 col, float duration, glm::vec2 pos, Texture2D& texture)
        : GameObject(pos, POWUP_SIZE, texture, col, POWUP_VEL), type(type), duration(duration), activated(false)
    {}
};

#endif