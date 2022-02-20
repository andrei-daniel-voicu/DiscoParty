#version 330

// Input
in vec3 world_position;
in vec3 world_normal;
in vec2 texcoord;

uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform float t;

uniform vec3 lightPos[14];
uniform vec3 lightColor[14];
uniform int lightType[14];
uniform vec3 lightDir[14];
uniform float lightCutOff[14];
uniform vec3 lightIntensity[14];

uniform int isReflector;
uniform int isGlobe;

uniform vec3 globe_position;

uniform vec3 eye_position;

uniform vec3 material_kd;
uniform vec3 material_ks;
uniform int material_shininess;
uniform vec3 material_ka;
uniform vec3 material_ke;

uniform float time;


// Output
layout(location = 0) out vec4 out_color;

vec3 PointLightContribution(vec3 lightPos, vec3 lightColor, int lightType, vec3 lightDir, float lightCutOff, vec3 lightIntensity)
{
    vec3 L = normalize(lightPos - world_position);
    vec3 V = normalize(eye_position - world_position);
    vec3 H = normalize(L + V);

    vec3 color = vec3(0,0,0);
    vec3 diffuse_light = material_kd * lightIntensity * max (dot(world_normal, L), 0);
    vec3 specular_light = vec3(0.f, 0.f, 0.f);

    if (diffuse_light.x > 0 || diffuse_light.y > 0 || diffuse_light.z > 0)
    {
         specular_light = material_ks * lightIntensity * pow(max(dot(world_normal, H), 0), material_shininess);
    }

    float att = 0.f;
    if(lightType == 1)
    {

        float spot_light = dot(-L, lightDir);
        float spot_light_limit = cos(lightCutOff);
 
        if (spot_light > spot_light_limit)
        {
            float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
            att = pow(linear_att, 2);
        }
    }
    else if(lightType == 0 || lightType == 3)
    {
        float dist = distance(lightPos, world_position);
        float light_radius = 1.1f;
        if(lightType == 3)
            light_radius = 10.1f;
        if (dist < light_radius)
          att = pow(light_radius - dist, 2);
    }
    if(lightType == 0 || lightType == 1 || lightType == 3)
        color = lightColor * (att*(diffuse_light + specular_light));
    else if(lightType == 2)
    {
        vec3 light_dir = world_position - globe_position;
 
        vec2 texcoord;
        texcoord.x = (1.0 / (2 * 3.14159265359)) * atan (light_dir.x, light_dir.z);
        texcoord.y = (1.0 / 3.14159265359) * acos (light_dir.y / length (light_dir));
        texcoord.x -= time/20.f;
        lightColor = texture (texture_1, texcoord).xyz;
        vec3 lightColor_2 = texture(texture_2, texcoord).xyz;
        vec3 final_color = mix (lightColor, lightColor_2, t);
        if(isGlobe)
            color = final_color;
        else
            color = final_color * (diffuse_light + specular_light);

    }
    return color;
}

void main()
{
     vec3 emissive_light = material_ke;
  
     vec3 light = emissive_light;
     for(int i=0;i<14;i++)
     {
        if(lightColor[i] == vec3(0,0,0))
            continue;
        light += PointLightContribution(lightPos[i], lightColor[i], lightType[i], lightDir[i], lightCutOff[i], lightIntensity[i]);
     }
     out_color += vec4(light, 1.f - isReflector / 2.f);
}
