#version 330

// Created an array of 10 for the combined transformations.
uniform mat4 combined_transform[10];

in vec3 vertex_position;
in vec3 vertex_normal;
in vec2 vertex_texcoord;

out vec3 colour_position;
out vec4 normal;
out vec2 varying_texcoord;

void main(void)
{
	varying_texcoord = vertex_texcoord;
	// gl_InstanceID automated instance counter.
	normal = vec4((mat3(combined_transform[gl_InstanceID]) * vertex_normal + 1) / 2,0);
	gl_Position = combined_transform[gl_InstanceID] * vec4(vertex_position, 1.0);
}
