#pragma once

#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "material.h"

class Model
{
private:
    Material* material;
    Texture* overrideTextureDiffuse;
    Texture* overrideTextureSpecular;
    std::vector<Mesh*> meshes;
    glm::vec3 position;

    void updateUniforms()
    {

    }

public:
    Model(glm::vec3 position, Material* material, Texture* orTexDif, Texture* orTexSpec, std::vector<Mesh*>& meshes)
    {
        this->position = position;
        this->material = material;
        this->overrideTextureDiffuse = orTexDif;
        this->overrideTextureSpecular = orTexSpec;

        for (auto* i: meshes)
        {
            this->meshes.push_back(new Mesh(*i));
        }

        for (auto& i : this->meshes)
        {
            i->move(this->position);
            i->setOrigin(this->position);
        }

    }

    ~Model()
    {
        for (auto*&  i : this->meshes)
        {
            delete i;
        }

    }

    // Functions
    void rotate(glm::vec3 rotation)
    {
        for (auto& i: this->meshes)
        {
            i->rotate(rotation);
        }
    }

    void update()
    {

    }

    void render(Shader* shader)
    {
        // update the uniforms
        this->updateUniforms();

        // update uniforms (you must bind a program before sending a uniform)
        this->material->sendToShader(*shader);

        // use program
        shader->use();

        //Activate texture
        this->overrideTextureDiffuse->bind(0);
        this->overrideTextureSpecular->bind(1); // we can dynamically choose where to bind

        // draw
        // glDrawArrays(GL_TRIANGLES, 0, nrOfVertices); // doesn't use indices
        for (auto& i : this->meshes)
        {
            i->render(shader);
        }
    }
};