#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Game
{
private:
// Variables
    // Window
    GLFWwindow* window;
    const int WINDOW_WIDTH;
    const int WINDOW_HEIGHT;
    int framebufferWidth;
    int framebufferHeight; 

    //OpenGL context
    const int GL_VERSION_MAJOR;
    const int GL_VERSION_MINOR; 

// Private functions
    void initGLFW();
    void initWindow(const char* title, bool resizable);
    void initGLEW();
    void initOpenGLOptions();

// Static variables

public:

// Constructors/Destructors
    Game(const char* title, const int WINDOW_WIDTH, const int WINDOW_HEIGHT, 
        const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR, bool resizable);

    virtual ~Game();    

// Accessors
    int getWindowShouldClose();

// Modifiers
    void setWindowShouldClose();

// Functions
    void update();
    void render();

//Static functions 
    static void framebuffer_resize_callback(GLFWwindow* window, int framebufferWidth, int framebufferHeight);
};