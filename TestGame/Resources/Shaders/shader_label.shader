<v>
	#version 330 core

	layout ( location = 0 ) in vec3 aPos;
	layout ( location = 1 ) in vec3 aColor;
	layout ( location = 2 ) in vec2 aTexCoord;

	out vec3 vertexColor;
	out vec2 uv;

	uniform mat4 uTransform;
	uniform mat4 uView;
	uniform mat4 uProjection;

	void main () {
		gl_Position = uProjection * uView * uTransform * vec4 ( aPos, 1.0 );
		vertexColor = aColor;
		uv = aTexCoord;
	}
</v>

<f>
	#version 330 core

	in vec3 vertexColor;
	in vec2 uv;

	out vec4 FragColor;

	uniform sampler2D uTexture;

	void main () {
		vec4 tex = texture( uTexture, uv );
		FragColor = vec4(vertexColor, tex.r);
	}
</f>