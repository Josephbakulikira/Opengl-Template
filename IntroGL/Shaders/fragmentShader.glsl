#version 330

in vec4 vCol;
in vec2 vTex;
in vec3 vNorm;
in vec3 vFragPos;

struct DirectionalLight{
    vec3 colour;
    float ambientIntensity;
    vec3 direction;
    float diffuseIntensity;
};

struct Material{
    float specularIntensity;
    float shininess;
};

uniform sampler2D theTexture;
uniform DirectionalLight directionalLight;
uniform Material material;

uniform vec3 eyePosition;

out vec4 colour;

void main()
{
    vec4 ambientColour = vec4(directionalLight.colour, 1.0) * directionalLight.ambientIntensity;

    float diffuseFactor = max(dot(normalize(vNorm), normalize(directionalLight.direction)), 0.0);
    vec4 diffuseColour = vec4(directionalLight.colour, 1.0) * directionalLight.diffuseIntensity * diffuseFactor;

    vec4 specularColour = vec4(0.0);
    if(diffuseFactor > 0.0){
        vec3 fragToEye = normalize(eyePosition - vFragPos);
        vec3 reflectedVertex = normalize(reflect(directionalLight.direction, normalize(vNorm) ));
        
        float specularFactor = dot(fragToEye,reflectedVertex);
        if (specularFactor > 0.0){
            specularFactor = pow(specularFactor, material.shininess);
            specularColour = vec4(directionalLight.colour * material.specularIntensity * specularFactor, 1.0);

        }
    }

    colour = texture(theTexture, vTex) * (ambientColour + diffuseColour + specularColour);
}