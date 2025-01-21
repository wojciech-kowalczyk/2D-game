#version 460 core

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in vec4 aColor;

out vec4 vertexColor;
out vec2 vertexTexCoords;

void main() {
	gl_Position = vec4(aPosition, 0.0, 1.0);
	vertexTexCoords = aTexCoords;
	vertexColor = aColor;
}
