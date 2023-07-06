#version 330 core

layout ( location = 0 ) out vec4 FragColor;
layout ( location = 1 ) out vec4 BrightColor;

struct Material{
    sampler2D diffuse;
    sampler2D specular;
	sampler2D shadowMap;
	sampler2D normalMap;
	sampler2D depthMap;
	samplerCube shadowdepthMap;
    float shininess;
};

struct Light{
	vec3 Position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linearterm;
	float quadraticterm;

	float cutoff;
	float outtercutoff;
	
	int blinn;

	bool gamma;
};

in vec2 texcoords;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

uniform sampler2D OurTexture;
uniform Material material;
uniform Light light;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightpos;
uniform vec3 campos;
uniform float heightScale;


float near = 0.1f;
float far = 100.0f;
float point_far = 25.0f;

float LinearizeDepth(float depth);
float Depth();
vec3 DirectionalLight(Light l, vec3 normal, vec3 viewdir);
vec3 PointLight(Light l, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 SpotLight(Light l, vec3 normal, vec3 fragPos, vec3 viewdir);
float ShadowCalculation(Light l, vec3 normals ,vec4 fragPosLightSpace);
float PointShadowCalculation(Light l,vec3 fragpos);
vec2 ParallaxMapping(vec2 texcoord,vec3 viewdir);

void main()
{
	
	vec3 norm = normalize(Normal);
	vec3 viewdir = normalize(campos-FragPos);
	vec3 normal = texture(material.normalMap,texcoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
	
	//vec3 result = DirectionalLight(light,norm,viewdir);
	vec3 result = PointLight(light,normal,FragPos,viewDir);

	FragColor = vec4( result, 1.0f);

	float brightness = dot( FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));

	BrightColor = (brightness > 1.0) ? vec4( FragColor.rgb, 1.0) : vec4(0.0,0.0,0.0,1.0);
	
}

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0;
	return (2.0 * near * far)/(far + near - z *(far - near));
}

float Depth()
{
	return LinearizeDepth(gl_FragCoord.z)/far;
}

vec3 DirectionalLight(Light l, vec3 normal, vec3 viewdir)
{
	vec3 lightdir = normalize(-l.direction);

	float diff = max(dot(normal,lightdir),0.0);

	vec3 reflecDir = reflect(-lightdir,normal);
	vec3 HalfwayDir = normalize(lightdir + viewdir);
	float spec = (l.blinn == 1) ? pow(max(dot(normal,HalfwayDir),0.0),material.shininess) : pow(max(dot(viewdir,reflecDir),0.0),material.shininess);

	vec3 ambient = l.ambient * vec3(texture(material.diffuse,texcoords));
	vec3 diffuse = l.diffuse * diff * vec3(texture(material.diffuse,texcoords));
	vec3 specular = l.specular * spec * vec3(texture(material.specular,texcoords));

	float shadow = ShadowCalculation(l,normal,FragPosLightSpace);

	return (ambient + (1.0 - shadow )*(diffuse + specular));
}

vec3 PointLight(Light l, vec3 normal, vec3 fragPos, vec3 viewdir)
{
	//vec3 lightdir = normalize(l.Position - fragPos);
	vec3 lightdir = normalize(TangentLightPos - TangentFragPos);
	float diff = max(dot(normal,lightdir),0.0);
	vec2 texCoords = texcoords;
	texCoords = ParallaxMapping(texcoords,viewdir);

	if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0) discard;

	vec3 reflecDir = reflect(-lightdir,normal);
	vec3 HalfwayDir = normalize(lightdir + viewdir);

	float spec = (l.blinn == 1) ? pow(max(dot(normal,HalfwayDir),0.0),material.shininess) : pow(max(dot(viewdir,reflecDir),0.0),material.shininess);

	float distance = length ( l.Position - fragPos );
	float attenuation = (l.gamma)? 1.0/distance : 1.0/(l.constant + l.linearterm * distance + l.quadraticterm * (distance*distance));

	vec3 ambient = l.ambient * vec3(texture(material.diffuse,texCoords));
	vec3 diffuse = l.diffuse * diff * vec3(texture(material.diffuse,texCoords));
	vec3 specular = l.specular * spec * vec3(texture(material.specular,texCoords));

	ambient *=attenuation;
	diffuse *=attenuation;
	specular *=attenuation;
	
	float shadow = PointShadowCalculation(l,fragPos);

	vec3 result = (ambient + diffuse + specular);
	if(l.gamma) result = pow(result,vec3(1.0/2.2));
	return result;

}

