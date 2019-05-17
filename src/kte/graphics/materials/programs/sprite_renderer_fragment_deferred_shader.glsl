uniform sampler2D spriteTexture;

in vec3 v_position;
in vec3 v_normal;
in vec2 v_texCoord;

out vec4 o_color;
out vec4 o_emmisiveColor;
out vec4 o_normal;
out vec4 o_position;

void main() {
  vec4 baseValue = texture(spriteTexture, v_texCoord);

  if (baseValue.a < 0.75) {
    discard;
  }

  o_color = vec4(0);
  o_emmisiveColor = baseValue;
  o_normal = vec4(v_normal, 0);
  o_position = vec4(v_position, 0);
}
