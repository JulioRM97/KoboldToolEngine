uniform Texture baseColor;
uniform Texture metallicRoughness;
uniform Texture emissive;
uniform Texture oclussion;
uniform Texture normals;

uniform Light light;

uniform vec4 metallicFactor;
uniform vec4 roughnessFactor;

in vec3 v_position;
in vec3 v_normal;
in vec4 v_color;
in vec2 v_texCoord_0;
in vec2 v_texCoord_1;

out vec4 o_color;

void main() {
  vec3 normal_tex = get(normals, v_texCoord_0, v_texCoord_1).xyz;

  float diffuse = diffuseLighting(v_normal, light);
  float specular = specularLighting(v_position, v_normal, light);
  float shadow = shadowCalculation(v_position, light);

  vec4 baseValue = get(baseColor, v_texCoord_0, v_texCoord_1) * v_color;
  vec4 emissiveValue = get(emissive, v_texCoord_0, v_texCoord_1);

  vec4 shaded_color = (1.0 - light.emissive_factor) * shadow * light.intensity * (diffuse + specular) * lightColor(light, v_position) * baseValue;
  vec4 emissive_color = light.emissive_factor * emissiveValue;

  o_color = shaded_color + emissive_color;
}
