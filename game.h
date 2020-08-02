#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>

#include <vector>
#include "shader.h"
#include "mesh.h"
#include "material.h"

#include "camera.h"
#include "model.h"

// Enumerations
enum shader_enum { SHADER_CORE_PROGRAM = 0 };
enum texture_enum { TEX_PUSHEEN0 = 0, TEX_PUSHEEN_SPECULAR, TEX_CONTAINER, TEX_CONTAINER_SPECULAR }; 
enum material_enum { MAT_1 = 0 };
enum mesh_enum { MESH_QUAD = 0 };

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

    // Delta time
    float dt;
    float curTime;
    float lastTime; 

    // Mouse input
    double lastMouseX;
    double lastMouseY;
    double mouseX;
    double mouseY;
    double mouseOffsetX;
    double mouseOffsetY;
    bool firstMouse;

    // Camera
    Camera camera; 

    // Matrices
    glm::mat4 ViewMatrix;
    glm::vec3 camPosition;
    glm::vec3 worldUp;
    glm::vec3 camFront;

    glm::mat4 ProjectionMatrix; 
    float fov;
    float nearPlane; 
    float farPlane;

    // Shaders
    std::vector<Shader*> shaders;

    // Textures
    std::vector<Texture*> textures;

    // Materials
    std::vector<Material*> materials;

    // Meshes
    // std::vector<Mesh*> meshes;

    // Models
    std::vector<Model*> models;

    // Lights
    std::vector<glm::vec3*> lights; // light positio s 

// Private functions
    void initGLFW();
    void initWindow(const char* title, bool resizable);
    void initGLEW();
    void initOpenGLOptions();
    void initMatrices();
    void initShaders();
    void initTextures();
    void initMaterials();
    void initModels();
    void initLights();
    void initUniforms();

    void updateUniforms();

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
    void updateDt();
    void updateMouseInput();
    void updateKeyboardInput();
    void updateInput();
    void update();
    void render();

//Static functions 
    static void framebuffer_resize_callback(GLFWwindow* window, int framebufferWidth, int framebufferHeight);
    // void updateInput(GLFWwindow* window);
    // void updateInput(GLFWwindow* window, Mesh &mesh);
};