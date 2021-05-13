#version 330

in vec4 vCol;
in vec2 vTex;
in vec3 vNorm;
in vec3 vFragPos;

const int MAX_POINT_LIGHTS = 3;

struct Light{
    vec3 colour;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight{
    Light base;
    vec3 direction;
};

struct PointLight{
    Light base;
    vec3 position;
    float constant;
    float linear;
    float exponent;
};

struct Material{
    float specularIntensity;
    float shininess;
};

uniform int pointLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform sampler2D theTexture;
uniform Material material;

uniform vec3 eyePosition;

out vec4 colour;

vec4 CalcLightByDirection(Light light, vec3 direction){
    vec4 ambientColour = vec4(light.colour, 1.0) * light.ambientIntensity;

    float diffuseFactor = max(dot(normalize(vNorm), normalize(direction)), 0.0);
    vec4 diffuseColour = vec4(light.colour, 1.0) * light.diffuseIntensity * diffuseFactor;

    vec4 specularColour = vec4(0.0);
    if(diffuseFactor > 0.0){
        vec3 fragToEye = normalize(eyePosition - vFragPos);
        vec3 reflectedVertex = normalize(reflect(direction, normalize(vNorm) ));
        
        float specularFactor = dot(fragToEye,reflectedVertex);
        if (specularFactor > 0.0){
            specularFactor = pow(specularFactor, material.shininess);
            specularColour = vec4(light.colour * material.specularIntensity * specularFactor, 1.0);

        }
    }
    return (ambientColour + diffuseColour + specularColour);
}

vec4 CalcDirectionalLight(){
    return CalcLightByDirection(directionalLight.base, directionalLight.direction);
}

vec4 CalcPointLights(){
    vec4 totalColour = vec4(0,0,0,0);
    for(int i=0; i<pointLightCount; i++){
        vec3 direction =  vFragPos - pointLights[i].position;
        float dist = length(direction);
        direction = normalize(direction);

        vec4 colour = CalcLightByDirection(pointLights[i].base, direction);
        float attenuation = pointLights[i].exponent * dist * dist + pointLights[i].linear * dist + pointLights[i].constant;

        totalColour += (colour/attenuation);
    }
    return totalColour;
}

void main()
{
    vec4 finalColour = CalcDirectionalLight();
    finalColour += CalcPointLights();
    colour = texture(theTexture, vTex) * finalColour;
}