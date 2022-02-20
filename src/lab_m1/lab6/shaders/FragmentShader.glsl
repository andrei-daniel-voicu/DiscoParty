#version 430

// Input
// TODO(student): Get values from vertex shader
in vec3 frag_color;
in vec3 normals;
in float time_out;

// Output
layout(location = 0) out vec4 out_color;

// bonus
vec2 SineWave( vec2 p ){
    float pi = 3.14159;
    float A = 0.55;
    float w = 100 * pi;
    float t = 30.0*pi/180.0;

    float x = cos( w*p.x + t) * A; 
    float y = sin( w*p.x + t) * A; 
    return vec2(p.x+x, p.y+y);
}

void main()
{
    // TODO(student): Write pixel out color
   out_color = vec4(abs(normals), 1);

   // bonus
   vec2 c = SineWave(vec2(out_color.x, out_color.y));
   out_color = vec4(c.x,c.y,out_color.z,1);
   out_color.x += cos (3*time_out);
   out_color.y += sin (3*time_out);
}
