#include "spriterenderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


SpriteRenderer::SpriteRenderer(Shader* shader)
    :shader(shader){
    std::cout << "spnum: " << shader->shaderProg << std::endl;
    this->initRenderData();
}

void SpriteRenderer::initRenderData(){
    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &this->VBO);
    /*
    * when position is given in drawSprite it defines the top left corner
    * but here in vertices 0,0 defines the bottom left corner, so what's happening?
    * 
    * The orth matrix is defined such that it interprets 0,0 is the top left coord
    * and 800, 600 as the bottom right coord. So for this set of vertices, they
    * basically get flipped around the y axis. And the bottom left corner (0,0)
    * ends up defining the top left corner.
    */
    float vertices[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glBindVertexArray(this->quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void SpriteRenderer::drawSprite(Texture2D* texture, glm::vec2 position, 
                    glm::vec2 size, float rotate, glm::vec3 color){
                
    shader->use();
    glm::mat4 model = glm::mat4(1.0f);
    // it's a bummer that these are applied from bottom to top
    // but anyway this is the classic Rtheta, C
    // rotating smth around a center C
    // rn im assuming rotation is about the center of this quad
    // but i can change the center to smth else later, if needed.

    //since the y axis is flipped after orth
    //the rotation happens clockwise, so i m just going to 
    //make this negative here
    model = glm::translate(model, glm::vec3(position, 0.0f));
    if (rotate != 0.0f){
        model = glm::translate(model, glm::vec3(size/2.0f, 0.0f) );
        model = glm::rotate(model, glm::radians(-rotate), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-size/2.0f, 0.0f) );
    }    
    model = glm::scale(model, glm::vec3(size, 1.0f));
    shader->setMatrix("model", model);
    shader->setVec3("color", color);

    shader->setInt("image", 0); // just precautionary
    glActiveTexture(GL_TEXTURE0);
    texture->Bind();

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

SpriteRenderer::~SpriteRenderer(){
    // delete[] verts;
    glDeleteVertexArrays(1, &this->quadVAO);
}