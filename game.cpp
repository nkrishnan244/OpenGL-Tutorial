#include "game.h"

// Private functions
void Game::initGLFW()
{
    // Iniit GLFW
    if (glfwInit() == GLFW_FALSE)
    {
        std::cout << "ERROR!::GLFW_INIT_FAILED" << "\n";
        glfwTerminate();
    }
}

void Game::initWindow(const char* title, bool resizable)

{
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->GL_VERSION_MAJOR); // this is the version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->GL_VERSION_MINOR); // this is the version (4.4)
    glfwWindowHint(GLFW_RESIZABLE, resizable); // window can be resized <- frame buffer will change as you resize 

    this->window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, title, NULL, NULL); // 4th param is full screen or window

    if (this->window == nullptr)
    {
        std::cout << "ERROR::GLFW_WINDOW_INIT_FAILED" << "\n";
        glfwTerminate();
    }

    // glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback); // as soon as window is resized, call this function!
    glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight); 
    glViewport(0, 0, this->framebufferWidth, this->framebufferHeight); 

    glfwMakeContextCurrent(window); // IMPORTANT!! 

    // INIT GLEW (NEEDS WINDOW AND OPENGL CONTEXT)
    glewExperimental = GL_TRUE; 

    if (glewInit() != GLEW_OK) // initialization failed 
    {
        std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
        glfwTerminate();
    }

    // OPENGL OPTIONS
    glEnable(GL_DEPTH_TEST); // allows us to use z coordinate, zoom away, etc

    glEnable(GL_CULL_FACE); // able to remove unecessary content
    glCullFace(GL_BACK); // remove back of triangle or whatever
    glFrontFace(GL_CCW); // front face is vertices that are going counter clockwise

    glEnable(GL_BLEND); // allow blending of colors
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // ??????? 

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // fill shape with color (other GL_... do other things like fill outline)
}

void Game::initGLEW()
{

    // INIT GLEW (NEEDS WINDOW AND OPENGL CONTEXT)
    glewExperimental = GL_TRUE; 

    if (glewInit() != GLEW_OK) // initialization failed 
    {
        std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
        glfwTerminate();
    }
}

void Game::initOpenGLOptions()
{
    // OPENGL OPTIONS
    glEnable(GL_DEPTH_TEST); // allows us to use z coordinate, zoom away, etc

    glEnable(GL_CULL_FACE); // able to remove unecessary content
    glCullFace(GL_BACK); // remove back of triangle or whatever
    glFrontFace(GL_CCW); // front face is vertices that are going counter clockwise

    glEnable(GL_BLEND); // allow blending of colors
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // ??????? 

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // fill shape with color (other GL_... do other things like fill outline)
}

// Constructors/Destructors
Game::Game(const char* title, const int WINDOW_WIDTH, const int WINDOW_HEIGHT, 
    const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR, bool resizable)
    : 
    WINDOW_WIDTH(WINDOW_WIDTH), 
    WINDOW_HEIGHT(WINDOW_HEIGHT), 
    GL_VERSION_MAJOR(GL_VERSION_MAJOR), 
    GL_VERSION_MINOR(GL_VERSION_MINOR)
{
    this->window = nullptr;
    this->framebufferWidth = WINDOW_WIDTH;
    this->framebufferHeight = WINDOW_HEIGHT;

    this->initGLFW();
    this->initWindow(title, resizable);
    this->initGLEW();
    this->initOpenGLOptions();
}

Game::~Game()
{
    glfwDestroyWindow(this->window);
    glfwTerminate();
}

// Accessor
int Game::getWindowShouldClose()
{
    return glfwWindowShouldClose(this->window);
}

// Modifier
void Game::setWindowShouldClose()
{
    glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}

// Functions
void Game::update()
{

}

void Game::render()
{

}

// Static functions
void Game::framebuffer_resize_callback(GLFWwindow* window, int framebufferWidth, int framebufferHeight) 
{
    glViewport(0, 0, framebufferWidth, framebufferHeight);
}
