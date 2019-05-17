uniform sampler2D screenTexture;

in vec2 v_texCoord;

out vec4 o_color;

void main() {
  o_color = texture(screenTexture, v_texCoord);
}

