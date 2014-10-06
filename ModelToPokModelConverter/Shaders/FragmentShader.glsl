#version 400
in vec4 positions;
//in vec4 colorSend;
in vec2 uvsend;
//in mat3 localSurface2World;
in vec3 normals;
flat in ivec4 blendingIndices;
flat in vec4 blendingWeights;

//uniform sampler2D tex;
//uniform sampler2D norm;
//uniform sampler2D ambientOccl;

//uniform bool useColorVectors;//
//uniform bool useLighting;//
//uniform bool useAmbientOccul;//
//uniform bool useNormals;//
//uniform bool useWhite;

uniform mat4 animation[100];
uniform mat4 modelToWorld;
uniform vec4 diffuseLightColor;
//uniform vec4 ambient;
uniform vec3 diffuseLightPosition;
uniform vec3 eyePosition;
uniform float specularIntense;

out vec4 newColor;

vec4 getColor()
{
	//if(useColorVectors)
	//{
	//	return colorSend;
	//}
	//else if (useWhite) return vec4(1,1,1,1);
	//else
	//{
	//	return texture2D(tex, uvsend);
	//}
	return vec4(1,1,1,1);
}

float getDiffusePercent(vec3 lightNormal, vec3 normal)
{
	return clamp(dot(lightNormal, normal), 0, 1);
}

float getSpecularPercent(vec3 lightNormal, vec3 normal)
{
	vec3 reflected = reflect(-lightNormal, normal);
	vec3 normalEye = normalize(eyePosition - vec3(positions));
	if(specularIntense > 0)
	return pow(clamp(dot(normalEye, reflected), 0, 1), specularIntense);
	else
	return pow(clamp(dot(normalEye, reflected), 0, 1), 0);
}

vec4 getAmbientLight()
{
	//if(useAmbientOccul) return ambient * texture2D(ambientOccl, uvsend);
	//else return ambient;
	return vec4(1,1,1,1);
}

vec4 getLighting(vec4 theColor)
{
		vec3 localCoords;
		//if(useNormals)
		//{
		//vec4 theNormals = texture2D(norm, uvsend);
		//localCoords = normalize((2.0 * vec3(theNormals)) - vec3(1,1,1));
		//localCoords = normalize(vec3(modelToWorld * vec4(localSurface2World * localCoords,0)));
		//}
		//else localCoords = normalize(vec3(modelToWorld * vec4(normals,1)));
		vec4 animatedNormal = vec4(0,0,0,0);
		if( blendingIndices.x >= 0) animatedNormal += blendingWeights.x * animation[blendingIndices.x] * vec4(normals, 1);
		if( blendingIndices.y >= 0) animatedNormal += blendingWeights.y * animation[blendingIndices.y] * vec4(normals, 1);
		if( blendingIndices.z >= 0) animatedNormal += blendingWeights.z * animation[blendingIndices.z] * vec4(normals, 1);
		if( blendingIndices.w >= 0) animatedNormal += blendingWeights.w * animation[blendingIndices.w] * vec4(normals, 1);
		localCoords = normalize(vec3(modelToWorld * vec4(animatedNormal)));
		vec3 normalLight = normalize(diffuseLightPosition - vec3(positions));
		float diffusePercent = getDiffusePercent(normalLight, localCoords);

		float specularPercent = 0;
		if(diffusePercent > 0)
		{
			specularPercent = getSpecularPercent(normalLight, localCoords);
		}

		return clamp(((diffusePercent * diffuseLightColor) + (specularPercent * diffuseLightColor) + (getAmbientLight())) * theColor,0,1);
}

void main()
{
	//vec4 theColor = getColor();

	//if(useLighting)
	//theColor = getLighting(theColor);

	newColor = vec4(1,1,1,1);
};