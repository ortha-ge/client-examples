$input v_colour

#include <bgfx_shader.sh>

void main() {
	gl_FragColor = vec4(v_colour.rgb, 1.0);
}
