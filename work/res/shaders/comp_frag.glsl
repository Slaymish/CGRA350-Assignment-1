#version 330 core

// Uniform data
uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;
uniform vec3 uColor;
uniform vec3 lightPos;
uniform float ambient;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;

// Viewspace data (this must match the output of the vertex shader)
in VertexData {
    vec3 position;
    vec3 normal;
    vec2 textureCoord;
} f_in;

// Framebuffer output
out vec4 fb_color;

void main() {
    // Get texture colors
    vec3 diffuseColor = texture(texture_diffuse, f_in.textureCoord).rgb;
    vec3 normalMap = texture(texture_normal, f_in.textureCoord).rgb * 2.0 - 1.0;

    // Normal maps
    vec3 normal = normalize(normalMap);
    vec3 lightDir = normalize(lightPos - f_in.position);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 ambientLight = ambient * diffuseColor;
    vec3 diffuseLight = diff * diffuseColor;

    // Combine the results
    vec3 color = uColor * (ambientLight + diffuseLight);

    // Output to the framebuffer
    fb_color = vec4(color, 1.0);
}
