$input v_color

#include <bgfx_shader.sh>

void main() {
	gl_FragColor = vec4(v_color.rgb, 1.0);
}
