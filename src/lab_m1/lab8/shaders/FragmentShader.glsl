#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform vec3 material_kd;
uniform vec3 material_ks;
uniform int material_shininess;

// TODO(student): Declare any other uniforms
uniform vec3 material_ka;
uniform vec3 material_ke;
uniform vec3 globalAmbientalIntensity;
uniform vec3 sourceIntensity;
uniform vec3 lightColor;

uniform int lightType;
uniform float cutOffAngle;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    vec3 L = normalize(light_position - world_position);
    vec3 V = normalize(eye_position - world_position);
    vec3 H = normalize(L + V);
    vec3 R = reflect (-L, world_normal);

    // TODO(student): Define ambient, diffuse and specular light components
    vec3 ambient_light = globalAmbientalIntensity * material_ka;
    vec3 emissive_light = material_ke;
    vec3 diffuse_light = material_kd * sourceIntensity * max (dot(world_normal, L), 0);
    vec3 specular_light = vec3(0.f, 0.f, 0.f);

    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!
    if (diffuse_light.x > 0 || diffuse_light.y > 0 || diffuse_light.z > 0)
    {
         specular_light = material_ks * sourceIntensity * pow(max(dot(world_normal, H), 0), material_shininess);
    }

    // TODO(student): If (and only if) the light is a spotlight, we need to do
    // some additional things.
    vec3 light = vec3(0.f, 0.f, 0.f);
    float light_att_factor = 0.f;

    if(lightType == 1)
    {
        float cut_off = radians(cutOffAngle);
        float spot_light = dot(-L, light_direction);
        float spot_light_limit = cos(cut_off);
 
        if (spot_light > spot_light_limit)
        {
            float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
            light_att_factor = pow(linear_att, 2);
        }
    }
    else
    {
         float dist = distance(light_position, world_position);
         light_att_factor = 1.f / (dist*dist);
    }
    // TODO(student): Compute the total light. You can just add the components
    // together, but if you're feeling extra fancy, you can add individual
    // colors to the light components. To do that, pick some vec3 colors that
    // you like, and multiply them with the respective light components.
    light = emissive_light + lightColor * (ambient_light + light_att_factor * ( diffuse_light + specular_light));

    // TODO(student): Write pixel out color
    vec3 color = light;
    out_color = vec4(color, 1);

}
