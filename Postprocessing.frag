#version 330 core

out vec4 FragColor;
in vec2 texcoord;

uniform sampler2D ourtexture;

const float offset = 1.0f / 300.0f;
void main()
{
	vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float kernel[9] = float[](
        1.0/14, 1.0/14 , 1.0/14 ,
        1.0/14, 8.0/14 , 1.0/14,
        1.0/14, 1.0/14 , 1.0/14
          
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(ourtexture, texcoord.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

	//FragColor = vec4(col,1.0f);
	FragColor = texture(ourtexture,texcoord);
}