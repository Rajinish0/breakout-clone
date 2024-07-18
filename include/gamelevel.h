#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include "spriterenderer.h"
#include <vector>
#include "gameobjects/gameobject.h"

typedef unsigned int uint;
typedef std::vector<std::vector<uint>> level_matrix;

class GameLevel{
public:
    /*
    brick types
    0 - no brick
    1 - solid brick (unbreakable)
    >=2 - different color destructable bricks
    */
    std::vector<GameObject> bricks;
    void load(std::string file, uint lvlWidth, uint lvlHeight);
    void draw(SpriteRenderer* renderer);
    bool isFinished();
private:
    void init(level_matrix tiles, 
              uint lvlWidth, uint lvlHeight);
};

#endif