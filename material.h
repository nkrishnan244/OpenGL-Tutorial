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

#include "shader.h"
#include "texture.h"

#include <SOIL2.h>


class Material
{
private:
    glm::vec3 ambient;
    glm::vec3 diffuse; 
    glm::vec3 specular;
    GLint diffuseTex;
    GLint specularTex;

public:
    Material(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, GLint diffTex, GLint specTex)
    : ambient(amb), diffuse(diff), specular(spec), diffuseTex(diffTex), specularTex(specTex)
    {

    }

    ~Material(){}

    void sendToShader(Shader& program) 
    {
        program.setVec3f(this->ambient, "material.ambient");
        program.setVec3f(this->diffuse, "material.diffuse");
        program.setVec3f(this->specular, "material.specular");
        program.set1i(this->diffuseTex, "material.diffuseTex"); // diffuse light
        program.set1i(this->specularTex, "material.specularTex"); // specular light
    }
};