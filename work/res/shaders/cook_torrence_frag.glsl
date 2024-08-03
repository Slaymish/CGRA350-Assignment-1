#version 330 core

// Uniform data
uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;
uniform vec3 uColor;
uniform float roughness;
uniform float F0;
uniform vec3 lightPos;
uniform float ambient;

// Viewspace data
in VertexData {
    vec3 position;
    vec3 normal;
    vec2 textureCoord;
} f_in;

// Framebuffer output
out vec4 fb_color;

void main() {
    vec3 N = normalize(f_in.normal);
    vec3 V = normalize(-f_in.position);
    vec3 L = normalize(lightPos - f_in.position);
    vec3 H = normalize(V + L);

    float NdotL = max(dot(N, L), 0.0);
    float NdotV = max(dot(N, V), 0.0);
    float NdotH = max(dot(N, H), 0.0);
    float VdotH = max(dot(V, H), 0.0);

    // Fresnel term using Schlick's approximation
    float F = F0 + (1.0 - F0) * pow(1.0 - VdotH, 5.0);

    // Normal Distribution Function (GGX/Trowbridge-Reitz)
    float alpha = roughness * roughness;
    float alpha2 = alpha * alpha;
    float denom = NdotH * NdotH * (alpha2 - 1.0) + 1.0;
    float D = alpha2 / (3.14159265359 * denom * denom);

    // Geometry term (Smith's method with Schlick-GGX)
    float k = alpha / 2.0;
    float G_V = NdotV / (NdotV * (1.0 - k) + k);
    float G_L = NdotL / (NdotL * (1.0 - k) + k);
    float G = G_V * G_L;

    // Combine the Cook-Torrance BRDF terms
    float specular = (D * F * G) / (4.0 * NdotV * NdotL + 0.0001);

    // Add diffuse term (scaled by 1 - F for energy conservation)
    float diffuse = (1.0 - F) * (1.0 / 3.14159265359);

    // Combine the results
    vec3 color = uColor * (diffuse + specular) * NdotL + ambient * uColor;

    // Output to the framebuffer
    fb_color = vec4(color, 1.0);
}
