#include "resourcemanager.h"
#include "stb_image.h"


std::map<std::string, Shader*> ResourceManager::shaders;
std::map<std::string, Texture2D*> ResourceManager::textures;


Shader* ResourceManager::loadShaderFromFile(std::string vShaderfile, std::string fShaderfile, std::string gShaderfile){
    Shader* shdr = new Shader(vShaderfile, fShaderfile, gShaderfile);
    return shdr;
}

Texture2D* ResourceManager::loadTextureFromFile(std::string file){
    Texture2D* t = new Texture2D();
    int width, height, nChannels;
	unsigned char* data = stbi_load(file.c_str(), &width, &height, &nChannels, 0);

    t->Generate(width, height, data, nChannels);

    stbi_image_free(data);
    return t;
}


Shader* ResourceManager::getShader(std::string name){
    if (shaders.count(name) > 0){
        // std::cout << "GET: " << shaders[name]->shaderProg << std::endl;
        return shaders[name];
    }
    else 
        throw std::runtime_error("inv");
}

Texture2D* ResourceManager::getTexture(std::string name){
    return textures[name];
}

Shader* ResourceManager::loadShader(std::string name, std::string vShaderfile, std::string fShaderfile, std::string gShaderfile){
    // if (shaders.count(name) > 0)
    //     return shaders[name];
    shaders[name] = ResourceManager::loadShaderFromFile(vShaderfile, fShaderfile, gShaderfile);
    std::cout << "LOAD: " << shaders[name]->shaderProg << std::endl;
    return shaders[name];
}

Texture2D* ResourceManager::loadTexture(std::string name, std::string file){
    // if (textures.count(name) > 0)
    //     return textures[name];
    std::cout << "loading texture: " << name << std::endl;
    textures[name] = ResourceManager::loadTextureFromFile(file);
    return textures[name];
}

void ResourceManager::clear(){
    for (auto shdrMap : shaders){
        glDeleteProgram(shdrMap.second->shaderProg);
        delete shdrMap.second;
    }

    for (auto txtMap : textures){
        glDeleteTextures(1, &txtMap.second->ID);
        delete txtMap.second;
    }

    shaders.clear();
    textures.clear();
}