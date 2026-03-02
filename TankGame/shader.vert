#version 330 core

// Attributes
in vec3 aVertexPosition;
in vec3 aVertexNormal;
in vec2 aVertexTexcoord;

out vec2 texcoord;
out vec3 ViewDirection;
out vec3 LightDirection;
out vec3 Normal;


uniform mat4 MVMatrix_uniform;
uniform mat4 ProjMatrix_uniform;
uniform vec3 LightPosition_uniform;


void main() {
	texcoord = aVertexTexcoord;

	vec4 worldPosition = MVMatrix_uniform * vec4(aVertexPosition, 1.0);
	vec4 lightPosition = vec4(LightPosition_uniform, 1.0);

	ViewDirection = -worldPosition.xyz;
	LightDirection = (lightPosition - worldPosition).xyz;
	Normal = mat3(transpose(inverse(MVMatrix_uniform))) * aVertexNormal;

	gl_Position = ProjMatrix_uniform * worldPosition;
}
