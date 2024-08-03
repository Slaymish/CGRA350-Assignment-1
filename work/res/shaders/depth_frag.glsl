#version 330 core

// uniform data
uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;
uniform vec3 uColor;
uniform vec3 uLightPos;
uniform sampler2D uShadowMap;

// viewspace data (this must match the output of the fragment shader)
in VertexData {
    vec3 position;
    vec3 normal;
    vec2 textureCoord;
    vec4 fragPosLightSpace;
} f_in;

// framebuffer output
out vec4 fb_color;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // Perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(uShadowMap, projCoords.xy).r; 
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // Check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

    return shadow;
}

void main() {
    // calculate lighting (improved version)
    vec3 lightDir = normalize(uLightPos - f_in.position);
    vec3 normal = normalize(f_in.normal);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 eye = normalize(-f_in.position);
    vec3 halfwayDir = normalize(lightDir + eye);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    // calculate shadow
    float shadow = ShadowCalculation(f_in.fragPosLightSpace);

    // combine lighting components
    vec3 ambient = 0.2 * uColor;
    vec3 diffuse = diff * uColor;
    vec3 specular = spec * vec3(0.3);
    vec3 lighting = ambient + (1.0 - shadow) * (diffuse + specular);

    // output to the frambuffer
    fb_color = vec4(lighting, 1.0);
}