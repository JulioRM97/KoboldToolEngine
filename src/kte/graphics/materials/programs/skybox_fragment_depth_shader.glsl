uniform Light light;

uniform samplerCube skybox;

in vec3 v_texCoord;

out vec4 o_color;

void main() {
  o_color = vec4(1.0, 1.0, 1.0, 1.0);
}
