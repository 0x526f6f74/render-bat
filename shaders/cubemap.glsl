#type vertex
#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 im_coords;
layout(location = 2) in float texture_index;

layout(location = 0) out vec3 v_im_coords;
layout(location = 1) out float v_texture_index;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(position, 1.0);
    v_im_coords = im_coords;
    v_texture_index = texture_index;
}

#type fragment
#version 450 core

layout(location = 0) in vec3 v_im_coords;
layout(location = 1) in float v_texture_index;

layout(location = 0) out vec4 fragment_color;

uniform samplerCube cubemaps[32];

void main()
{
    switch (int(v_texture_index))
    {
        case 0: fragment_color = texture(cubemaps[0], v_im_coords); break;
        case 1: fragment_color = texture(cubemaps[1], v_im_coords); break;
        case 2: fragment_color = texture(cubemaps[2], v_im_coords); break;
        case 3: fragment_color = texture(cubemaps[3], v_im_coords); break;
        case 4: fragment_color = texture(cubemaps[4], v_im_coords); break;
        case 5: fragment_color = texture(cubemaps[5], v_im_coords); break;
        case 6: fragment_color = texture(cubemaps[6], v_im_coords); break;
        case 7: fragment_color = texture(cubemaps[7], v_im_coords); break;
        case 8: fragment_color = texture(cubemaps[8], v_im_coords); break;
        case 9: fragment_color = texture(cubemaps[9], v_im_coords); break;
        case 10: fragment_color = texture(cubemaps[10], v_im_coords); break;
        case 11: fragment_color = texture(cubemaps[11], v_im_coords); break;
        case 12: fragment_color = texture(cubemaps[12], v_im_coords); break;
        case 13: fragment_color = texture(cubemaps[13], v_im_coords); break;
        case 14: fragment_color = texture(cubemaps[14], v_im_coords); break;
        case 15: fragment_color = texture(cubemaps[15], v_im_coords); break;
        case 16: fragment_color = texture(cubemaps[16], v_im_coords); break;
        case 17: fragment_color = texture(cubemaps[17], v_im_coords); break;
        case 18: fragment_color = texture(cubemaps[18], v_im_coords); break;
        case 19: fragment_color = texture(cubemaps[19], v_im_coords); break;
        case 20: fragment_color = texture(cubemaps[20], v_im_coords); break;
        case 21: fragment_color = texture(cubemaps[21], v_im_coords); break;
        case 22: fragment_color = texture(cubemaps[22], v_im_coords); break;
        case 23: fragment_color = texture(cubemaps[23], v_im_coords); break;
        case 24: fragment_color = texture(cubemaps[24], v_im_coords); break;
        case 25: fragment_color = texture(cubemaps[25], v_im_coords); break;
        case 26: fragment_color = texture(cubemaps[26], v_im_coords); break;
        case 27: fragment_color = texture(cubemaps[27], v_im_coords); break;
        case 28: fragment_color = texture(cubemaps[28], v_im_coords); break;
        case 29: fragment_color = texture(cubemaps[29], v_im_coords); break;
        case 30: fragment_color = texture(cubemaps[30], v_im_coords); break;
        case 31: fragment_color = texture(cubemaps[31], v_im_coords); break;
    }
}
