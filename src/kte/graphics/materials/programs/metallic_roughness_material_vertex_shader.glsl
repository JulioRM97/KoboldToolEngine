uniform mat4 u_m_projection;
uniform mat4 u_m_view;
uniform mat4 u_m_model;

in vec3 a_position;

in vec3 a_normal;

in vec3 a_v3color_0;
in vec4 a_v4color_0;
in int color_is_vec3;

in vec2 a_texCoord_0;
in vec2 a_texCoord_1;

in vec4 a_tangent;
in vec4 a_joints_0;
in vec4 a_weights_0;

out vec3 v_position;
out vec3 v_normal;
out vec4 v_color;
out vec2 v_texCoord_0;
out vec2 v_texCoord_1;

void main() {
  gl_Position = u_m_projection * u_m_view * u_m_model * vec4(a_position, 1);

  v_position = (u_m_model * vec4(a_position, 1.0)).xyz;
  v_normal = (u_m_model * vec4(a_normal, 0.0)).xyz;
  v_color = vec4(a_v3color_0, 1) * color_is_vec3 + a_v4color_0 + (1 - color_is_vec3);

  v_texCoord_0 = a_texCoord_0;
  v_texCoord_1 = a_texCoord_1;
}

