#version 330 core

out vec4 FragColor;

in vec2 texcoords;

uniform sampler2D hdrBuffer;
uniform sampler2D bloomBlur;
uniform bool hdr;
uniform float exposure;

void main()
{
	const float gamma = 2.2;

	vec3 hdrColor = texture(hdrBuffer,texcoords).rgb;
	vec3 bloomColor = texture(bloomBlur,texcoords).rgb;
	hdrColor += bloomColor;

	if(hdr)
	{
		vec3 result = vec3(1.0) - exp(-hdrColor * exposure);

		result  = pow(result,vec3(1.0/gamma));
		FragColor = vec4(result,1.0);
	}
	else{
		vec3 result = pow(hdrColor,vec3(1.0/gamma));
		FragColor = vec4(result,1.0);
	}
}