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

// Using tutorial at https://www.youtube.com/watch?v=iYZA1k8IKgM&list=PL6xSOsbVA1eYSZTKBxnoXYboy7wc4yg-Z&index=9

struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texcoord; 
};

// Position, Color, Texture
Vertex vertices[] =
{
    glm::vec3(-0.5f, 0.5f, 0.f),      glm::vec3(1.f, 0.f, 0.f),     glm::vec2(0.f, 1.f),
    glm::vec3(-0.5f, -0.5f, 0.f),    glm::vec3(0.f, 1.f, 0.f),     glm::vec2(0.f, 0.f),
    glm::vec3(0.5f, -0.5f, 0.f),     glm::vec3(0.f, 0.f, 1.f),     glm::vec2(1.f, 0.f),

    glm::vec3(0.5f, 0.5f, 0.f),    glm::vec3(1.f, 1.f, 0.f),     glm::vec2(1.f, 1.f)
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
void framebuffer_resize_callback(GLFWwindow* window, int framebufferWidth, int framebufferHeight) 
{
    glViewport(0, 0, framebufferWidth, framebufferHeight);
}

// need to load and activate shaders for them to be used
bool loadShaders(GLuint &program)
{
    bool loadSuccess = true; 
    char infoLog[512]; // information if program can't link or shader can't compile
    GLint success; 

    std::string temp = "";
    std::string src = ""; 

    std::ifstream in_file; 

    // load vertex shader 
    in_file.open("../vertex_core.glsl");

    if (in_file.is_open())
    {
        while (std::getline(in_file, temp)) // read next line into temp 
        {
            src += temp + "\n"; // add next line to our string src
        }
    }
    else 
    {
        std::cout << "ERROR::LOADSHADERS::COULD_NOT_OPEN_VERTEX_FILE" << "\n";
        loadSuccess = false; 
    }

    in_file.close();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // let's actually create the shader 
    const GLchar* vertSrc = src.c_str(); // string -> c type string
    glShaderSource(vertexShader, 1, &vertSrc, NULL); // set the source to the shader
    glCompileShader(vertexShader); // actually compile the shader 

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // Error check! (-1 or 1)
    if (!success) 
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog); // see what happened
        std::cout << "ERROR::LOADSHADERS::COULD_NOT_COMPILE_VERTEX_SHADER" << "\n";
        std::cout << infoLog << "\n";
        loadSuccess = false; 
    }

    temp = ""; // will reuse these for fragment shader 
    src = "";

    in_file.open("../fragment_core.glsl");

    if (in_file.is_open()) 
    {
        while (std::getline(in_file, temp))
            src += temp + "\n";     
    }
    else 
    {
        std::cout << "ERROR::LOADSHADERS::COULD_NOT_OPEN_FRAGMENT_FILE" << "\n";
        loadSuccess = false; 
    }

    in_file.close();

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* fragSrc = src.c_str();
    glShaderSource(fragmentShader, 1, &fragSrc, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::LOADSHADERS::COULD_NOT_COMPILE_FRAGMENT_SHADER" << "\n";
        std::cout << infoLog << "\n";
        loadSuccess = false; 
    }

    // Program (from outside the function)
    program = glCreateProgram(); 
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program); 

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) 
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << infoLog;
        std::cout << "ERROR::LOADSHADERS::COULD_NOT_LINK_PROGRAM" << "\n";
        loadSuccess = false; 

    }

    // delete shader 
    glUseProgram(0); // reset program that we are using
    glDeleteShader(vertexShader); // free up memory
    glDeleteShader(fragmentShader);

    return loadSuccess; 
}

