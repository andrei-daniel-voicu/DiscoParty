#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Output
out vec2 texcoord;

uniform bool billboard;

void main()
{
    // TODO(student): Pass v_texture_coord as output to fragment shader
    texcoord = v_texture_coord;
    mat4 modelview = View * Model;
    if(billboard)
    {
        modelview[0][0] = 1;
        modelview[1][0] = 0;
        modelview[2][0] = 0;


        modelview[0][2] = 0;
        modelview[1][2] = 0;
        modelview[2][2] = 1;
    }
    gl_Position = Projection * modelview * vec4(v_position, 1.0);
}
