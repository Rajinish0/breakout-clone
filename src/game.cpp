#include "game.h"
#include "resourcemanager.h"
#include "spriterenderer.h"
#include "gameobjects/paddle.h"
#include "gameobjects/ball.h"
#include "vectorconsts.h"
#include "post_processor.h"
#include "funcs.h"
#include "audio_manager.h"
#include "particle_generator.h"
#include "textrender.h"

#include <algorithm>
#include <filesystem>


SpriteRenderer *Renderer;
Paddle *paddle;
Ball *ball;
ParticleGenerator *pGenerator;
PostProcessor *postProc;
AudioManager *audioMgr;
TextRenderer *textRenderer;


const glm::vec2 PADDLE_SIZE (150.0f, 20.0f);
const float     PADDLE_SPEED = 300.0f;
const float BALL_SPEED = 100.0f;
const float BALL_RADIUS = 12.5f;
const glm::vec2 BALL_INIT_VEL (100.0f, -350.f);
float SPEED;

float shakeTime = 0.0f;

Game::Game(unsigned int width, unsigned int height)
    :state(GAME_MENU), keys(), width(width), height(height), levels(), level(0), powerups(),
    lives(3), keyProcessed(){
    this->powerups = std::vector<Powerup>();
    SPEED = glm::length(BALL_INIT_VEL);
}

void Game::Init(){
    ResourceManager::loadShader("sprite", "shaders/sprite.vert", "shaders/sprite.frag");
    ResourceManager::loadShader("particle", "shaders/particlevs.glsl", "shaders/particlefs.glsl");
    ResourceManager::loadShader("postproc", "shaders/quadv.glsl", "shaders/quadf.glsl");
    ResourceManager::loadShader("text", "shaders/textVshader.glsl", "shaders/textFshader.glsl");


    ResourceManager::loadTexture("nobreak_brick", "textures/block_solid.png");
    ResourceManager::loadTexture("break_brick", "textures/block.png");
    ResourceManager::loadTexture("face", "textures/awesomeface.png");
    ResourceManager::loadTexture("background", "textures/background.jpg");
    ResourceManager::loadTexture("ball", "textures/awesomeface.png");
    ResourceManager::loadTexture("paddleImage", "textures/paddle.png");
    ResourceManager::loadTexture("particle", "textures/particle.png");
    ResourceManager::loadTexture("text_speed", "textures/powerup_speed.png");
    ResourceManager::loadTexture("text_sticky", "textures/powerup_sticky.png");
    ResourceManager::loadTexture("text_passthrough", "textures/powerup_passthrough.png");
    ResourceManager::loadTexture("text_increase", "textures/powerup_increase.png");
    ResourceManager::loadTexture("text_confuse", "textures/powerup_confuse.png");
    ResourceManager::loadTexture("text_chaos", "textures/powerup_chaos.png");



    std::string path = "levels";
    for (auto& lvlf : std::filesystem::directory_iterator(path)){
        GameLevel level;
        level.load(lvlf.path().string(), this->width, this->height / 2);
        this->levels.push_back(level);
    }
    this->level = 0;


    glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(this->width), 
                                static_cast<float>(this->height), 0.0f, -1.0f, 1.0f);
    ResourceManager::getShader("sprite")->use().setMatrix("proj", proj);
    ResourceManager::getShader("particle")->use().setMatrix("proj", proj);
    ResourceManager::getShader("text")->use().setMatrix("proj", proj);

    Renderer = new SpriteRenderer(ResourceManager::getShader("sprite"));
    paddle = new Paddle(glm::vec2( width/2 - PADDLE_SIZE.x / 2 , height-PADDLE_SIZE.y), glm::vec2(150.f, 20.f),
                        *ResourceManager::getTexture("paddleImage"), PADDLE_SPEED);
    ball = new Ball( glm::vec2(width/2 - BALL_RADIUS, height - PADDLE_SIZE.y  - BALL_RADIUS * 2.0f),
                     BALL_RADIUS, BALL_INIT_VEL,
                     *ResourceManager::getTexture("ball")); 
    pGenerator = new ParticleGenerator(*ResourceManager::getShader("particle"), 
                                       *ResourceManager::getTexture("particle"),
                                        500);
    postProc = new PostProcessor(*ResourceManager::getShader("postproc"),
                                 this->width, this->height);
    audioMgr = new AudioManager();
    textRenderer = new TextRenderer("fonts/ocraext.ttf");

    audioMgr->play2D("audio/breakout.wav", true);
}

