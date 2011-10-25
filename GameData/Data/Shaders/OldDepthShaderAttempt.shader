api = "OpenGL2";

fragment = "uniform sampler2D depthMap;
uniform sampler2D scene;

float LinearizeDepth(vec2 uv)
{
  float n = 1.0; // camera z near
  float f = 100000.0; // camera z far
  float z = texture2D(depthMap, uv).x;
  return (10.0 * n) / (f + n - z * (f - n));
}

float getBlurSize(float distance)
{
	if (distance == 1.0) return 0.0;
	return ( (10 * distance)) / 1024.0;
}

vec4 blurPixels(float blurSize, vec2 uv)
{
	vec4 result = vec4(0.0, 0.0, 0.0, 0.0);

	
	result += texture2D(scene, vec2(uv.s - 4.0* blurSize, uv.t)) * 0.05;
	result += texture2D(scene, vec2(uv.s - 3.0* blurSize, uv.t)) * 0.09;
	result += texture2D(scene, vec2(uv.s - 2.0* blurSize, uv.t)) * 0.12;
	result += texture2D(scene, vec2(uv.s - blurSize, uv.t)) * 0.15;
	result += texture2D(scene, vec2(uv.s, uv.t)) * 0.16;
	result += texture2D(scene, vec2(uv.s + blurSize, uv.t)) * 0.15;
	result += texture2D(scene, vec2(uv.s + 2.0 * blurSize, uv.t)) * 0.12;
	result += texture2D(scene, vec2(uv.s + 3.0 * blurSize, uv.t)) * 0.09;
	result += texture2D(scene, vec2(uv.s + 4.0 * blurSize, uv.t)) * 0.05;	

	return result;
}

void main()
{
	vec2 uv = gl_TexCoord[0].st;
	float d = LinearizeDepth(uv);

	float blurSize = getBlurSize(d);

	if (blurSize != 0.0)
	{
		gl_FragColor = blurPixels(blurSize, uv);
	}
	else
	{
		gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
	}

}";

vertex = "void main()
{
	// Transforming The Vertex
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
}";

