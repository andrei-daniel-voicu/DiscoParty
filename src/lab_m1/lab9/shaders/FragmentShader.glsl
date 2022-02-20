#version 330

// Input
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;

// TODO(student): Declare various other uniforms
uniform bool mix_textures;
uniform bool cycle;
uniform float time;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture2D() function.
    vec2 modifiedTexCoord = texcoord;
    if(cycle)
        modifiedTexCoord = vec2(texcoord.x - time/10.f, texcoord.y);

    vec4 color1 = texture2D(texture_1, modifiedTexCoord);
    vec4 color2 = texture2D(texture_2, modifiedTexCoord);
    if(mix_textures)
         out_color  = mix(color1, color2, 0.5f);
    else
        out_color = color1;
   if (out_color.a < 0.5f)
		discard;
}