int main() 
{

    // CREATE WINDOW
    const int WINDOW_WIDTH = 640;
    const int WINDOW_HEIGHT = 480; 
    int framebufferWidth = 0;
    int framebufferHeight = 0;

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // this is the version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5); // this is the version (4.4)
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // window can be resized <- frame buffer will change as you resize 

    // INIT GLFW
    glfwInit(); // initialize glfw library (must be before create window)

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tutorial", NULL, NULL); // 4th param is full screen or window

    // glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback); // as soon as window is resized, call this function!
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight); 
    glViewport(0, 0, framebufferWidth, framebufferHeight); 

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

    // Initialize Shader
    GLuint core_program; 
    if (!loadShaders(core_program))
    {
        std::cout << "ERROR::MAIN.CPP::LOAD_SHADERS_FAILED" << "\n";
        glfwTerminate();
    }

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

    // BIND VAO 0
    glBindVertexArray(0); // compiles above codes

    // TEXTURE 0
    // Init texture
    int image_width = 0;
    int image_height = 0;
    unsigned char* image = SOIL_load_image("../Images/pusheen.png", &image_width, &image_height, NULL, SOIL_LOAD_RGBA); // load in image

    GLuint texture0;
    glGenTextures(1, &texture0); //texture 0 links to index 1 
    glBindTexture(GL_TEXTURE_2D, texture0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // S = x, T = y (IF we run out of texture along X, repeat)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // S = x, T = y (IF we run out of texture along Y, repeat)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); // How do we wnat texture to increase in size as we zoom in (linear)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // what happens as we zoom out 

    if (image) // if there is an image in here 
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image); // created an open gl texture 
        glGenerateMipmap(GL_TEXTURE_2D); // takes your image and stores it in several different resolutions for zooming in and out
    }
    else 
    {
        std::cout << "ERROR::TEXTURE_LOADING_FAILED" << "\n";
    }
    
    glActiveTexture(0); // there is no active texture unit
    glBindTexture(GL_TEXTURE_2D, 0); // clear the bound textures 
    SOIL_free_image_data(image); // removes image data

    // TEXTURE 1
    // Init texture
    int image_width1 = 0;
    int image_height1 = 0;
    unsigned char* image1 = SOIL_load_image("../Images/micky.png", &image_width1, &image_height1, NULL, SOIL_LOAD_RGBA); // load in image

    GLuint texture1;
    glGenTextures(1, &texture1); //texture 0 links to index 1 
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // S = x, T = y (IF we run out of texture along X, repeat)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // S = x, T = y (IF we run out of texture along Y, repeat)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); // How do we wnat texture to increase in size as we zoom in (linear)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // what happens as we zoom out 

    if (image1) // if there is an image in here 
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width1, image_height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1); // created an open gl texture 
        glGenerateMipmap(GL_TEXTURE_2D); // takes your image and stores it in several different resolutions for zooming in and out
    }
    else 
    {
        std::cout << "ERROR::TEXTURE_LOADING_FAILED" << "\n";
    }
    
    glActiveTexture(0); // there is no active texture unit
    glBindTexture(GL_TEXTURE_2D, 0); // clear the bound textures 
    SOIL_free_image_data(image1); // removes image data

    // MAIN LOOP

    while (!glfwWindowShouldClose(window)) // while the window is not being closed
    {
        // Update input
        glfwPollEvents(); // allows cursor to interact with window

        // update
        updateInput(window); // right now if we press escape, close 

        // draw

        // clear (clear the previous frame so you can make a new frame)
        glClearColor(0.f, 0.f, 0.f, 1.f); // rgb, alpha (transparency) <-- this is black
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // clear different buffers

        // use a program
        glUseProgram(core_program);

        // update uniforms (you must bind a program before sending a uniform)
        glUniform1i(glGetUniformLocation(core_program, "texture0"), 0); // send 1 uniform, bind to texture coordinate 0
        glUniform1i(glGetUniformLocation(core_program, "texture1"), 1); // send 1 uniform, bind to texture coordinate 1

        //Activate texture
        glActiveTexture(GL_TEXTURE0); // active the first texture unit
        glBindTexture(GL_TEXTURE_2D, texture0);
        glActiveTexture(GL_TEXTURE1); // active the second texture unit
        glBindTexture(GL_TEXTURE_2D, texture1);

        // bind vertex array object (contains links to all other object data in the GPU) 
        glBindVertexArray(VAO);

        // draw
        // glDrawArrays(GL_TRIANGLES, 0, nrOfVertices); // doesn't use indices
        glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0); // draw all triangles (using indices)

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
    glDeleteProgram(core_program);

    return 0;
}