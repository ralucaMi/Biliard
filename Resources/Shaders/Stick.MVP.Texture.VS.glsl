#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Charge;

out vec3 frag_normal;
out vec3 frag_color;
out vec2 tex_coord;

void main()
{
	frag_normal = v_normal;
	frag_color = v_color;
	tex_coord = v_texture_coord;
	vec3 pos = v_position;
	pos.z += Charge;
	gl_Position = Projection * View * Model * vec4(pos, 1.0);
}
