#include "gamelevel.h"
#include "resourcemanager.h"
#include <istream>
#include <sstream>


void GameLevel::load(std::string file, uint lvlWidth, uint lvlHeight){
    this->bricks.clear();
    std::cout << "HERE LOADING" << std::endl;
    level_matrix tiles;
    uint tCode;
    std::ifstream file_stream (file);

    if (!file_stream){
        std::cerr << "Failed to load level: " << file << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file_stream, line)){
        std::stringstream s_stream(line);
        std::vector<uint> row;
        while (s_stream >> tCode)
            row.push_back(tCode);
        tiles.push_back(row);
    }

    init(tiles, lvlWidth, lvlHeight);
}

void GameLevel::init(level_matrix tiles, uint lvlWidth, uint lvlHeight){
    float brickHeight = ( (float) lvlHeight) / tiles.size() ;
    float brickWidth  =  ( (float) lvlWidth) / tiles[0].size();

    glm::vec2 size(brickWidth, brickHeight);

    for (unsigned int i =0; i < tiles.size(); i++){
        for (unsigned int j =0; j < tiles[0].size(); j++){
            std::cout << tiles[i][j] << " ";
            if (tiles[i][j]  == 1) // solid wall
            {
                GameObject gobj (glm::vec2(j * brickWidth, i * brickHeight), 
                            size,
                            *ResourceManager::getTexture("nobreak_brick")
                            );
                gobj.isSolid = true;
                this->bricks.push_back(gobj);
            }

            else if (tiles[i][j] >= 1){
                glm::vec3 color = glm::vec3(1.0f); // original: white
                if (tiles[i][j] == 2)
                    color = glm::vec3(0.2f, 0.6f, 1.0f);
                else if (tiles[i][j] == 3)
                    color = glm::vec3(0.0f, 0.7f, 0.0f);
                else if (tiles[i][j] == 4)
                    color = glm::vec3(0.8f, 0.8f, 0.4f);
                else if (tiles[i][j] == 5)
                    color = glm::vec3(1.0f, 0.5f, 0.0f);
                GameObject gobj (glm::vec2(j * brickWidth, i * brickHeight),
                                 size,
                                 *ResourceManager::getTexture("break_brick"),
                                 color
                                 );
                this->bricks.push_back(gobj);
            }
        }
        std::cout << std::endl;
    }
    std::cout << "loaded" << std::endl;
}

void GameLevel::draw(SpriteRenderer* renderer){
    for (auto& brick: bricks)
        if (!brick.destroyed)
            brick.draw(renderer);
}

bool GameLevel::isFinished(){
    for (auto& brick: bricks){
        if (!brick.isSolid && !brick.destroyed)
            return false;
    }
    return true;
}