// Description: Simple shader that applies transform, lighting, specular and displacement
// onto an earth texture with clouds and nightlighting 

cbuffer TransformBuffer : register(b0)
{
	matrix World;
	matrix WVP;
	float3 ViewPosition;
}

Texture2D textureMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D displacementMap : register(t2);
Texture2D normalMap : register(t3);
Texture2D nightLightMap : register(t4);
Texture2D cloudMap : register(t5);
SamplerState textureSampler : register(s0);

cbuffer LightBuffer : register(b1)
{
	float3 LightDirection;
	float4 LightAmbient;
	float4 LightDiffuse;
	float4 LightSpecular;
}

cbuffer MaterialBuffer : register(b2)
{
	float4 MaterialAmbient;
	float4 MaterialDiffuse;
	float4 MaterialSpecular;
	float MaterialPower;
}

cbuffer SettingBuffer : register(b3)
{
	float specularMapWeight : packoffset(c0.x);
	float bumpMapWeight : packoffset(c0.y);
	float normalMapWeight : packoffset(c0.z);
}

struct VSInput
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 texCoord : TEXCOORD0;
};

struct VSOutput
{
	float4 position : SV_Position;
	float3 worldNormal : NORMAL;
	float3 worldTangent : TEXTCOORD0;
	float3 dirToLight : TEXCOORD1;
	float3 dirToView : TEXCOORD2;
	float2 texCoord : TEXCOORD3;

};

struct VSCloudOutput
{
	float4 position : SV_Position;
	float2 texCoord : TEXCOORD;
};



VSOutput VSEarth(VSInput input)
{
	VSOutput output;

	float displacement = displacementMap.SampleLevel(textureSampler, input.texCoord, 0).x;
	float3 localPosition = input.position + (input.normal * displacement * bumpMapWeight);
	float3 worldPosition = mul(float4(localPosition, 1.0f), World).xyz;
	float3 worldNormal = mul(input.normal, (float3x3)World);
	float3 worldTangent = mul(input.tangent, (float3x3)World);

	output.position = mul(float4(localPosition, 1.0f), WVP);
	output.worldNormal = worldNormal;
	output.worldTangent = worldTangent;
	output.dirToLight = -LightDirection;
	output.dirToView = normalize(ViewPosition - worldPosition);
	output.texCoord = input.texCoord;

	return output;
}

// |
// V
// Rasterizer
// |
// V
float4 PSEarth(VSOutput input) : SV_Target
{
	//Renormalize normals from vertex shader
	float3 worldNormal = normalize(input.worldNormal);
	float3 worldTangent = normalize(input.worldTangent);
	float3 worldBinormal = normalize(cross(worldNormal, worldTangent));
	float3 dirToLight = normalize(input.dirToLight);
	float3 dirToView = normalize(input.dirToView);

	float3 normal = worldNormal;
	if (normalMapWeight != 0.0f)
	{
	//Brings us from tangent space all the way to world space (shorcut) 
	float3x3 TBNW = { worldTangent, worldBinormal, worldNormal };
	float4 normalColor = normalMap.Sample(textureSampler, input.texCoord);
	//Grab from the texture normal that is sampled (multiply by 2 - 1 renormalize)
	float3 normalSampled = (normalColor.xyz * 2.0f) - 1.0f;
	normal = mul(normalSampled, TBNW);
	}

	float4 ambient = LightAmbient * MaterialAmbient;

	float diffuseIntensity = saturate(dot(dirToLight, normal));
	float4 diffuse = diffuseIntensity * LightDiffuse * MaterialDiffuse;
	
	float3 halfAngle = normalize(dirToLight + dirToView);
	float specularBase = saturate(dot(halfAngle, normal));
	float specularIntensity = pow(specularBase, MaterialPower);
	float4 specular = specularIntensity * LightSpecular * MaterialSpecular;

	float4 textureColor = textureMap.Sample(textureSampler, input.texCoord);
	float4 specularFactor = specularMap.Sample(textureSampler, input.texCoord).r;
	float4 nightColor = nightLightMap.Sample(textureSampler, input.texCoord);
	//float4 color = (ambient + diffuse) * textureColor + specular;

	textureColor = lerp(textureColor, nightColor, dot(LightDirection, worldNormal));

	float4 color = (ambient + diffuse) * textureColor + nightColor + specular *
		(specularMapWeight != 0.0f ? specularFactor : 1.0f);

	return color;
}


VSCloudOutput VSCloud(VSInput input)
{
	VSCloudOutput output;

	float3 localPosition = input.position + (input.normal * (0.15f + bumpMapWeight));

	output.position = mul(float4(localPosition, 1.0f), WVP);
	output.texCoord = input.texCoord;

	return output;
}

float4 PSCloud(VSCloudOutput input) : SV_Target
{

	float4 color = cloudMap.Sample(textureSampler, input.texCoord);
	color.a = 0.4;
	return color;
}

