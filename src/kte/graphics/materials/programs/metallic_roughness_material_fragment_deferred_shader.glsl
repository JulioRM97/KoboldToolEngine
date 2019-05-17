uniform Texture baseColor;
uniform Texture metallicRoughness;
uniform Texture emissive;
uniform Texture oclussion;
uniform Texture normals;

uniform vec4 metallicFactor;
uniform vec4 roughnessFactor;

in vec4 v_color;
in vec3 v_normal;
in vec3 v_position;

in vec2 v_texCoord_0;
in vec2 v_texCoord_1;

out vec4 o_baseColor;
out vec4 o_emmisiveColor;
out vec4 o_normal;
out vec4 o_position;

void main() {
  o_baseColor = get(baseColor, v_texCoord_0, v_texCoord_1) * v_color;
  o_emmisiveColor = get(emissive, v_texCoord_0, v_texCoord_1);
  o_normal = vec4(normalize(v_normal) * get(normals, v_texCoord_0, v_texCoord_1).xyz, 0);
  o_position = vec4(v_position, 0);
}
