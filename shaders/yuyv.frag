precision mediump float;

uniform sampler2D tex;
uniform float texl_w;
uniform float tex_w;

varying vec2 texCoordOut;

void main()
{
	float y, u, v;
	float pixelx = texCoordOut.x;
	float pixely = texCoordOut.y;
	float xcoord = floor(pixelx * tex_w);
	vec4 luma_chroma = texture2D(tex, vec2(pixelx, pixely));
	y = (luma_chroma.r - 0.0625) * 1.1643;
	if (mod(xcoord, 2.0) == 0.0)
	{
		u = luma_chroma.a;
		v = texture2D(tex, vec2(pixelx + texl_w, pixely)).a;
	}
	else
	{
		v = luma_chroma.a;
		u = texture2D(tex, vec2(pixelx - texl_w, pixely)).a;
	}
	u = u - 0.5;
	v = v - 0.5;
	float r = y + 1.5958 * v;
	float g = y - 0.39173 * u - 0.81290 * v;
	float b = y + 2.017 * u;
	gl_FragColor = vec4(r, g, b, 1.0);
}
