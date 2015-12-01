attribute vec2 position;
attribute vec2 tex_coord;

uniform mat4 projection;

varying vec2 tex_coord_out;

void main()
{
	vec4 p = vec4(0.0, 0.0, 0.0, 1.0);
	p.xy = position;
	tex_coord_out = tex_coord;
	gl_Position = projection * p;
}