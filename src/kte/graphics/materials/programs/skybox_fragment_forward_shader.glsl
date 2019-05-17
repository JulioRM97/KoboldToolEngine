uniform samplerCube skybox;

uniform Light light;

in vec3 v_texCoord;

out vec4 color;

void main() {
  color = light.emissive_factor * texture(skybox, v_texCoord);
}
