attribute vec2 position;
attribute vec2 texCoord;

uniform mat4 projection;

varying vec2 texCoordOut;

void main()
{
	vec4 p = vec4(0.0, 0.0, 0.0, 1.0);
	p.xy = position;
	texCoordOut = texCoord;
	gl_Position = projection * p;
}