#version 330

// Input
in vec3 world_position;
in vec3 world_normal;


uniform vec3 pointLightPos[9];
uniform vec3 pointLightColor[9];

uniform vec3 eye_position;

uniform vec3 material_kd;
uniform vec3 material_ks;
uniform int material_shininess;
uniform vec3 material_ke;

uniform vec3 sourceIntensity;

// Output
layout(location = 0) out vec4 out_color;
  

vec3 PointLightContribution(vec3 lightPos, vec3 lightColor)
{
    vec3 L = normalize(lightPos - world_position);
    vec3 V = normalize(eye_position - world_position);
    vec3 H = normalize(L + V);

	vec3 color = vec3(0,0,0);
    vec3 diffuse_light = material_kd * sourceIntensity * max (dot(world_normal, L), 0);
    vec3 specular_light = vec3(0.f, 0.f, 0.f);

    if (diffuse_light.x > 0 || diffuse_light.y > 0 || diffuse_light.z > 0)
    {
         specular_light = material_ks * sourceIntensity * pow(max(dot(world_normal, H), 0), material_shininess);
    }
    float att = 0.0f;
    float dist = distance(lightPos, world_position);
    float light_radius = 1.1f;
     
    if (dist < light_radius)
          att = pow(light_radius - dist, 2);

    color += lightColor * (att*(diffuse_light + specular_light));
	return color;
}

void main()
{
    vec3 emissive_light = material_ke;
  
    vec3 light = emissive_light;
    for(int i=0;i<9;i++)
    {
        if(pointLightColor[i] == vec3(0,0,0))
            continue;
        light += PointLightContribution(pointLightPos[i], pointLightColor[i]);
    }

    out_color = vec4(light, 1);
}
