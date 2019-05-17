uniform sampler2D spriteTexture;

in vec2 v_texCoord;

out vec4 o_color;

void main() {
  vec4 baseValue = texture(spriteTexture, v_texCoord);

  if (baseValue.a < 0.75) {
    discard;
  }

  o_color = vec4(1.0, 1.0, 1.0, 1.0);
}
