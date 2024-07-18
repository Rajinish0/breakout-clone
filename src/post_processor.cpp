#include "post_processor.h"

/*
what we want to do is render to a multi sampled buffer 
to avoid anti aliasing, however that fbo cannot be directly fed 
to shaders for post-processing, so the contents of the multi sampled buffer
must be transfered to another fbo (normal fbo) using glBlitFrameBuffer
Then post processing can be done on this fbo using the normal things

MSFBO -> fbo for multi sampling, this is where stuff will be drawn
FBO -> this is where MSFBO wil be "realized" (just copy from MSFBO to this)
       so that we can do post processing
RBO -> render buffer obj

the msfbo is screwed up for some reason, so im just directly drawing to the fbo,
there aren't any visible quirks as of now.
*/

PostProcessor::PostProcessor(Shader& shader, unsigned int width, unsigned int height)
    :postProcShdr(shader), width(width), height(height), texture(),
     confuse(false), chaos(false), shake(false){

    glGenFramebuffers(1, &this->MSFBO);
    glGenFramebuffers(1, &this->FBO);
    glGenRenderbuffers(1, &this->RBO);

    glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
    // dont need stencil or depth buffers for the render buffer
    // because (the game is 2d..)
    // allocates the multisample render buffer (with 4 samples per pixel)
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, width, height);
    //attach the render buffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->RBO);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR while initializing MSFBO" << std::endl;

    //going to use texture for this
    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
    this->texture.Generate(width, height, NULL);
    std::cout << "Pproc t id : " << this->texture.ID << std::endl;
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture.ID, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR while initializing color FBO" << std::endl;

    this->initRenderData();
    this->postProcShdr.use();

    this->postProcShdr.setInt("scene", 0);
    float offset = 1.0f / 300.0f;
    float offsets[9][2] = {
        { -offset,  offset  },  // top-left
        {  0.0f,    offset  },  // top-center
        {  offset,  offset  },  // top-right
        { -offset,  0.0f    },  // center-left
        {  0.0f,    0.0f    },  // center-center
        {  offset,  0.0f    },  // center - right
        { -offset, -offset  },  // bottom-left
        {  0.0f,   -offset  },  // bottom-center
        {  offset, -offset  }   // bottom-right 
    };
    //im not going to introduce another function for this in shader.h
    glUniform2fv(this->postProcShdr.getLoc("offsets"), 9, (float*)offsets);

    int edgeKernel[9] = { 
        -1, -1, -1,
        -1,  8, -1,
        -1, -1, -1
    };
    glUniform1iv(this->postProcShdr.getLoc("edgeKernel"), 9, edgeKernel);

    float blurKernel[9] = {
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
        2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
    };

    glUniform1fv(this->postProcShdr.getLoc("blurKernel"), 9, blurKernel);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/*
this will be called before rendering the game objects
so the multisampled fbo should be bound (binded?) here
*/
void PostProcessor::beingRender(){
    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

/*
this will be called after everything is done
so we just need to copy info from msfbo, to fbo
this fills up the texture2d (which was holding color info)
of the fbo

and then the default framebuffer should also be bound
because the quad will be drawn on that (using colors from fbo)
*/

void PostProcessor::endRender(){
    // glBindFramebuffer(GL_READ_BUFFER, this->MSFBO);
    // glBindFramebuffer(GL_DRAW_BUFFER, this->FBO);
    // glBlitFramebuffer(0, 0, this->width, this->height, 0, 0, this->width, this->height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::render(float time){
    this->postProcShdr.use();
    this->postProcShdr.setBool("confuse", this->confuse);
    this->postProcShdr.setBool("chaos", this->chaos);
    this->postProcShdr.setBool("shake", this->shake);
    this->postProcShdr.setFloat("time", time);

    glActiveTexture(GL_TEXTURE0);
    this->texture.Bind(); // has colors from fbo (which is just msfbo in disguise)
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void PostProcessor::initRenderData(){
    //these are just the whole screen coords (the fbo quad will be rendered here)
    // I NEED TO MAKE A .H file including all kinds of these vertices
    float vertices[] = {
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,

        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f
    };

    unsigned int VBO;
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}