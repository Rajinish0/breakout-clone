#include "shader.h"
#include "camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "game.h"
#include "window.h"
#include "resourcemanager.h"
#include "spriterenderer.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;



float vertices[] = {
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f
};

unsigned int VAO, VBO;


Camera cam;
Game BreakOut(WIDTH, HEIGHT);

int main() {
	Window window(WIDTH, HEIGHT, "Breakout");

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initliaze GLAD" << std::endl;
		return -1;
	}

    // Shader* shader {"shaders/sprite.vert", "shaders/sprite.frag"};

    // SpriteRenderer sp (shader);

    Texture2D text;
    

	glfwSetFramebufferSizeCallback(window.window, framebuffer_size_callback);
	// glfwSetCursorPosCallback(window.window, mouse_callback);
    glfwSetKeyCallback(window.window, key_callback);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    float lastTime = 0.0;
    float curTime;
    float dt = 0.0;

    BreakOut.Init();


	while (!window.shouldClose())
	{
        curTime = glfwGetTime();
        dt = curTime - lastTime;
        lastTime = curTime;
        glfwPollEvents();

        BreakOut.ProcessInput(dt);
        BreakOut.Update(dt);

		glClearColor(0.0f, 0.1f, 7.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

        // sp.drawSprite(text, glm::vec2(0.0f));
        // shader.use();
        // glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 6);
        // glBindVertexArray(0);

        BreakOut.Render();

		window.update();

	}

    ResourceManager::clear();
	glfwTerminate();
	return 0;
}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (key >=0 && key < 1024){
        if (action == GLFW_PRESS)
            BreakOut.keys[key] = true;
        else if (action == GLFW_RELEASE){
            BreakOut.keys[key] = false;
            BreakOut.keyProcessed[key] = false;
        }
    }

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window){}

void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
	cam.handleMouse(xPos, yPos);
}