void Game::Render(){
    postProc->beingRender();
    if (this->state == GAME_ACTIVE || this->state == GAME_MENU){

            Renderer->drawSprite(ResourceManager::getTexture("background"), 
                glm::vec2(0.0f), glm::vec2(this->width, this->height));
            this->levels[this->level].draw(Renderer);
            paddle->draw(Renderer);
            if (!ball->stuckToTheBoard)
                pGenerator->draw();
            ball->draw(Renderer);

            for (Powerup& powup : powerups){
                if (!powup.destroyed)
                    powup.draw(Renderer);
            }

        textRenderer->renderText(*ResourceManager::getShader("text"), 
                                "Lives: " + std::to_string(lives), 0.0f, 0.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.0f),
                                TextRenderer::TOP_LEFT);


    }

    if (this->state == GAME_MENU){
        textRenderer->renderText(*ResourceManager::getShader("text"), 
                            "Press enter to start", width/2.0f, height/2.0f, 1.0f, 
                            glm::vec3(0.2f, 0.6f, 0.6f), 
                            TextRenderer::CENTER);

        textRenderer->renderText(*ResourceManager::getShader("text"), 
                            "Press W or S to select level", width/2.0f, height/2.0f + 40.0f, 0.5f, 
                            glm::vec3(0.2f, 0.6f, 0.6f),
                            TextRenderer::CENTER);
    }

    if (this->state == GAME_WIN){
        textRenderer->renderText(*ResourceManager::getShader("text"), 
                            "YOU WON!!", width/2.0f, height/2.0f, 1.0f, 
                            glm::vec3(0.2f, 0.6f, 0.6f), 
                            TextRenderer::CENTER);
        textRenderer->renderText(*ResourceManager::getShader("text"), 
                            "Press enter to start again or ESC to quit", width/2.0f, height/2.0f + 40.0f, 0.5f, 
                            glm::vec3(0.2f, 0.6f, 0.6f), 
                            TextRenderer::CENTER);
    }

    postProc->endRender();

    postProc->render(glfwGetTime());

    // Renderer->drawSprite(ResourceManager::getTexture("face"), 
    // glm::vec2(400.0f, 300.0), glm::vec2(400.0f, 300.f), 10.0f*(float)glfwGetTime());
    // paddle->draw(Renderer);
}

void Game::ProcessInput(float dt){
    if (this->state == GAME_ACTIVE){
        float vel = PADDLE_SPEED * dt;
        if ((keys[GLFW_KEY_LEFT] || keys[GLFW_KEY_A]) && 
            paddle->getPos().x - vel > 0)
            {
                paddle->move(LEFT_VEC, dt);
                if (ball->stuckToTheBoard)
                    ball->pos.x -= vel;
            }
        else if ((keys[GLFW_KEY_RIGHT] || keys[GLFW_KEY_D]) &&
            paddle->getPos().x + paddle->size.x + vel < this->width)
            {
                paddle->move(RIGHT_VEC, dt);
                if (ball->stuckToTheBoard)
                    ball->pos.x += vel;
            }
        if (keys[GLFW_KEY_SPACE] && ball->stuckToTheBoard)
            ball->stuckToTheBoard = false;
    }

    if (this->state == GAME_MENU){
        if (this->keys[GLFW_KEY_ENTER] && !this->keyProcessed[GLFW_KEY_ENTER]){
            this->state = GAME_ACTIVE;
            this->keyProcessed[GLFW_KEY_ENTER] = true;
        }
        if (this->keys[GLFW_KEY_W] && !this->keyProcessed[GLFW_KEY_W]){
            this->level = (this->level + 1) % 4;
            this->keyProcessed[GLFW_KEY_W] = true;
        }
        if (this->keys[GLFW_KEY_S] && !this->keyProcessed[GLFW_KEY_S]){
            if (this->level > 0)
                this->level--;
            else
                this->level = 3;
            this->keyProcessed[GLFW_KEY_S] = true;
        }
    }

    if (this->state == GAME_WIN){
        if (this->keys[GLFW_KEY_ENTER]){
            this->keyProcessed[GLFW_KEY_ENTER] = true;
            postProc->chaos = false;
            this->state = GAME_MENU;
        }
    }
}

bool shouldGenerate(unsigned int chance){
    int randomNum = (rand() % chance);
    return (randomNum == 0);
}

