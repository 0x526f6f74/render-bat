/**
 * Note from the author:
 * The texture_index attribute of a vertex must be a floating-point number due to the limits of the OpenGL shading language, also known as GLSL.
 * The index must also never be an integer, since it may end up below the desired value and be rounded down to the closest integer,
 * upon attempting to convert it to an integer in the cubemap rendering fragment shader (also known as pixel shader) of the Render Bat™ project.
 * The reason for this is so-called floating-point inprecision, a phenomenon in Computer Science, caused by the IEEE 754 standard for floating-point numbers.
 * I will not go into further detail regarding this subject, one may further research the topic in the case of interest being present.
 **/

#pragma once

#include <glm/glm.hpp>

namespace rb
{

struct Vertex
{
    glm::vec3 position;
    glm::vec3 im_coords;
    float texture_index;
};

}  // namespace rb
