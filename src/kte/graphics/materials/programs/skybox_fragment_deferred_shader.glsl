uniform samplerCube skybox;

in vec3 v_texCoord;

out vec4 o_color;
out vec4 o_emmisiveColor;
out vec4 o_normal;
out vec4 o_position;

void main() {
  o_color = vec4(0);
  o_emmisiveColor = texture(skybox, v_texCoord);
  o_normal = normalize(-vec4(v_texCoord, 0));
  o_position = vec4(v_texCoord, 0);
}