void Game::generatePowerup(GameObject& object){
    if (shouldGenerate(75)){
        std::cout << "GENERATED SPEED" << std::endl; 
        this->powerups.push_back( Powerup(Powerup::SPEED, glm::vec3(0.5f, 0.5f, 1.0f),
                                          0.0f, object.pos, *ResourceManager::getTexture("text_speed"))
                                );
        std::cout << "GENERATED SPEED" << std::endl; 
        }
    if (shouldGenerate(75)){
        std::cout << "GENERATED STICKY" << std::endl;
        this->powerups.push_back( Powerup(Powerup::STICKY, glm::vec3(1.0f, 0.5f, 1.0f),
                                          20.0f, object.pos, *ResourceManager::getTexture("text_sticky"))
                                );
        std::cout << "GENERATED STICKY" << std::endl;
    }
    if (shouldGenerate(75)){
        std::cout << "GENERATED P_THRU" << std::endl;
        this->powerups.push_back( Powerup(Powerup::PASS_THROUGH, glm::vec3(0.5f, 1.0f, 0.5f),
                                          20.0f, object.pos, *ResourceManager::getTexture("text_passthrough"))
                                );
        std::cout << "GENERATED P_THRU" << std::endl;
    }
    if (shouldGenerate(75)){
        std::cout << "GENERATED INCREASE" << std::endl;
        this->powerups.push_back( Powerup(Powerup::PAD_SZ_INCR, glm::vec3(1.0f, 0.6f, 0.3f),
                                          0.0f, object.pos, *ResourceManager::getTexture("text_increase"))
                                );
        std::cout << "GENERATED INCREASE" << std::endl;
    }
    if (shouldGenerate(35)){
        std::cout << "GENERATED CONFUSE" << std::endl;
        this->powerups.push_back( Powerup(Powerup::CONFUSE, glm::vec3(1.0f, 0.3f, 0.3f),
                                          15.0f, object.pos, *ResourceManager::getTexture("text_confuse"))
                                );
        std::cout << "GENERATED CONFUSE" << std::endl;
    }
    if (shouldGenerate(35)){
        std::cout << "GENERATED CHAOS" << std::endl;
        this->powerups.push_back( Powerup(Powerup::CHAOS, glm::vec3(0.9f, 0.24f, 0.24f),
                                          15.0f, object.pos, *ResourceManager::getTexture("text_chaos"))
                                );
        std::cout << "GENERATED CHAOS" << std::endl;
    }
}



void activePowerup(Powerup& powup);

void Game::doCollisions(){
    for (auto& brick : levels[level].bricks){
        if (!brick.destroyed){
            Collision col = funcs::rectToCircleCollision(brick, *ball);
            if (std::get<0>(col)){
                brick.destroyed = !brick.isSolid;

                if (brick.destroyed){
                    std::cout << "GENERATING POWUP" << std::endl;
                    this->generatePowerup(brick);
                }

                glm::vec2 d = std::get<1>(col);
                if (!ball->passThrough || brick.isSolid){
                    if (d == LEFT_VEC || d == RIGHT_VEC){
                        ball->vel.x *= -1;
                        float distToMove = ball->radius - std::abs(std::get<2>(col).x);
                            if (d == LEFT_VEC)
                                ball->pos.x += distToMove;
                            else 
                                ball->pos.x -= distToMove;
                    }

                    else{
                        ball->vel.y *= -1;
                        float distToMove = ball->radius - std::abs(std::get<2>(col).y);
                        if (!ball->passThrough){
                            if (d == UP_VEC)
                                ball->pos.y += distToMove;
                            else 
                                ball->pos.y -= distToMove;
                        }
                    }
                }

                if (brick.isSolid){
                    shakeTime = 0.10f;
                    postProc->shake = true;
                    audioMgr->play2D("audio/solid.wav");
                }
                else{
                    //annoying
                    // audioMgr->play2D("audio/brick.wav");
                }
            }
        }
    }

    Collision boardCol  = funcs::rectToCircleCollision(*paddle, *ball);
    if (!ball->stuckToTheBoard && std::get<0>(boardCol)){
        float centerx = paddle->pos.x + paddle->size.x / 2.0f;
        float dist = (ball->pos.x + ball->radius) - centerx; // this can be neg
        float percent = dist / (paddle->size.x / 2.0f);

        //changing the angle at which it deflects
        float strength = 2.0f;
        float prevspeed = glm::length(ball->vel);
        ball->vel.x = BALL_INIT_VEL.x * percent * strength;
        //reversing the vel
        ball->vel.y = -std::abs(ball->vel.y);

        //maintain speed
        ball->vel = glm::normalize(ball->vel) * prevspeed;
        ball->stuckToTheBoard = ball->sticky;
    }

    for (Powerup& powup : this->powerups){
        if (!powup.destroyed){

            if (powup.pos.y >= this->height)
                powup.destroyed = true;

            if (funcs::rectToRectCollision(powup, *paddle)){
                activePowerup(powup);
                powup.destroyed = true;
                powup.activated = true;
                audioMgr->play2D("audio/powerup.wav");
            }

        }
    }

}

