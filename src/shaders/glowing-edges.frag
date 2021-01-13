#version 330 core
out vec4 FragColor;

in vec2 TexCoords;



uniform float u_screenWidth;
uniform float u_screenHeight;
float step_w = 1.0/u_screenWidth;
float step_h = 1.0/u_screenHeight;

uniform sampler2D u_screenTexture;	
#define KERNEL_SIZE 9

void main(void) 
{	
    vec2 offsets[KERNEL_SIZE] = vec2[](
        vec2(-step_w,  step_h), // top-left
        vec2( 0.0f,    step_h), // top-center
        vec2( step_w,  step_h), // top-right
        vec2(-step_w,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( step_w,  0.0f),   // center-right
        vec2(-step_w, -step_h), // bottom-left
        vec2( 0.0f,   -step_h), // bottom-center
        vec2( step_w, -step_h)  // bottom-right    
    );

    float kernel[KERNEL_SIZE] = float[](
        1, 2, 1,
        0,  0, 0,
        -1, -2, -1
    );

	vec3 sampleTex[KERNEL_SIZE];
	for(int i = 0; i < KERNEL_SIZE; i++)
	{
		sampleTex[i] = vec3(texture(u_screenTexture, TexCoords.st + offsets[i]));
	}

	vec3 color = vec3(0.0);
    for(int i = 0; i < KERNEL_SIZE; i++)
	{
        color += sampleTex[i] * kernel[i];
	}		

	FragColor = vec4(color, 1.0);
}

  