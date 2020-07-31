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


#include <SOIL2.h>

class Shader
{
private:
    // Member variables
    GLuint id; // program id
    const int versionMajor;
    const int versionMinor;

    // Privatefunctions
    std::string loadShaderSource(char* fileName)
    {
        std::string temp = "";
        std::string src = ""; 

        std::ifstream in_file; 

        // load vertex shader 
        in_file.open(fileName);

        if (in_file.is_open())
        {
            while (std::getline(in_file, temp)) // read next line into temp 
            {
                src += temp + "\n"; // add next line to our string src
            }
        }
        else 
        {
            std::cout << "ERROR::SHADER::COULD_NOT_OPEN_FILE: " << fileName << "\n";
        }

        in_file.close();

        std::string versionNr = std::to_string(this->versionMajor) + std::to_string(this->versionMinor) + "0";

        src.replace(src.find("#version"), 12, ("#version " + versionNr)); // replace in the string
        std::cout << src << "\n";

        return src; // contains the source for the shader
    }

    GLuint loadShader(GLenum type, char* fileName)
    { 
        char infoLog[512]; // information if program can't link or shader can't compile
        GLint success; 

        GLuint shader = glCreateShader(type); // let's actually create the shader 
        std::string str_src = this->loadShaderSource(fileName);

        const GLchar* src = str_src.c_str(); // string -> c type string
        glShaderSource(shader, 1, &src, NULL); // set the source to the shader
        glCompileShader(shader); // actually compile the shader 

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success); // Error check! (-1 or 1)
        if (!success) 
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog); // see what happened
            std::cout << "ERROR::LOADSHADERS::COULD_NOT_COMPILE_SHADER: " << fileName << "\n";
            std::cout << infoLog << "\n";
        }

        return shader;
    }

    void linkProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader)
    {
        GLint success = true; 
        char infoLog[512]; // information if program can't link or shader can't compile

        this->id = glCreateProgram();

        glAttachShader(this->id, vertexShader);

        if(geometryShader)
        {
            glAttachShader(this->id, geometryShader);
        }

        glAttachShader(this->id, fragmentShader);

        glLinkProgram(this->id);

        glGetProgramiv(this->id, GL_LINK_STATUS, &success);
        if (!success) 
        {
            glGetProgramInfoLog(this->id, 512, NULL, infoLog);
            std::cout << infoLog;
            std::cout << "ERROR::SHADER::COULD_NOT_LINK_PROGRAM" << "\n";
        }

        glUseProgram(0);

    }

public:

    // Constructors, Destructors 
    Shader(const int versionMajor, const int versionMinor, char* vertexFile, char* fragmentFile, char* geometryFile = "")
        : versionMajor(versionMajor), versionMinor(versionMinor)
    {
        GLuint vertexShader = 0;
        GLuint geometryShader = 0;
        GLuint fragmentShader = 0;

        vertexShader = loadShader(GL_VERTEX_SHADER, vertexFile);

        if (geometryFile != "")
        {
            geometryShader = loadShader(GL_GEOMETRY_SHADER, geometryFile);
        }

        fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentFile);

        this->linkProgram(vertexShader, geometryShader, fragmentShader);
    
        // delete shader 
        glDeleteShader(vertexShader); // free up memory
        glDeleteShader(geometryShader);
        glDeleteShader(fragmentShader);
    }

    ~Shader()
    {
        glDeleteProgram(this->id);
    }

    // set uniform functions
    void use() 
    {
        glUseProgram(this->id);
    }

    void unuse()
    {
        glUseProgram(0);
    }

    void set1i(GLfloat value, const GLchar* name)
    {
        this->use();

        glUniform1i(glGetUniformLocation(this->id, name), value);

        this->unuse(); // unuse the program after setting it
    }

    void set1f(GLfloat value, const GLchar* name)
    {
        this->use();

        glUniform1f(glGetUniformLocation(this->id, name), value);

        this->unuse(); // unuse the program after setting it
    }

    void setVec2f(glm::fvec2 value, const GLchar* name)
    {
        this->use();

        glUniform2fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value)); 

        this->unuse(); // unuse the program after setting it
    }

    void setVec3f(glm::fvec3 value, const GLchar* name)
    {
        this->use();

        glUniform3fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value)); 

        this->unuse(); // unuse the program after setting it
    }

    void setMat3fv(glm::mat3 value, const GLchar* name, GLboolean transpose = GL_FALSE)
    {
        this->use();

        glUniformMatrix3fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));

        this->unuse(); // unuse the program after setting it
    }

    void setMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose = GL_FALSE)
    {
        this->use();

        glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));

        this->unuse(); // unuse the program after setting it
    }
    
};