#version 120

varying vec4 fragmentColor;
varying vec2 fragmentPosition;
varying vec2 fragmentUV;

//out vec4 color;

uniform float time;
uniform sampler2D sampler;

void main()
{
    vec4 textureColor = texture2D(sampler, fragmentUV);

	gl_FragColor = vec4(fragmentColor.r * (cos(fragmentPosition.x * 4.0 + time) + 1.0) * 0.5,
	fragmentColor.g * (cos(fragmentPosition.y * 8.0 + time) + 1.0) * 0.5,
	fragmentColor.b * (cos(fragmentPosition.x * 2.0 + time) + 1.0) * 0.5, fragmentColor.a) * textureColor;
}
