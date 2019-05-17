uniform sampler2D screenTexture;

in vec2 v_texCoord;

out vec4 o_color;

void main() {
  int lod = 5;
  int radius = 2;
  int radiussq = radius * radius;

  vec2 texelSize = 1.0 / textureSize(screenTexture, lod);

  vec4 blur = vec4(0);
  float count = 0;
  for (int u = -radius; u <= radius; u++) {
    for (int v = -radius; v <= radius; v++) {
        float d = 1.0 - clamp(u * u + v * v - radiussq, 0, radiussq) / radiussq;
        blur += textureLod(screenTexture, v_texCoord + vec2(u, v) * texelSize, lod) * d;
        count += d;
    }
  }

  vec4 cpure = texture(screenTexture, v_texCoord);
  vec4 cblur = blur / count;
  vec4 cmix = mix(cpure, cblur, 0.25);

  o_color = vec4(1) - (vec4(1) - cpure) * (vec4(1) - cblur);
}

