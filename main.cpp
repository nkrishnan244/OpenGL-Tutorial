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

#include <SOIL2.h>

#include "shader.h"
#include "texture.h"
#include "material.h"

#include "vertex.h"
#include "primitives.h"
#include "mesh.h"

#include "game.h"

// Using tutorial at https://www.youtube.com/watch?v=iYZA1k8IKgM&list=PL6xSOsbVA1eYSZTKBxnoXYboy7wc4yg-Z&index=9

Primitive test();

// Position, Color, Texture
Vertex vertices[] =
{
    glm::vec3(-0.5f, 0.5f, 0.f),      glm::vec3(1.f, 0.f, 0.f),     glm::vec2(0.f, 1.f),  glm::vec3(0.f, 0.f, -1.f), 
    glm::vec3(-0.5f, -0.5f, 0.f),    glm::vec3(0.f, 1.f, 0.f),     glm::vec2(0.f, 0.f),    glm::vec3(0.f, 0.f, -1.f), 
    glm::vec3(0.5f, -0.5f, 0.f),     glm::vec3(0.f, 0.f, 1.f),     glm::vec2(1.f, 0.f),     glm::vec3(0.f, 0.f, -1.f), 
    glm::vec3(0.5f, 0.5f, 0.f),    glm::vec3(1.f, 1.f, 0.f),     glm::vec2(1.f, 1.f),        glm::vec3(0.f, 0.f, -1.f) // -1 z direction means pointing towrads us 
};

// Is used to reuse vertices for defining different triangles
GLuint indices[] =
{
    0, 1, 2, // triangle1 
    0, 2, 3  // triangle2
};

int nrOfVertices = sizeof(vertices) / sizeof(Vertex); 
int nrOfIndices = sizeof(indices) / sizeof(GLuint);

void updateInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

// you need this if you want to resize your windowq
// void framebuffer_resize_callback(GLFWwindow* window, int framebufferWidth, int framebufferHeight) 
// {
//     glViewport(0, 0, framebufferWidth, framebufferHeight);
// }

void updateInput(GLFWwindow* window, Mesh &mesh) 
{
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        mesh.move(glm::vec3(0.f, 0.f, -0.01f));
        // position.z -= 0.0001f; 
    }

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        mesh.move(glm::vec3(0.f, 0.f, 0.01f));
    }

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        mesh.move(glm::vec3(-0.01f, 0.f, 0.f));
    }

    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        mesh.move(glm::vec3(0.01f, 0.f, 0.f));
    }

    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        mesh.move(glm::vec3(0.f, -0.01f, 0.f));
    }

    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        mesh.move(glm::vec3(0.f, 0.01f, 0.f));
    }

    if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        mesh.scaleUp(glm::vec3(0.01f, 0.01f, 0.01f));
    }

    if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
         mesh.scaleUp(glm::vec3(-0.01f, -0.01f, -0.01f));
    }
}

GLFWwindow* createWindow(const char* title, const int width, const int height, 
    int& fbwidth, int& fbHeight, const int GLmajorVer, const int GLminorVer, bool resizable)
{
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLmajorVer); // this is the version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLminorVer); // this is the version (4.4)
    glfwWindowHint(GLFW_RESIZABLE, resizable); // window can be resized <- frame buffer will change as you resize 

    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL); // 4th param is full screen or window

    // glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback); // as soon as window is resized, call this function!
    glfwGetFramebufferSize(window, &fbwidth, &fbHeight); 
    glViewport(0, 0, fbwidth, fbHeight); 

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

    return window;
}

