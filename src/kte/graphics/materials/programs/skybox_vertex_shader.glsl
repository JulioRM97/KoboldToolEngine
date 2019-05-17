uniform mat4 u_m_projection;
uniform mat4 u_m_view;

in vec3 a_position;

out vec3 v_texCoord;

void main() {
  gl_Position = (u_m_projection * u_m_view * vec4(a_position, 0.0)).xyzz;
  v_texCoord = a_position;
}
