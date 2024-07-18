#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "spriterenderer.h"
#include "resourcemanager.h"
#include "texture.h"
#include "gameobjects/gameobject.h"
#include <string>

class Paddle : public GameObject{
public:
    Paddle(glm::vec2 position, glm::vec2 size, Texture2D sprite,
           float speed = 10.0f);
    void move(glm::vec2 direc, float dt);
    glm::vec2 getPos();
    float speed;
};