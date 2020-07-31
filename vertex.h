#pragma once

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texcoord; 
    glm::vec3 normal; // perpendicular vector to the object
    // we use the dot product between the normal vector and the light vector to get the cos of the angle
    // we then use that angle to determine the percentage of the light that is seen by that vertex
    // multiply that number by the pixel value there
};