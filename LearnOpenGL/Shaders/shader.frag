#version 330 core

in vec3 vertexColor;
in vec2 uv;

out vec4 FragColor;

uniform sampler2D uTexture;

void main()
{
	FragColor = texture(uTexture, uv);
}