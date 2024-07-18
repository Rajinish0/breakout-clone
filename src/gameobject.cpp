#include "gameobjects/gameobject.h"

GameObject::GameObject()
    :pos(glm::vec2(0.0f)), size(glm::vec2(0.0f)), rotation(0.0f), 
    isSolid(false), destroyed(false), sprite(),
    color(glm::vec3(1.0f)), vel(glm::vec2(0.0f))
{}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite,
 glm::vec3 color, glm::vec2 vel)
    :pos(pos), size(size), vel(vel), sprite(sprite), color(color), isSolid(false), destroyed(false){
        // std::cout << "OBJ CREATED WITH SPRITE ID: " << sprite.ID << std::endl;
    }

void GameObject::draw(SpriteRenderer* renderer){
    renderer->drawSprite(&this->sprite, this->pos, this->size, this->rotation, this->color);
}