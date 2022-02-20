#version 430

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec3 vertex_coordinate;
layout(location = 3) in vec3 vertex_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float time;

// Output
// TODO(student): Output values to fragment shader
out vec3 frag_color;
out vec3 normals;
out float time_out;

void main()
{
    // TODO(student): Send output to fragment shader
    frag_color = vertex_color;
    
    // bonus
    vec3 move = vertex_position;
    move.x += 4 * cos(2 * time);
    move.y += 4 * sin(2 * time);

    // TODO(student): Compute gl_Position
    gl_Position = Projection * View * Model * vec4(move, 1.0);
  

    normals = vertex_normal;
    time_out = time;
}
