#version 330 core

struct Texture {
  sampler2D texture;
  vec4 factor;
  int texCoord;
};

struct Light {
  vec3 position;
  vec3 direction;
  vec3 color;
  float intensity;
  float constant_attenuation;
  float linear_attenuation;
  float quadratic_attenuation;
  float emissive_factor;
  int falloff_exponent;
  float specular_strength;
  int specular_density;
  vec3 camera_position;
  mat4 projection;
  mat4 view;
  sampler2D shadow_tex;
};

const vec3 dielectricSpecular = vec3(0.04, 0.04, 0.04);
const vec3 black = vec3(0, 0, 0);

vec4 get(in Texture tex, in vec2 p_texCoord_0, in vec2 p_texCoord_1 )
{
  return texture(tex.texture, mix(p_texCoord_0, p_texCoord_1, tex.texCoord)) * tex.factor;
}

float diffuseLighting(in vec3 p_normal, in Light p_light)
{
  return max(dot(normalize(p_normal), p_light.direction), 0.0);
}

float specularLighting(in vec3 p_position, in vec3 p_normal, in Light p_light)
{
    // TODO Get specular information from the texture.
    vec3 lightView = normalize(p_light.camera_position - p_position);
    vec3 reflect = reflect(-p_light.direction, p_normal);
    float spec = pow(max(dot(lightView, reflect), 0.0), p_light.specular_density);

    return p_light.specular_strength * spec;
}

float shadowCalculation(in vec3 p_position, in Light p_light)
{
    if (textureSize(p_light.shadow_tex, 0).x == 0) {
        return 1.0f;
    }

    vec4 fragPosLightSpace = p_light.projection * p_light.view * vec4(p_position, 1.0f);

    vec2 projCoords = fragPosLightSpace.xy / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.x < 0 || projCoords.x > 1 ||
        projCoords.y < 0 || projCoords.y > 1 ||
        fragPosLightSpace.z < 0) {
      return 0.0;
    }

    float closestDepth = texture(p_light.shadow_tex, projCoords).r;
    float currentDepth = fragPosLightSpace.z / fragPosLightSpace.w;

    return currentDepth > closestDepth * 1.001 ? 0.0 : 1.0;
}

vec4 lightColor(in Light p_light, in vec3 p_position)
{
    vec3 light_color;
    float d = distance(p_light.position, p_position);
    vec3 L = normalize(p_light.position - p_position);

    float val = max(dot(-p_light.direction, L), 0.000000001);

    val = pow(val, p_light.falloff_exponent);

    float k = val / ( p_light.constant_attenuation  +
                     (p_light.linear_attenuation * d) +
                     (p_light.quadratic_attenuation * (d * d))
                     );

    light_color = k * p_light.color;

    return vec4(light_color, 1.0f);
}


float ssaoCalculation(in vec2 p_texCoord, in sampler2D p_position_tex,
                      in vec3 p_position, in vec3 p_normal,
                      in sampler2D p_noise_tex, in vec3 p_samples[64],
                      in int p_kernel_size, in mat4 p_projection)
{
   const vec2 noiseScale = vec2(640.0/4.0, 480.0/4.0);

   vec3 randomVec = texture(p_noise_tex, p_texCoord * noiseScale).xyz;

   vec3 tangent = normalize(randomVec - p_normal * dot(randomVec, p_normal));
   vec3 bitangent = cross(p_normal, tangent);
   mat3 TBN  = mat3(tangent, bitangent, p_normal);

   float occlusion = 0.0;
   float radius = 0.5f;

   for(int i = 0; i < p_kernel_size; ++i)
   {
       // get sample position
       vec3 v_sample = TBN * p_samples[i]; // From tangent to view-space
       v_sample      = (p_position + v_sample) * radius;

       vec4 offset = vec4(v_sample, 1.0);
       offset      =  p_projection * offset;    // from view to clip-space
       offset.xyz /= offset.w;               // perspective divide
       offset.xyz  = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0

       float sampleDepth = texture(p_position_tex, offset.xy).z;

       float rangeCheck = smoothstep(0.0, 1.0, radius / abs(p_position.z - sampleDepth));

       occlusion += (sampleDepth >= v_sample.z + 0.025? 1.0 : 0.0) * rangeCheck;

       occlusion = 1.0 - (occlusion / p_kernel_size);
   }

    return occlusion / 1.2f;
}