int main() 
{

     // INIT GLFW
    glfwInit(); // initialize glfw library (must be before create window)

    // CREATE WINDOW
    const int GLmajorVersion = 4;
    const int GLminorVersion = 5; 

    const int WINDOW_WIDTH = 640;
    const int WINDOW_HEIGHT = 480; 
    int framebufferWidth = 0;
    int framebufferHeight = 0;
    
    const char* title = "YOUTUBE_TUTORIAL";

    GLFWwindow* window = createWindow(title, WINDOW_WIDTH, WINDOW_HEIGHT, framebufferWidth, framebufferHeight, GLmajorVersion, GLminorVersion, false);

    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // this is the version
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5); // this is the version (4.4)
    // glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); // window can be resized <- frame buffer will change as you resize 

    // // GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tutorial", NULL, NULL); // 4th param is full screen or window

    // // glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback); // as soon as window is resized, call this function!
    // glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight); 
    // glViewport(0, 0, framebufferWidth, framebufferHeight); 

    // glfwMakeContextCurrent(window); // IMPORTANT!! 

    // // INIT GLEW (NEEDS WINDOW AND OPENGL CONTEXT)
    // glewExperimental = GL_TRUE; 

    // if (glewInit() != GLEW_OK) // initialization failed 
    // {
    //     std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
    //     glfwTerminate();
    // }

    // OPENGL OPTIONS
    // glEnable(GL_DEPTH_TEST); // allows us to use z coordinate, zoom away, etc

    // glEnable(GL_CULL_FACE); // able to remove unecessary content
    // glCullFace(GL_BACK); // remove back of triangle or whatever
    // glFrontFace(GL_CCW); // front face is vertices that are going counter clockwise

    // glEnable(GL_BLEND); // allow blending of colors
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // ??????? 

    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // fill shape with color (other GL_... do other things like fill outline)

    // Shader test("vertex_core.glsl", "fragment_core.glsl"); // testing class

    // Initialize Shader
    // GLuint core_program; 
    Shader core_program(GLmajorVersion, GLminorVersion, "vertex_core.glsl", "fragment_core.glsl");
    // if (!loadShaders(core_program))
    // {
    //     std::cout << "ERROR::MAIN.CPP::LOAD_SHADERS_FAILED" << "\n";
    //     glfwTerminate();
    // }

    // MODEL MESH
    Quad tempQuad;
    Mesh test(&tempQuad, glm::vec3(0.f), glm::vec3(0.f), glm::vec3(1.f));

    // VAO, VBO, EBO (Put object and model data onto the GPU)
    GLuint VAO; // "big box that holds a lot of data"
    glGenVertexArrays(1, &VAO); // vertex array object, will hold a lot of data for the triangle
    glBindVertexArray(VAO); // anything with any other buffers will bind to this VAO

    // VBO
    GLuint VBO; // VBO's allow us to store large number of vertices in GPU's memory rather than sending them one at a time
    glGenBuffers(1, &VBO); // aloccate memory for one buffer (vertex buffer object)
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind VBO to attach point GL ARRAY BUFFER
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // data we are sending to graphics card (GL static draw means we won't change vertices often)

    // EBO
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // SET VERTEXATTRIBPOINTERS AND ENABLE (tell gpu how to split up data) (INPUT ASSEMBLY)
    // GLuint attribLoc = glGetAttribLocation(core_program, "vertex_position"); // this is useful if we don't set the location param in glsl file
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position)); // position and size (normalized = false, stride is indices until next vertex) 
    glEnableVertexAttribArray(0); // or attrib Location

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color)); // color
    glEnableVertexAttribArray(1); 

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord)); // texture
    glEnableVertexAttribArray(2); 

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(3);

    // BIND VAO 0
    glBindVertexArray(0); // compiles above codes

    // TEXTURE 0
    // Init texture
    Texture texture0("Images/pusheen.png", GL_TEXTURE_2D, 0);
    Texture texture1("Images/micky.png", GL_TEXTURE_2D, 1);

    // Material 0
    Material material0(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), texture0.getTextureUnit(), texture1.getTextureUnit());

    // internally, the calculations start with the bottom ones (basically right to left)
    glm::vec3 position(0.f);
    glm::vec3 rotation(0.f);
    glm::vec3 scale(1.f); 

    glm::mat4 ModelMatrix(1.f); // makes identity matrix
    ModelMatrix = glm::translate(ModelMatrix, position); // puts in a translation of 0, 0, 0
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f)); // second input is the angle, third output is axis to rotate arond
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f)); // second input is the angle, third output is axis to rotate arond
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f)); // second input is the angle, third output is axis to rotate arond
    ModelMatrix = glm::scale(ModelMatrix, scale);

    glm::vec3 camPosition(0.f, 0.f, 1.f);
    glm::vec3 worldUp(0.f, 1.f, 0.f);
    glm::vec3 camFront(0.f, 0.f, -1.f);


    glm::mat4 ViewMatrix(1.f);
    ViewMatrix = glm::lookAt(camPosition, camPosition + camFront, worldUp);

    float fov = 90.f;
    float nearPlane = 0.1f; // slightly behind our eyes
    float farPlane = 1000.f; 

    glm::mat4 ProjectionMatrix(1.f);
    ProjectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(framebufferWidth) / framebufferHeight,
        nearPlane,
        farPlane
    );


    // LIGHTS
    glm::vec3 lightPos0(0.f, 0.f, 1.f);

    // INIT UNIFORMS
    // core_program.use();
    // glUseProgram(core_program);

    core_program.setMat4fv(ModelMatrix, "ModelMatrix");
    core_program.setMat4fv(ViewMatrix, "ViewMatrix");
    core_program.setMat4fv(ProjectionMatrix, "ProjectionMatrix");

    core_program.setVec3f(lightPos0, "lightPos0");
    core_program.setVec3f(camPosition, "cameraPos");

    // MAIN LOOP

    while (!glfwWindowShouldClose(window)) // while the window is not being closed
    {
        // Update input
        glfwPollEvents(); // allows cursor to interact with window
        updateInput(window, test); // zoom in, out, scale, position

        // update
        updateInput(window); // right now if we press escape, close 

        // draw

        // clear (clear the previous frame so you can make a new frame)
        glClearColor(0.f, 0.f, 0.f, 1.f); // rgb, alpha (transparency) <-- this is black
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // clear different buffers


        // update uniforms (you must bind a program before sending a uniform)
        core_program.set1i(texture0.getTextureUnit(), "texture0");
        core_program.set1i(texture1.getTextureUnit(), "texture1");
        material0.sendToShader(core_program);

        // Move, rotate, scale
        ModelMatrix = glm::mat4(1.f);
        ModelMatrix = glm::translate(ModelMatrix, position); // puts in a translation of 0, 0, 0
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f)); // second input is the angle, third output is axis to rotate arond
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f)); // second input is the angle, third output is axis to rotate arond
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f)); // second input is the angle, third output is axis to rotate arond
        ModelMatrix = glm::scale(ModelMatrix, scale);

        core_program.setMat4fv(ModelMatrix, "ModelMatrix");

        glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);  // aspect ratio is changing

        ProjectionMatrix = glm::perspective(
            glm::radians(fov), 
            static_cast<float>(framebufferWidth) / framebufferHeight,
            nearPlane,
            farPlane
        );

        // send in uniforms
        // core_program.setMat4fv(ViewMatrix, "ViewMatrix");
        core_program.setMat4fv(ProjectionMatrix, "ProjectionMatrix");

        // use program
        core_program.use();

        //Activate texture
        texture0.bind();
        texture1.bind();

        // bind vertex array object (contains links to all other object data in the GPU) 
        glBindVertexArray(VAO);

        // draw
        // glDrawArrays(GL_TRIANGLES, 0, nrOfVertices); // doesn't use indices
        glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0); // draw all triangles (using indices)
        test.render(&core_program);

        // end draw 
        glfwSwapBuffers(window); // back buffer is being drawn to while front buffer is being shown, this brings the back one to the front
        glFlush(); // ??

        glBindVertexArray(0); // unbind
        glUseProgram(0); // unbind ( will never be less than 1 )
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D, 0);

    }

    // END OF PROGRAM
    glfwDestroyWindow(window);
    glfwTerminate(); // free up memory 

    // Delete program 
    // glDeleteProgram(core_program);

    return 0;
}