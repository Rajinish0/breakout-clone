#ifndef RMGR_H
#define RMGR_H

#include <glad/glad.h>
#include <map>

#include "texture.h"
#include "shader.h"

class ResourceManager{
public:
    static std::map<std::string, Shader*> shaders;
    static std::map<std::string, Texture2D*> textures;

    static Shader* loadShader(std::string name, std::string vShaderfile, std::string fShaderfile, std::string gShaderfile = "");
    static Shader* getShader(std::string name);
    static Texture2D* loadTexture(std::string name, std::string file);
    static Texture2D* getTexture(std::string name);

    static void clear();

private:
    ResourceManager() {}
    static Shader* loadShaderFromFile(std::string vShaderfile, std::string fShaderfile, std::string gShaderfile = "");
    static Texture2D* loadTextureFromFile(std::string file);
};

#endif