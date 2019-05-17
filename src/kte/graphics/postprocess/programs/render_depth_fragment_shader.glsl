uniform sampler2D screenTexture;
uniform sampler2D depthTexture;

in vec2 v_texCoord;

out vec4 o_color;

void main()
{
  o_color =  vec4(texture(depthTexture, v_texCoord).r);

 //o_color = vec4(1.0, 0.0, 0.0, 1.0);
}