void Game::resetLevel(){
    for (auto& brick : this->levels[this->level].bricks){
        brick.destroyed = false;
    }
    this->lives = 3;
}

void Game::resetPlayer(){
    paddle->pos=glm::vec2( width/2 - PADDLE_SIZE.x / 2 , height-PADDLE_SIZE.y);
    ball->pos=glm::vec2(width/2 - BALL_RADIUS, height - PADDLE_SIZE.y  - BALL_RADIUS * 2.0f);
    ball->vel = BALL_INIT_VEL;
    ball->stuckToTheBoard = true;
}

bool Game::existsAnotherWithType(Powerup::TYPES type){
    for (Powerup& powup : this->powerups)
        if (powup.activated && powup.type == type)
                return true;
    return false;
}

void Game::updatePowerups(float dt){
    for (Powerup& powup : this->powerups){
        powup.pos += powup.vel * dt;
        powup.duration -= dt;
        if (powup.duration <= 0.0f){
            powup.activated = false;
            //this is horrible, to do: come up with a better sol
            switch (powup.type){
                case Powerup::STICKY:
                    if (!existsAnotherWithType(Powerup::STICKY))
                        ball->sticky = false;
                        paddle->color = glm::vec3(1.0f);
                    break;
                
                case Powerup::PASS_THROUGH:
                    if (!existsAnotherWithType(Powerup::PASS_THROUGH))
                        ball->passThrough = false;
                        paddle->color = glm::vec3(1.0f);
                    break;
                
                case Powerup::CONFUSE:
                    if (!existsAnotherWithType(Powerup::CONFUSE))
                        postProc->confuse = false;
                    break;

                case Powerup::CHAOS:
                    if (!existsAnotherWithType(Powerup::CHAOS))
                        postProc->chaos = false;
                    break;
            }
        }
    }


    //remove_if moves elements to be removed at the end, to actually remove them i need to use erase

    this->powerups.erase(
    std::remove_if(this->powerups.begin(), this->powerups.end(), 
                   [] (const Powerup& p) {return (p.destroyed && !p.activated); } ),
    this->powerups.end()
    );
}

void Game::Update(float dt){
    ball->move(dt, this->width);
    doCollisions();
    pGenerator->update(dt, ball, 2);

    if (ball->pos.y > this->height){
        this->lives--;
        if (this->lives == 0){
            this->resetLevel();
            this->state = GAME_MENU;
        }
        this->resetPlayer();
    }

    if (shakeTime > 0.0f)
        shakeTime -= dt;
    else
        postProc->shake = false;

    this->updatePowerups(dt);

    audioMgr->update();

    if (this->state == GAME_ACTIVE && this->levels[this->level].isFinished()){
        this->resetLevel();
        this->resetPlayer();
        postProc->chaos = true;
        this->state = GAME_WIN;
    }
}

void activePowerup(Powerup& powup){
    switch (powup.type){
        case Powerup::SPEED:
            ball->vel *= 1.2f;
            break;
        case Powerup::STICKY:
            ball->sticky = true;
            paddle->color = glm::vec3(1.0f, 0.5f, 1.0f);
            break;
        case Powerup::PASS_THROUGH:
            ball->passThrough = true;
            paddle->color = glm::vec3(1.0f, 0.5f, 0.0f);
            break;
        case Powerup::PAD_SZ_INCR:
            paddle->size.x += 50;
            break;
        case Powerup::CONFUSE:
            postProc->confuse = true;
            break;
        case Powerup::CHAOS:
            postProc->chaos = true;
            break;
    }
}

Game::~Game(){
    delete Renderer;
    delete paddle;
    delete ball;
    delete pGenerator;
    delete postProc;
    delete audioMgr;
    delete textRenderer;
    // alcDestroyContext(context);
    // alcCloseDevice(device);
}