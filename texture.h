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

class Texture
{
private:
    GLuint id;
    int height;
    int width;
    unsigned int type;

public:
    Texture(const char* fileName, GLenum type) 
    {
        this->type = type;

        // TEXTURE 0
        // Init texture
        unsigned char* image = SOIL_load_image(fileName, &this->width, &this->height, NULL, SOIL_LOAD_RGBA); // load in image

        glGenTextures(1, &this->id); //texture 0 links to index 1 
        glBindTexture(GL_TEXTURE_2D, this->id);

        glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT); // S = x, T = y (IF we run out of texture along X, repeat)
        glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT); // S = x, T = y (IF we run out of texture along Y, repeat)
        glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); // How do we wnat texture to increase in size as we zoom in (linear)
        glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // what happens as we zoom out 

        if (image) // if there is an image in here 
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image); // created an open gl texture 
            glGenerateMipmap(GL_TEXTURE_2D); // takes your image and stores it in several different resolutions for zooming in and out
        }
        else 
        {
            std::cout << "ERROR::TEXTURE::TEXTURE_LOADING_FAILED: " << fileName << "\n";
        }
        
        glActiveTexture(0); // there is no active texture unit
        glBindTexture(GL_TEXTURE_2D, 0); // clear the bound textures 
        SOIL_free_image_data(image); // removes image data
    }

    ~Texture()
    {
        glDeleteTextures(1, &this->id);
    }

    GLuint getID() const
    {
        return this->id;
    }


    void bind(const GLint texture_unit)
    {
        glActiveTexture(GL_TEXTURE0 + texture_unit);
        glBindTexture(this->type, this->id);
    }

    void unbind()
    {
        glActiveTexture(0);
        glBindTexture(this->type, 0);
    }

};