#version 330 core

// uniform data
uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;
uniform vec3 uColor;

// viewspace data (this must match the output of the fragment shader)
in VertexData {
	vec3 position;
	vec3 normal;
	vec2 textureCoord;
} f_in;

// framebuffer output
out vec4 fb_color;

void main() {
	vec3 lightPos = vec3(0, 0, 1);
	vec3 eye = normalize(-f_in.position);
	float light = abs(dot(normalize(f_in.normal), eye));

	// Cook-Torrence model
	float roughness = 0.1;
	float fresnel = 0.1;
	float k = 0.1;
	float D = 0.1;
	float G = 0.1;
	float F = 0.1;

	// calculate the reflectance
	vec3 reflectance = (D * G * F) / (4 * dot(normalize(f_in.normal), eye) * dot(normalize(f_in.normal), lightPos));

	vec3 ambient = vec3(0.1, 0.1, 0.1);
	reflectance += ambient;

	vec3 color = uColor * reflectance;

	// output to the frambuffer
	fb_color = vec4(color, 1);
}