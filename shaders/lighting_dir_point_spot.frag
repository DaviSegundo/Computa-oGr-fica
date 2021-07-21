#version 330 core

struct Material 
{
    vec3 ambient;
    sampler2D diffuseMap;
    vec3 specular;
    float shininess;
};

struct DirectionalLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	int on;
};

struct PointLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	int on;

	float constant;
	float linear;
	float exponent;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float cosInnerCone;
	float cosOuterCone;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	int on;

	float constant;
	float linear;
	float exponent;
};

  
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
in vec4 FragPosLightSpace;

#define MAX_POINT_LIGHTS 4

uniform DirectionalLight sunLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

uniform sampler2D shadowMap;

uniform vec3 viewPos;
uniform vec3 lightPos;

out vec4 frag_color;

vec4 calcDirectionalLightColor(DirectionalLight light, vec3 normal, vec3 viewDir, vec4 fragPosLightSpace);
vec4 calcPointLightColor(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 fragPosLightSpace);
vec4 calcSpotLightColor(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 fragPosLightSpace);
float ShadowCalculation(vec4 fragPosLightSpace);

//-----------------------------------------------------------------------------------------------
// Main Shader Entry
//-----------------------------------------------------------------------------------------------
void main()
{ 
	vec3 normal = normalize(Normal);  
	vec3 viewDir = normalize(viewPos - FragPos);

    // Ambient ----------------------------------------------------------------------------------
	vec3 ambient = spotLight.ambient * material.ambient * vec3(texture(material.diffuseMap, TexCoord));
	vec4 outColor;

	
	if (sunLight.on == 1){
		outColor += calcDirectionalLightColor(sunLight, normal, viewDir, FragPosLightSpace);
	};

   for(int i = 0; i < MAX_POINT_LIGHTS; i++){
        if (pointLights[i].on == 1){
			outColor += calcPointLightColor(pointLights[i], normal, FragPos, viewDir, FragPosLightSpace);}
		};
	
	
	// If the light isn't on then just return 0 for diffuse and specular colors
	if (spotLight.on == 1)
		outColor += calcSpotLightColor(spotLight, normal, FragPos, viewDir, FragPosLightSpace);

	frag_color = outColor;
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

//-----------------------------------------------------------------------------------------------
// Calculate the direction light effect and return the resulting 
// diffuse and specular color summation
//-----------------------------------------------------------------------------------------------
vec4 calcDirectionalLightColor(DirectionalLight light, vec3 normal, vec3 viewDir, vec4 fragPosLightSpace)
{
	vec3 color = vec3(texture(material.diffuseMap, TexCoord));
	vec3 lightDir = normalize(-light.direction);  // negate => Must be a direction from fragment towards the light

	vec3 ambient = 0.15 * color;

	// Diffuse ------------------------------------------------------------------------- --------
    float NdotL = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * NdotL * vec3(texture(material.diffuseMap, TexCoord));
    
     // Specular - Blinn-Phong ------------------------------------------------------------------
	vec3 halfDir = normalize(lightDir + viewDir);
	float NDotH = max(dot(normal, halfDir), 0.0f);
	vec3 specular = light.specular * material.specular * pow(NDotH, material.shininess);

	float shadow = ShadowCalculation(fragPosLightSpace);       
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
	
	return vec4(lighting, 1.0f);
}


//-----------------------------------------------------------------------------------------------
// Calculate the point light effect and return the resulting diffuse and specular color summation
//-----------------------------------------------------------------------------------------------
vec4 calcPointLightColor(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 fragPosLightSpace)
{	
	vec3 color = vec3(texture(material.diffuseMap, TexCoord));
	vec3 lightDir = normalize(light.position - fragPos);

	vec3 ambient = 0.15 * color;

	// Diffuse ----------------------------------------------------------------------------------
    float NdotL = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * NdotL * color;
    
     // Specular - Blinn-Phong ------------------------------------------------------------------
	vec3 halfDir = normalize(lightDir + viewDir);
	float NDotH = max(dot(normal, halfDir), 0.0f);
	vec3 specular = light.specular * material.specular * pow(NDotH, material.shininess);

	// Attenuation using Kc, Kl, Kq -------------------------------------------------------------
	float d = length(light.position - FragPos);
	float attenuation = 1.0f / (light.constant + light.linear * d + light.exponent * (d * d));

	diffuse *= attenuation;
	specular *= attenuation;

	float shadow = ShadowCalculation(fragPosLightSpace);       
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
	
	return vec4(lighting, 1.0f);
}

//------------------------------------------------------------------------------------------------
// Calculate the spotlight effect and return the resulting // diffuse and specular color summation
//------------------------------------------------------------------------------------------------
vec4 calcSpotLightColor(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 fragPosLightSpace)
{
	vec3 color = vec3(texture(material.diffuseMap, TexCoord));
	vec3 lightDir = normalize(light.position - fragPos);
	vec3 spotDir  = normalize(light.direction);

	vec3 ambient = 0.15 * color;

	float cosDir = dot(-lightDir, spotDir);  // angle between the lights direction vector and spotlights direction vector
	float spotIntensity = smoothstep(light.cosOuterCone, light.cosInnerCone, cosDir);

	// Diffuse ----------------------------------------------------------------------------------
    float NdotL = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = spotLight.diffuse * NdotL * color;
    
     // Specular - Blinn-Phong ------------------------------------------------------------------
	vec3 halfDir = normalize(lightDir + viewDir);
	float NDotH = max(dot(normal, halfDir), 0.0f);
	vec3 specular = light.specular * material.specular * pow(NDotH, material.shininess);

	// Attenuation using Kc, Kl, Kq -------------------------------------------------------------
	float d = length(light.position - FragPos);
	float attenuation = 1.0f / (light.constant + light.linear * d + light.exponent * (d * d));

	diffuse *= attenuation * spotIntensity;
	specular *= attenuation * spotIntensity;
	
	float shadow = ShadowCalculation(fragPosLightSpace);       
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
	
	return vec4(lighting, 1.0f);
}