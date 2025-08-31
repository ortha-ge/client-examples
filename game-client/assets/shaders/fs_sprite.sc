$input v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(s_texColour, 0);
uniform vec4 u_alphaColour;

void main() {
    vec4 sampledColour = texture2D(s_texColour, v_texcoord0);

    if (abs(sampledColour.x - u_alphaColour.x) + abs(sampledColour.y - u_alphaColour.y) + abs(sampledColour.z - u_alphaColour.z) < 0.1) {
        sampledColour.a = 0.0;
        sampledColour.r = 0.0;
        sampledColour.g = 0.0;
        sampledColour.b = 0.0;
        //discard;
    }

	gl_FragColor = sampledColour;
}
