in vec3 a_position;
in vec2 a_texCoord;

out vec2 v_texCoord;

void main()
{
  gl_Position = vec4(a_position, 1);
  v_texCoord = a_texCoord;
}
