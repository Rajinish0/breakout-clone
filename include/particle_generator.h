#ifndef P_GEN_H
#define P_GEN_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.h"
#include "gameobjects/gameobject.h"
#include "texture.h"

#include <vector>

struct Particle{
    glm::vec2 pos, vel;
    glm::vec4 col;
    float life;
    Particle()
        : pos(0.0f), vel(0.0f), col(1.0f), life(0.0f){}
};

class ParticleGenerator{
public:
    ParticleGenerator(Shader shader, Texture2D texture, unsigned int amt);
    void update(float dt, GameObject* obj, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f));
    void draw();
private:
    std::vector<Particle> particles;
    unsigned int amt;
    Shader shader;
    Texture2D texture;
    unsigned int VAO;
    void init();
    unsigned int firstUnusedParticle();
    void respawnParticle(Particle& particle, GameObject* obj, glm::vec2 offset = glm::vec2(0.0f));
};  

#endif