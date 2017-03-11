#version 330

uniform sampler2D hex_texture;

in vec3 colour_position;
in vec4 normal;
in vec2 varying_texcoord;

out vec4 fragment_colour;

void main(void)
{
	fragment_colour = texture(hex_texture, varying_texcoord) * normal;
}
