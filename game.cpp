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

    glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight); 
    glfwSetFramebufferSizeCallback(window, Game::framebuffer_resize_callback); // as soon as window is resized, call this function!
    // glViewport(0, 0, this->framebufferWidth, this->framebufferHeight); 

    glfwMakeContextCurrent(window); // IMPORTANT!! 
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

    // Input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // puts cursor inside window
}

void Game::initMatrices()
{
    this->ViewMatrix = glm::mat4(1.f);
    this->ViewMatrix = glm::lookAt(this->camPosition, this->camPosition + this->camFront, this->worldUp);

    this->ProjectionMatrix = glm::mat4(1.f);
    this->ProjectionMatrix = glm::perspective(
        glm::radians(this->fov), 
        static_cast<float>(this->framebufferWidth) / this->framebufferHeight,
        this->nearPlane,
        this->farPlane
    );

}

void Game::initShaders()
{

    this->shaders.push_back(new Shader(this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR, "vertex_core.glsl", "fragment_core.glsl"));

}

void Game::initTextures()
{
    // TEXTURE 0
    // Init texture
    this->textures.push_back(new Texture("Images/pusheen.png", GL_TEXTURE_2D));
    this->textures.push_back(new Texture("Images/pusheen_specular.png", GL_TEXTURE_2D));

    this->textures.push_back(new Texture("Images/container.png", GL_TEXTURE_2D));
    this->textures.push_back(new Texture("Images/container_specular.png", GL_TEXTURE_2D));
}

void Game::initMaterials()
{
    // Material 0
    this->materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), 0, 1));
}

void Game::initUniforms()
{
    this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ViewMatrix, "ViewMatrix");
    this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");

    this->shaders[SHADER_CORE_PROGRAM]->setVec3f(*this->lights[0], "lightPos0");
    // this->shaders[SHADER_CORE_PROGRAM]->setVec3f(this->camPosition, "cameraPos");
}


void Game::initModels()
{
    std::vector<Mesh*> meshes; 
    meshes.push_back(
        new Mesh(
            new Pyramid(),
            glm::vec3(0.f),
            glm::vec3(0.f),
            glm::vec3(0.f),
            glm::vec3(1.f)
        )
    );

     this->models.push_back(new Model(
        glm::vec3(0.f, 0.f, 0.f),
        this->materials[0],
        this->textures[TEX_CONTAINER],
        this->textures[TEX_CONTAINER_SPECULAR],
        meshes
    ));

    this->models.push_back(new Model(
        glm::vec3(0.f, 1.f, 1.f),
        this->materials[0],
        this->textures[TEX_PUSHEEN0],
        this->textures[TEX_PUSHEEN_SPECULAR],
        meshes
    ));

    this->models.push_back(new Model(
        glm::vec3(2.f, 0.f, 2.f),
        this->materials[0],
        this->textures[TEX_CONTAINER],
        this->textures[TEX_CONTAINER_SPECULAR],
        meshes
    ));

    for (auto*& i: meshes)
    {
        delete i;
    }
}

void Game::initLights()
{
    this->lights.push_back(new glm::vec3(0.f, 0.f, 1.f));
}

void Game::updateUniforms()
{
    // update view matrix
    this->ViewMatrix = this->camera.getViewMatrix();

    this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ViewMatrix, "ViewMatrix");
    this->shaders[SHADER_CORE_PROGRAM]->setVec3f(this->camera.getPosition(), "cameraPos"); // we will be updating camera positoin now

    glfwGetFramebufferSize(window, &this->framebufferWidth, &this->framebufferHeight);  // aspect ratio is changing

    ProjectionMatrix = glm::perspective(
        glm::radians(this->fov), 
        static_cast<float>(this->framebufferWidth) / this->framebufferHeight,
        this->nearPlane,
        this->farPlane
    );

    // send in uniforms
    // core_program.setMat4fv(ViewMatrix, "ViewMatrix");
    this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(ProjectionMatrix, "ProjectionMatrix");
}

// Constructors/Destructors
Game::Game(const char* title, 
    const int WINDOW_WIDTH, const int WINDOW_HEIGHT, 
    const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR, 
    bool resizable)
    : 
    WINDOW_WIDTH(WINDOW_WIDTH), 
    WINDOW_HEIGHT(WINDOW_HEIGHT), 
    GL_VERSION_MAJOR(GL_VERSION_MAJOR), 
    GL_VERSION_MINOR(GL_VERSION_MINOR),
    camera(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f))
{
    this->window = nullptr;
    this->framebufferWidth = this->WINDOW_WIDTH;
    this->framebufferHeight = this->WINDOW_HEIGHT;

    this->camPosition = glm::vec3(0.f, 0.f, 1.f);
    this->worldUp = glm::vec3(0.f, 1.f, 0.f);
    this->camFront = glm::vec3(0.f, 0.f, -1.f);

    this->fov = 90.f;
    this->nearPlane = 0.1f;
    this->farPlane = 1000.f;

    this->dt = 0.f;
    this->curTime = 0.f;
    this->lastTime = 0.f;

    this->lastMouseX = 0.0;
    this->lastMouseY = 0.0;
    this->mouseX = 0.0;
    this->mouseY = 0.0;
    this->mouseOffsetX = 0.0;
    this->mouseOffsetY = 0.0;
    this->firstMouse = true; 

    this->initGLFW();
    this->initWindow(title, resizable);
    this->initGLEW();
    this->initOpenGLOptions();

    this->initMatrices();
    this->initShaders();
    this->initTextures();
    this->initMaterials();
    this->initModels();
    this->initLights();
    this->initUniforms();

}

