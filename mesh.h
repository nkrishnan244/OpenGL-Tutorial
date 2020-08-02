#pragma once

#include <iostream>
#include <vector>
#include "texture.h"
#include "vertex.h"
#include "primitives.h"

class Mesh
{
private:
    Vertex* vertexArray;
    unsigned nrOfVertices;
    GLuint* indexArray;
    unsigned nrOfIndices; 

    // std::vector<Vertex> vertices;
    // std::vector<GLuint> indices;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO; 

    glm::vec3 position;
    glm::vec3 origin; 
    glm::vec3 rotation;
    glm::vec3 scale; 
    glm::mat4 ModelMatrix;

    void initVAO()
    {
        // // set variables;
        // this->nrOfVertices = nrOfVertices;
        // this->nrOfIndices = nrOfIndices;

        // VAO, VBO, EBO (Put object and model data onto the GPU)
        // GLuint VAO; // "big box that holds a lot of data"
        glGenVertexArrays(1, &this->VAO); // vertex array object, will hold a lot of data for the triangle
        glBindVertexArray(this->VAO); // anything with any other buffers will bind to this VAO

        // VBO
        // GLuint VBO; // VBO's allow us to store large number of vertices in GPU's memory rather than sending them one at a time
        glGenBuffers(1, &this->VBO); // aloccate memory for one buffer (vertex buffer object)
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO); // bind VBO to attach point GL ARRAY BUFFER
        glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices * sizeof(Vertex), this->vertexArray, GL_STATIC_DRAW); // data we are sending to graphics card (GL static draw means we won't change vertices often)

        // EBO
        // GLuint EBO;
        if (this->nrOfIndices > 0)
        {
            glGenBuffers(1, &this->EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices * sizeof(GLuint), this->indexArray, GL_STATIC_DRAW);
        }

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
    }

    void updateUniforms(Shader* shader)
    {
        shader->setMat4fv(this->ModelMatrix, "ModelMatrix");
    }

    void updateModelMatrix()
    {
        this->ModelMatrix = glm::mat4(1.f); // makes identity matrix
        this->ModelMatrix = glm::translate(this->ModelMatrix, this->origin); // puts in a translation of 0, 0, 0
        this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f)); // second input is the angle, third output is axis to rotate arond
        this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f)); // second input is the angle, third output is axis to rotate arond
        this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f)); // second input is the angle, third output is axis to rotate arond
        this->ModelMatrix = glm::translate(this->ModelMatrix, this->position - this->origin); // puts in a translation of 0, 0, 0
        this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);
    }


public:

    Mesh(Primitive* primitive, 
        glm::vec3 position = glm::vec3(0.f),
        glm::vec3 origin = glm::vec3(0.f),
        glm::vec3 rotation = glm::vec3(0.f),
        glm::vec3 scale = glm::vec3(1.f))
    {
        this->position = position;
        this->origin = origin;
        this->rotation = rotation;
        this->scale = scale;

        this->nrOfVertices = primitive->getNrOfVertices();
        this->nrOfIndices = primitive->getNrOfIndices();

        this->vertexArray = new Vertex[this->nrOfVertices];
        for (size_t i = 0; i < nrOfVertices; i++)
        {
            this->vertexArray[i] = primitive->getVertices()[i];
        }

        this->indexArray = new GLuint[this->nrOfIndices];
        for (size_t i = 0; i < nrOfIndices; i++)
        {
            this->indexArray[i] = primitive->getIndices()[i];
        }

        this->initVAO();
        this->updateModelMatrix();
    }

    Mesh(Vertex* vertexArray, 
        const unsigned& nrOfVertices,
        GLuint* indexArray,
        const unsigned& nrOfIndices, 
        glm::vec3 position = glm::vec3(0.f),
        glm::vec3 origin = glm::vec3(0.f),
        glm::vec3 rotation = glm::vec3(0.f),
        glm::vec3 scale = glm::vec3(1.f))
    {
        this->position = position;
        this->origin = origin;
        this->rotation = rotation;
        this->scale = scale;

        this->nrOfVertices = nrOfVertices;
        this->nrOfIndices = nrOfIndices;

        this->vertexArray = new Vertex[this->nrOfVertices];
        for (size_t i = 0; i < nrOfVertices; i++)
        {
            this->vertexArray[i] = vertexArray[i];
        }

        this->indexArray = new GLuint[this->nrOfIndices];
        for (size_t i = 0; i < nrOfIndices; i++)
        {
            this->indexArray[i] = indexArray[i];
        }

        this->initVAO();
        this->updateModelMatrix();
    }

    Mesh(const Mesh& obj)
    {
        this->position = obj.position;
        this->origin = obj.origin;
        this->rotation = obj.rotation;
        this->scale = obj.scale;

        this->nrOfVertices = obj.nrOfVertices;
        this->nrOfIndices = obj.nrOfIndices;

        this->vertexArray = new Vertex[this->nrOfVertices];
        for (size_t i = 0; i < nrOfVertices; i++)
        {
            this->vertexArray[i] = obj.vertexArray[i];
        }

        this->indexArray = new GLuint[this->nrOfIndices];
        for (size_t i = 0; i < nrOfIndices; i++)
        {
            this->indexArray[i] = obj.indexArray[i];
        }

        this->initVAO();
        this->updateModelMatrix();
    }

    ~Mesh()
    {
        glDeleteVertexArrays(1, &this->VAO);
        glDeleteBuffers(1, &this->VBO);
        if (this->nrOfIndices > 0)
        {
            glDeleteBuffers(1, &this->EBO);
        }

        delete[] this->vertexArray;
        delete[] this->indexArray;
    }

    // Accessors
    void setPosition(const glm::vec3 position)
    {
        this->position = position; 
    }

    void setOrigin(const glm::vec3 origin)
    {
        this->origin = origin;
    }

    void setRotation(const glm::vec3 rotation)
    {
        this->rotation = rotation; 
    }

    void setScale(const glm::vec3 scale)
    {
        this->scale = scale; 
    }

    void move(const glm::vec3 position)
    {
        this->position += position;
    }

    void rotate(const glm::vec3 rotation)
    {
        this->rotation += rotation;
    }

    void scaleUp(const glm::vec3 scale)
    {
        this->scale += scale;
    }


    void update();

    void render(Shader* shader)
    {
        // update uniforms
        this->updateModelMatrix();
        this->updateUniforms(shader);

        shader->use(); // when you update uniforms, you unbind the shader so need to do it after

        // bind vertex array object (contains links to all other object data in the GPU) 
        glBindVertexArray(this->VAO);

        // RENDER IT
        if (this->nrOfIndices == 0)
        {
            glDrawArrays(GL_TRIANGLES, 0, this->nrOfVertices); // doesn't use indices
        }
        else
        {
            glDrawElements(GL_TRIANGLES, this->nrOfIndices, GL_UNSIGNED_INT, 0); // draw all triangles (using indices)
        }

        // Cleanup
        glBindVertexArray(0); // unbind
        glUseProgram(0); // unbind ( will never be less than 1 )
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

};