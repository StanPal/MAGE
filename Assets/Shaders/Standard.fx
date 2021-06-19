
cbuffer TransformBuffer : register(b0)
{
	matrix World;
	matrix WVP;
	float3 ViewPosition;
}

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

cbuffer SettingsBuffer : register(b3)
{
	float specularMapWeight : packoffset(c0.x);
	float bumpMapWeight : packoffset(c0.y);
	float normalMapWeight : packoffset(c0.z);
	float aoMapWeight : packoffset(c0.w);
	float brightness : packoffset(c1.x);
	bool useShadow : packoffset(c1.y);
	float depthBias : packoffset(c1.z);
	bool useSkinning : packoffset(c1.w);
}

cbuffer ShadowBuffer : register(b4)
{
	matrix WVPLight;
}

cbuffer BoneTransformBuffer : register(b5)
{
	matrix boneTransforms[128];
}

Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D displacementMap : register(t2);
Texture2D normalMap : register(t3);
Texture2D aoMap : register(t4);
Texture2D depthMap : register(t5);

SamplerState textureSampler : register(s0);

static matrix Identity =
{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

matrix GetBoneTransform(int4 indices, float4 weights)
{
	if (length(weights) <= 0.0f)
		return Identity;

	matrix transform;
	transform = boneTransforms[indices[0]] * weights[0];
	transform += boneTransforms[indices[1]] * weights[1];
	transform += boneTransforms[indices[2]] * weights[2];
	transform += boneTransforms[indices[3]] * weights[3];
	return transform;
}

struct VS_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 texCoord	: TEXCOORD;
	int4 blendIndices : BLENDINDICES;
	float4 blendWeights : BLENDWEIGHT;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float3 worldNormal : NORMAL;
	float3 worldTangent : TEXCOORD0;
	float3 dirToLight : TEXCOORD1;
	float3 dirToView : TEXCOORD2;
	float2 texCoord	: TEXCOORD3;
	float4 positionNDC : TEXCOORD4;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;

	float displacement = displacementMap.SampleLevel(textureSampler, input.texCoord, 0).x;
	float3 localPosition = input.position + (input.normal * displacement * bumpMapWeight);

	matrix toWorld = World;
	matrix toNDC = WVP;
	matrix toNDCLight = WVPLight;
	if (useSkinning)
	{
		matrix toRoot = GetBoneTransform(input.blendIndices, input.blendWeights);
		toWorld = mul(toRoot, World);
		toNDC = mul(toRoot, WVP);
		toNDCLight = mul(toRoot, WVPLight);
	}


	float3 worldPosition = mul(float4(localPosition, 1.0f), toWorld).xyz;
	float3 worldNormal = mul(input.normal, (float3x3)toWorld);
	float3 worldTangent = mul(input.tangent, (float3x3)toWorld);

	output.position = mul(float4(localPosition, 1.0f), toNDC);
	output.worldNormal = worldNormal;
	output.worldTangent = worldTangent;
	output.dirToLight = -LightDirection;
	output.dirToView = normalize(ViewPosition - worldPosition);
	output.texCoord = input.texCoord;

	if (useShadow)
		output.positionNDC = mul(float4(localPosition, 1.0f), toNDCLight);

	return output;
}

//Homework: Test Skinning in a new project - HelloSkeletalAnimation 

float4 PS(VS_OUTPUT input) : SV_Target
{
	// Renormalize normals from vertex shader
	float3 worldNormal = normalize(input.worldNormal);
	float3 worldTangent = normalize(input.worldTangent);
	float3 worldBinormal = normalize(cross(worldNormal, worldTangent));
	float3 dirToLight = normalize(input.dirToLight);
	float3 dirToView = normalize(input.dirToView);

	float3 normal = worldNormal;
	if (normalMapWeight != 0.0f)
	{
		float3x3 TBNW = { worldTangent, worldBinormal, worldNormal };
		float4 normalColor = normalMap.Sample(textureSampler, input.texCoord);
		float3 normalSampled = (normalColor.xyz * 2.0f) - 1.0f;
		normal = mul(normalSampled, TBNW);
	}

	float4 ambient = LightAmbient * MaterialAmbient;
	if (aoMapWeight != 0.0f)
		ambient += aoMap.Sample(textureSampler, input.texCoord);

	float diffuseIntensity = saturate(dot(dirToLight, normal));
	float4 diffuse = diffuseIntensity * LightDiffuse * MaterialDiffuse;

	float3 halfAngle = normalize(dirToLight + dirToView);
	//Saturate is a clamp from 0 to 1
	float specularBase = saturate(dot(halfAngle, normal));
	float specularIntensity = pow(specularBase, MaterialPower);
	float4 specular = specularIntensity * LightSpecular * MaterialSpecular;

	float4 textureColor = diffuseMap.Sample(textureSampler, input.texCoord);
	float specularFactor = 1.0f;
	if (specularMapWeight > 0.0f)
		specularFactor = specularMap.Sample(textureSampler, input.texCoord).r;

	float4 color = (ambient + diffuse) * textureColor * brightness + specular * specularFactor;

	if (useShadow)
	{
		// NDC:   +---------+        UV: +----------->
		//       /   1     /|            |(0, 0)    |
		//      +----|----+ |            |          |            
		//      |    |    | |            |          |
		//   -1 -----+----- 1            |          |
		//      |    |    |/             |----------+
		//      +----|----+              V        (1, 1)
		//          -1
		float actualDepth = 1.0f - input.positionNDC.z / input.positionNDC.w;
		float2 shadowUV = input.positionNDC.xy / input.positionNDC.w;
		shadowUV = (shadowUV + 1.0f) * 0.5f;
		shadowUV.y = 1.0f - shadowUV.y;
		if (saturate(shadowUV.x) == shadowUV.x &&
			saturate(shadowUV.y) == shadowUV.y)
		{
			float savedDepth = depthMap.Sample(textureSampler, shadowUV).r;
			if (savedDepth > actualDepth + depthBias)
				color = ambient * textureColor;
		}
	}

	return color;
}