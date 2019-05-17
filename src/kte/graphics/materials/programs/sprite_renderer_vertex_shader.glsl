uniform mat4 u_m_projection;
uniform mat4 u_m_view;
uniform mat4 u_m_model;

uniform vec2 u_v_center;
uniform vec2 u_v_scale;

in vec2 a_position;

out vec3 v_position;
out vec3 v_normal;
out vec2 v_texCoord;

void main() {
  vec4 position = vec4((a_position - u_v_center) * u_v_scale, 0, 1);

  gl_Position = u_m_projection * u_m_view * u_m_model * position;
  v_position = (u_m_model * position).xyz;
  v_normal = (u_m_model * vec4(1, 0, 0, 0)).xyz;
  v_texCoord = -a_position;
}
