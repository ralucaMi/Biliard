#version 330

// TODO: get values from fragment shader
in vec3 frag_position;
in vec3 frag_normal;
in vec3 frag_texture;
in vec3 frag_color;

layout(location = 0) out vec4 out_color;

void main()
{
	//initial culoarea: negru, normala: colorat
	// TODO: write pixel out color
	//out_color = vec4(1);
	out_color = vec4(frag_color, 1);
	//out_color = vec4(frag_normal, 1);
}
