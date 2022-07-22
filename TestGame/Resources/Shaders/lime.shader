<v>
	#version 330 core

	layout ( location = 0 ) in vec3 aPos;
	layout ( location = 1 ) in vec3 aColor;
	layout ( location = 2 ) in vec2 aTexCoord;

	uniform mat4 uTransform;
	uniform mat4 uView;
	uniform mat4 uProjection;

	void main () {
		gl_Position = uProjection * uView * uTransform * vec4 ( aPos, 1.0 );
	}
</v>

<f>
	#version 330 core

	out vec4 FragColor;

	uniform sampler2D uTexture;

	void main () {
		FragColor = vec4 ( 0, 1, 0, 1 );
	}
</f>