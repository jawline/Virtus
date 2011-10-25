api = "OpenGL2";

fragment = "varying vec3 normal;
varying vec3 vertex_to_light_vector;

uniform sampler2D texture;

void main()
{
	// Defining The Material Colors
	const vec4 AmbientColor = vec4(0.5, 0.5, 0.5, 1.0);
	const vec4 DiffuseColor = vec4(0.4, 0.4, 0.4, 1.0);

	// Scaling The Input Vector To Length 1
	vec3 normalized_normal = normalize(normal);
	vec3 normalized_vertex_to_light_vector = normalize(vertex_to_light_vector);

	// Calculating The Diffuse Term And Clamping It To [0;1]
	float DiffuseTerm = clamp(dot(normal, vertex_to_light_vector), 0.0, 1.0);

	// Calculating The Final Color
	gl_FragColor = (texture2D( texture, gl_TexCoord[0].st) * AmbientColor) + (DiffuseColor * DiffuseTerm);
}";

vertex = "varying vec3 normal;
varying vec3 vertex_to_light_vector;

void main()
{
	// Transforming The Vertex
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	// Transforming The Normal To ModelView-Space
	normal = gl_NormalMatrix * gl_Normal;

	// Transforming The Vertex Position To ModelView-Space
	vec4 vertex_in_modelview_space = gl_ModelViewMatrix * gl_Vertex;

	// Calculating The Vector From The Vertex Position To The Light Position
	vertex_to_light_vector = vec3(vertex_in_modelview_space) - vec3(gl_LightSource[0].position);

	gl_TexCoord[0] = gl_MultiTexCoord0;
}";
