uniform Light light;

uniform sampler2D spriteTexture;

in vec3 v_position;
in vec3 v_normal;
in vec2 v_texCoord;

out vec4 o_color;

void main() {
  vec4 baseValue = texture(spriteTexture, v_texCoord);

  if (baseValue.a < 0.75) {
    discard;
  }

//  float diffuse = diffuseLighting(v_normal, light);
//  float specular = specularLighting(v_position, v_normal, light);
//  float shadow = shadowCalculation(v_position, light);

//  vec4 shaded_color = (1.0 - light.emissive_factor) * shadow * light.intensity * (diffuse + specular) * lightColor(light, v_position) * baseValue;

  o_color = baseValue;
}
