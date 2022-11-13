#type vertex
#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 fill_color;

out vec4 v_fill_color;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(position, 1.0);
    v_fill_color = fill_color;
}

#type fragment
#version 450 core

layout(location = 0) in vec4 v_fill_color;

void main()
{
    gl_FragColor = v_fill_color;
}