vec3 SpotLight(Light l, vec3 normal, vec3 fragPos, vec3 viewdir)
{
	vec3 lightdir = normalize(l.Position - fragPos);
	
	float diff = max(dot(normal,lightdir),0.0);

	vec3 reflecDir = reflect(-lightdir,normal);
	vec3 HalfwayDir = normalize(lightdir + viewdir);

	float spec = (l.blinn == 1) ? pow(max(dot(normal,HalfwayDir),0.0),material.shininess) : pow(max(dot(viewdir,reflecDir),0.0),material.shininess);

	float distance = length ( l.Position - fragPos );
	float attenuation = (l.gamma)? 1.0/distance : 1.0/(l.constant + l.linearterm * distance + l.quadraticterm * (distance*distance));

	float theta = dot(lightdir,normalize(-l.direction));
	float epsilone = l.cutoff - l.outtercutoff;
	float intensity = clamp((theta - l.outtercutoff)/epsilone,0.0f,1.0f);

	vec3 ambient = l.ambient * vec3(texture(material.diffuse,texcoords));
	vec3 diffuse = l.diffuse * diff * vec3(texture(material.diffuse,texcoords));
	vec3 specular = l.specular * spec * vec3(texture(material.specular,texcoords));

	ambient *=attenuation;
	diffuse *=attenuation * intensity;
	specular *=attenuation * intensity;

	vec3 result = (ambient + diffuse + specular);
	if(l.gamma) result = pow(result,vec3(1.0/2.2));
	return result;
}

float ShadowCalculation(Light l, vec3 normals,vec4 fragPosLightSpace)
{
	//perform percepective division

	vec3 projCoor = fragPosLightSpace.xyz / fragPosLightSpace.w;

	// transform to range of [0,1]

	projCoor = projCoor * 0.5 + 0.5;

	// calculate closest depth 

	float closestDepth = texture(material.shadowMap,projCoor.xy).r;

	//calculate current depth

	
	float currentDepth = projCoor.z;

	//calculate if the fragement is in shadow or not

	
	vec3 lightdir = normalize(l.Position - FragPos);
	float bias = max(0.05 *(1.0 - dot(normals,lightdir)),0.005);

	//float shadow = (currentDepth - bias > closestDepth) ? 1.0 : 0.0;

	//if(projCoor.z > 1.0) shadow = 0.0;

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(material.shadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(material.shadowMap, projCoor.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}

	return shadow/9.0;
}

float PointShadowCalculation(Light l,vec3 fragpos)
{
	vec3 fragtolight = fragpos - l.Position;

	float closestDepth = texture(material.shadowdepthMap,fragtolight).r;

	closestDepth *= point_far;

	float currentDepth = length(fragtolight);

	float bias = 0.05f;
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	
	return shadow;
}

vec2 ParallaxMapping(vec2 texcoord,vec3 viewdir)
{	
	const float minLayers = 8;
	const float maxLayers = 32;

	float numberOfLayers = mix(maxLayers, minLayers,abs(dot( vec3(0.0,0.0,1.0) , viewdir )));

	float layerDepth = 1.0 / numberOfLayers;

	float currentLayerDepth = 0.0;

	vec2 p = viewdir.xy / viewdir.z * heightScale;
	vec2 deltaTexCoords = p / numberOfLayers;

	vec2 currentTexCoord = texcoord;
	float currentDepthMapValue = texture(material.depthMap, currentTexCoord).r;

	while(currentLayerDepth < currentDepthMapValue)
	{
		currentTexCoord -= deltaTexCoords;

		currentDepthMapValue = texture(material.depthMap,currentTexCoord).r;

		currentLayerDepth += layerDepth; 
	}

	vec2 prevTexCoord = currentTexCoord + deltaTexCoords;

	float afterDepth = currentDepthMapValue - currentLayerDepth;
	float beforeDepth = texture(material.depthMap, prevTexCoord).r - currentLayerDepth + layerDepth;

	float weight = afterDepth / (afterDepth - beforeDepth);
	vec2 FinalTexCoord = prevTexCoord * weight + currentTexCoord * (1.0 - weight);

	return FinalTexCoord;
}