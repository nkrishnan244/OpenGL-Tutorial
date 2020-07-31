#pragma once

#include <vector>
#include "vertex.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Primitive 
{
private:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

public:
    Primitive() {}
    virtual ~Primitive() {}

    // Functions
    void set(const Vertex* vertices, const unsigned nrOfVertices, const GLuint* indices, const unsigned nrOfIndices)
    {

        for (size_t i = 0; i < nrOfVertices; i++)
        {
            this->vertices.push_back(vertices[i]);
        }

        for (size_t i = 0; i < nrOfIndices; i++)
        {
            this->indices.push_back(indices[i]);
        }

    }

    inline Vertex* getVertices() { return this->vertices.data();}
    inline GLuint* getIndices() { return this->indices.data();}
    inline const unsigned getNrOfVertices() { return this->vertices.size();}
    inline const unsigned getNrOfIndices() { return this->indices.size();}

};

class Quad : public Primitive
{
public:
    Quad()
        : Primitive()
    {
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

        this->set(vertices, nrOfVertices, indices, nrOfIndices);
    }
};

class Triangle : public Primitive
{
public:
    Triangle()
        : Primitive()
    {
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
        };

        int nrOfVertices = sizeof(vertices) / sizeof(Vertex); 
        int nrOfIndices = sizeof(indices) / sizeof(GLuint);

        this->set(vertices, nrOfVertices, indices, nrOfIndices);
    }
};