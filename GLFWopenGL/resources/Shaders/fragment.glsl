#version 330 core

// Stores the material properties of the object.
// Keeps it more organised than storing them as uniforms.
// Also allows you to simulate simple real world materials.
struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D shadowMap;
	float shininess;
};

// Light structures.
// Could be housed in one struct but not all lights need all the variables, asside from a few extra lines of code it saves up memory.
struct DirLight
{
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;
	
	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

#define pointLightAmnt 4

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	vec4 FragPosLightSpace;
} fs_in;

out vec4 FragColor;

uniform DirLight dirLight;
uniform PointLight pointLights[pointLightAmnt];
uniform SpotLight spotLight;

uniform Material material;
uniform vec3 viewPos;

// Function prototypes.
vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir); 
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);  
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float CalculateShadows(vec4 fragPosLightSpace);

void main()
{
	vec3 norm = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    
	// Define an output colour.
	vec3 output = vec3(0.0);
	// Add directional light's contributon.
	output += CalculateDirLight(dirLight, norm, viewDir);
	// Do same for point lights.
	for(int i = 0; i < pointLightAmnt; i++)
	{
		output += CalculatePointLight(pointLights[i], norm, fs_in.FragPos, viewDir);
	}
	// Add spotlights
	output += CalculateSpotLight(spotLight, norm, fs_in.FragPos, viewDir);

	output += CalculateShadows(fs_in.FragPosLightSpace);


	
    FragColor = vec4(output, 1.0);
	

}

// Lighting functions. Abstracting this away from main is a good idea, removes duplicate code and makes it cleaner and simpler to use for multiple lights. 
vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.TexCoords));
    return (ambient + diffuse + specular);
}  

// Simply itterate through this in main to create multiple point lights.
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

// Calculate spotlight torch function.
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

// Calculate the shadows.
float CalculateShadows(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(material.shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}