#include "shader.h"
#include "gamelevel.h"
#include "gameobjects/gameobject.h"
#include "gameobjects/powerup.h"
#include <vector>

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};


class Game{
public:
    GameState state;
    std::vector<GameLevel> levels;
    unsigned int level;
    bool keys[1024];
    bool keyProcessed[1024];
    unsigned int width, height;
    unsigned int lives;
    std::vector<Powerup> powerups;

    Game(unsigned int width, unsigned int height);
    ~Game();
    void Init();
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();

    void resetLevel();
    void resetPlayer();
    void doCollisions();
    void generatePowerup(GameObject& object);
    void updatePowerups(float dt);
    bool existsAnotherWithType(Powerup::TYPES type);
};