Game::~Game()
{
    glfwDestroyWindow(this->window);
    glfwTerminate();

    for (size_t i = 0; i < this->shaders.size(); i++)
    {
        delete this->shaders[i];
    }

    for (size_t i = 0; i < this->textures.size(); i++)
    {
        delete this->textures[i];
    }

    for (size_t i = 0; i < this->materials.size(); i++)
    {
        delete this->materials[i];
    }

    // for (size_t i = 0; i < this->meshes.size(); i++)
    // {
    //     delete this->meshes[i];
    // }

    for (auto*& i: this->models) // need to reference the pointers, otherwise we would be deleting a temporary object
    {
        delete i;
    }

    for (size_t i = 0; i < this->lights.size(); i++)
    {
        delete this->lights[i];
    }
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
void Game::updateDt()
{
    this->curTime = static_cast<float>(glfwGetTime());
    this->dt = this->curTime - this->lastTime;
    this->lastTime = this->curTime;
}

void Game::updateMouseInput()
{
    glfwGetCursorPos(this->window, &this->mouseX, &this->mouseY);

    if (this->firstMouse)
    {
        this->lastMouseX = this->mouseX;
        this->lastMouseY = this->mouseY;
        this->firstMouse = false; 
    }

    this->mouseOffsetX = this->mouseX - this->lastMouseX;
    this->mouseOffsetY = this->lastMouseY - this->mouseY; // y is inverted

    this->lastMouseX = this->mouseX;
    this->lastMouseY = this->mouseY; 
}

void Game::updateKeyboardInput()
{
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(this->window, GLFW_TRUE);
    }
    if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
    {
        this->camera.move(this->dt, FORWARD);
    }
    if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
    {
        this->camera.move(this->dt, BACKWARD);
    }
    if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
    {
        this->camera.move(this->dt, LEFT);
    }
    if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
    {
        this->camera.move(this->dt, RIGHT);
    }
     if (glfwGetKey(this->window, GLFW_KEY_C) == GLFW_PRESS)
    {
        this->camPosition.y -= 0.005f;
    }
    if (glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        this->camPosition.y += 0.005f;
    }
}

void Game::updateInput()
{
    glfwPollEvents(); // allows cursor to interact with window

    this->updateKeyboardInput();
    this->updateMouseInput();
    this->camera.updateInput(dt, -1, this->mouseOffsetX, this->mouseOffsetY);
}

void Game::update()
{
    // Update input
    // glfwPollEvents(); // allows cursor to interact with window
    this->updateDt();
    this->updateInput();

    this->models[0]->rotate(glm::vec3(0.f, 1.f, 0.f));
    this->models[1]->rotate(glm::vec3(0.f, 1.f, 0.f));
    this->models[2]->rotate(glm::vec3(0.f, 1.f, 0.f));
    // Game::updateInput(this->window);
    // Game::updateInput(this->window, *this->meshes[MESH_QUAD]);
    // this->meshes[0]->rotate(glm::vec3(0.f, 0.001f, 0.f));
}

void Game::render()
{

        // glfwPollEvents(); // allows cursor to interact with window
        // updateInput(window, *this->meshes[0]); // zoom in, out, scale, position

        // update
        // updateInput(window); // right now if we press escape, close 

        // draw

        // clear (clear the previous frame so you can make a new frame)
        glClearColor(0.f, 0.f, 0.f, 1.f); // rgb, alpha (transparency) <-- this is black
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // clear different buffers

        // update the uniforms
        this->updateUniforms();

        // update uniforms (you must bind a program before sending a uniform)
        // this->materials[MAT_1]->sendToShader(*this->shaders[SHADER_CORE_PROGRAM]);

        // use program
        // this->shaders[SHADER_CORE_PROGRAM]->use();

        //Activate texture
        // this->textures[TEX_CONTAINER]->bind(0);
        // this->textures[TEX_CONTAINER_SPECULAR]->bind(1); // we can dynamically choose where to bind

        // draw
        // glDrawArrays(GL_TRIANGLES, 0, nrOfVertices); // doesn't use indices
        // Render Models
        for (auto& i: this->models)
            i->render(this->shaders[SHADER_CORE_PROGRAM]);

        // end draw 
        glfwSwapBuffers(window); // back buffer is being drawn to while front buffer is being shown, this brings the back one to the front
        glFlush(); // ??

        glBindVertexArray(0); // unbind
        glUseProgram(0); // unbind ( will never be less than 1 )
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D, 0);
}

// Static functions
void Game::framebuffer_resize_callback(GLFWwindow* window, int framebufferWidth, int framebufferHeight) 
{
    glViewport(0, 0, framebufferWidth, framebufferHeight);
}

