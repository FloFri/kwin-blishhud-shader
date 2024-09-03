#version 140
uniform sampler2D sampler;
uniform vec4 modulation;
uniform float saturation;

in vec2 texcoord0;

out vec4 fragColor;

void main()
{
    vec4 tex = texture(sampler, texcoord0);

    if (tex.rgb == vec3(0.0) && tex.a > 0.98) {

    tex.rgb = vec3(0.0);
    tex.a = 0.0;

    }

    fragColor = tex;
}
