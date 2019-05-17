uniform Texture baseColor;
uniform Texture metallicRoughness;
uniform Texture emissive;
uniform Texture oclussion;
uniform Texture normals;

vec3 v_position;
in vec3 v_normal;
in vec4 v_color;
in vec2 v_texCoord_0;
in vec2 v_texCoord_1;

out vec4 o_color;

void main() {
  o_color = vec4(1.0, 1.0, 1.0, 1.0);
}
