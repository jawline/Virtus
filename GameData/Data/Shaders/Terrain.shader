api = "OpenGL2";

fragment = "

uniform sampler2D baseTexture;
uniform sampler2D lightMap;

void main()
{
	vec2 uv = gl_TexCoord[0].st;
	vec4 texel1, texel2, result;
	
	texel1 = texture2D(baseTexture, uv);
	texel2 = texture2D(lightMap, uv);

	result = texel1 * texel2;

	gl_FragColor = result;
}

";

vertex = "

void main()
{
	// Transforming The Vertex
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
}

";
