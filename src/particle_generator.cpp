#include "particle_generator.h"

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, unsigned int amt)
    :shader(shader), texture(texture), amt(amt){
        init();
    }

void ParticleGenerator::init(){
    float vertices[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    unsigned int VBO;
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    particles = std::vector<Particle>(this->amt);
}

unsigned int findex = 0;

unsigned int ParticleGenerator::firstUnusedParticle(){
    for (unsigned int i =findex; i < amt; ++i){
        if (particles[i].life <= 0.0f){
            findex =i;
            return i;
        }
    }

    for (unsigned int i =0; i < findex; ++i){
        if (particles[i].life <= 0.0f){
            findex =i;
            return i;
        }
    }
    
    findex =0;
    return 0;
}


void ParticleGenerator::respawnParticle(Particle& particle, GameObject* obj, glm::vec2 offset){
    float random = ((rand() % 100) - 50) / 10.f; // [-5.0, 4.9]
    float randCol = 0.5f + ((rand() % 100) / 100.0f);
    particle.pos = obj->pos + random + offset;
    particle.vel = obj->vel * 0.1f;
    particle.life = 1.0f;
    particle.col = glm::vec4(glm::vec3(randCol), 1.0f);
}

void ParticleGenerator::update(float dt, GameObject* obj, unsigned int newParticles, glm::vec2 offset){
    for (unsigned int i =0; i < newParticles; ++i){
        unsigned int funused = this->firstUnusedParticle();
        respawnParticle(this->particles[funused], obj, offset);
    }

    for (Particle& particle : this->particles){
        particle.life -= dt;
        if (particle.life > 0.0f){
            particle.pos -= particle.vel * dt;
            particle.col.w -= dt * 2.5f;
        }
    }
}

void ParticleGenerator::draw(){
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    this->shader.use();
    for (Particle& particle : this->particles){
        if (particle.life > 0.0f){
            this->shader.setVec2("offset", particle.pos);
            this->shader.setVec4("color", particle.col);
            this->texture.Bind();
            glBindVertexArray(this->VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}