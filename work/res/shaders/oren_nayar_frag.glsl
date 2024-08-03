#version 330 core

// Uniform data
uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;
uniform vec3 uColor;
uniform vec3 lightPos;
uniform float ambient;
uniform float sigma;  // Added sigma uniform

// Viewspace data (this must match the output of the vertex shader)
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

    float NdotL = max(dot(N, L), 0.0);
    float NdotV = max(dot(N, V), 0.0);

    // Compute the angles
    float angleVN = acos(NdotV);
    float angleLN = acos(NdotL);

    // Compute A and B terms
    float alpha = max(angleVN, angleLN);
    float beta = min(angleVN, angleLN);
    
    float sigma2 = sigma * sigma;  // Use sigma for calculations
    float A = 1.0 - 0.5 * (sigma2 / (sigma2 + 0.33));
    float B = 0.45 * (sigma2 / (sigma2 + 0.09));

    // Compute the Oren-Nayar diffuse term
    vec3 v1 = normalize(L - N * NdotL);
    vec3 v2 = normalize(V - N * NdotV);
    float VdotL = max(0.0, dot(v1, v2));

    float oren_nayar = NdotL * (A + B * VdotL * sin(alpha) * tan(beta));

    // Combine the results
    vec3 color = uColor * (oren_nayar + ambient);

    // Output to the framebuffer
    fb_color = vec4(color, 1.0);
}
