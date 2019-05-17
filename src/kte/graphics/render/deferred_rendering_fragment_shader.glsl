uniform sampler2D baseColor;
uniform sampler2D emissiveColor;
uniform sampler2D normal;
uniform sampler2D position;

uniform Light light;

uniform sampler2D noise_tex;
uniform mat4 u_m_projection;
uniform vec3 samples[64];


in vec2 v_texCoord;

out vec4 o_color;

void main()
{
  vec4 g_baseColor = texture(baseColor, v_texCoord);
  vec4 g_emissiveColor = texture(emissiveColor, v_texCoord);
  vec3 g_normal = texture(normal, v_texCoord).xyz;
  vec3 g_position = texture(position, v_texCoord).xyz;

  float diffuse = diffuseLighting(g_normal, light);
  float specular = specularLighting(g_position, g_normal, light);
  float shadow = shadowCalculation(g_position, light);
  float occlusion = ssaoCalculation(v_texCoord, position, g_position,
                                    g_normal, noise_tex, samples,
                                    64, u_m_projection);

  vec4 shaded_color = (1.0 - light.emissive_factor)  * occlusion * shadow * light.intensity * (diffuse + specular) * lightColor(light, g_position) * g_baseColor;

  vec4 emissive_color = light.emissive_factor * g_emissiveColor;

  o_color = shaded_color + emissive_color;
}
