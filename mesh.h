#pragma once

#include <iostream>
#include <vector>
#include "texture.h"
#include "vertex.h"
#include "primitives.h"

class Mesh
{
private:
    unsigned nrOfVertices;
    unsigned nrOfIndices; 

    // std::vector<Vertex> vertices;
    // std::vector<GLuint> indices;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO; 

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale; 
    glm::mat4 ModelMatrix;

    void initVAO(Primitive* primitive)
    {
        // set variables;
        this->nrOfVertices = primitive->getNrOfVertices();
        this->nrOfIndices = primitive->getNrOfIndices();

        // VAO, VBO, EBO (Put object and model data onto the GPU)
        // GLuint VAO; // "big box that holds a lot of data"
        glGenVertexArrays(1, &this->VAO); // vertex array object, will hold a lot of data for the triangle
        glBindVertexArray(this->VAO); // anything with any other buffers will bind to this VAO

        // VBO
        // GLuint VBO; // VBO's allow us to store large number of vertices in GPU's memory rather than sending them one at a time
        glGenBuffers(1, &this->VBO); // aloccate memory for one buffer (vertex buffer object)
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO); // bind VBO to attach point GL ARRAY BUFFER
        glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices * sizeof(Vertex), primitive->getVertices(), GL_STATIC_DRAW); // data we are sending to graphics card (GL static draw means we won't change vertices often)

        // EBO
        // GLuint EBO;
        glGenBuffers(1, &this->EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices * sizeof(GLuint), primitive->getIndices(), GL_STATIC_DRAW);

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

    void initVAO(Vertex* vertexArray, 
        const unsigned& nrOfVertices,
        GLuint* indexArray,
        const unsigned& nrOfIndices)
    {
        // set variables;
        this->nrOfVertices = nrOfVertices;
        this->nrOfIndices = nrOfIndices;

        // VAO, VBO, EBO (Put object and model data onto the GPU)
        // GLuint VAO; // "big box that holds a lot of data"
        glGenVertexArrays(1, &this->VAO); // vertex array object, will hold a lot of data for the triangle
        glBindVertexArray(this->VAO); // anything with any other buffers will bind to this VAO

        // VBO
        // GLuint VBO; // VBO's allow us to store large number of vertices in GPU's memory rather than sending them one at a time
        glGenBuffers(1, &this->VBO); // aloccate memory for one buffer (vertex buffer object)
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO); // bind VBO to attach point GL ARRAY BUFFER
        glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices * sizeof(Vertex), vertexArray, GL_STATIC_DRAW); // data we are sending to graphics card (GL static draw means we won't change vertices often)

        // EBO
        // GLuint EBO;
        glGenBuffers(1, &this->EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices * sizeof(GLuint), indexArray, GL_STATIC_DRAW);

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
        this->ModelMatrix = glm::translate(this->ModelMatrix, position); // puts in a translation of 0, 0, 0
        this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f)); // second input is the angle, third output is axis to rotate arond
        this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f)); // second input is the angle, third output is axis to rotate arond
        this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f)); // second input is the angle, third output is axis to rotate arond
        this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);
    }


public:

    Mesh(Primitive* primitive, 
        glm::vec3 position = glm::vec3(0.f),
        glm::vec3 rotation = glm::vec3(0.f),
        glm::vec3 scale = glm::vec3(1.f))
    {
        this->position = position;
        this->rotation = rotation;
        this->scale = scale;

        this->initVAO(primitive);
        this->updateModelMatrix();
    }

    Mesh(Vertex* vertexArray, 
        const unsigned& nrOfVertices,
        GLuint* indexArray,
        const unsigned& nrOfIndices, 
        glm::vec3 position = glm::vec3(0.f),
        glm::vec3 rotation = glm::vec3(0.f),
        glm::vec3 scale = glm::vec3(1.f))
    {
        this->position = position;
        this->rotation = rotation;
        this->scale = scale;

        this->initVAO(vertexArray, nrOfVertices, indexArray, nrOfIndices);
        this->updateModelMatrix();
    }

    ~Mesh()
    {
        glDeleteVertexArrays(1, &this->VAO);
        glDeleteBuffers(1, &this->VBO);
        glDeleteBuffers(1, &this->EBO);
    }

    // Accessors
    void setPosition(const glm::vec3 position)
    {
        this->position = position; 
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
    }

};