$input a_position, a_color0
$output v_colour

#include <bgfx_shader.sh>

void main()
{
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0) );
	v_colour = a_color0;
}
