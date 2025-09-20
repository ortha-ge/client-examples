$input v_texCoord0, v_colour

#include <bgfx_shader.sh>

SAMPLER2D(s_texColour, 0);
uniform vec4 u_alphaColour;

void main() {
    vec4 sampledColour = texture2D(s_texColour, v_texCoord0);
    if (sampledColour.r == 0.0) {
        sampledColour.a = 0.0;
    }

	gl_FragColor = sampledColour.rrra * vec4(v_colour, 1.0);
}
