// Description: Terrain shader for MAGE Engine.

cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix WVP;
	float3 ViewPosition;
	float4 LightDirection;
	float4 LightAmbient;
	float4 LightDiffuse;
	float4 LightSpecular;
}

Texture2D diffuseMap : register(t0);
SamplerState textureSampler : register(s0);
	
struct VS_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 texCoord	: TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float3 worldNormal : NORMAL;
	float3 worldTangent : TEXCOORD0;
	float3 dirToView : TEXCOORD2;
	float2 texCoord	: TEXCOORD3;

};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.position = mul(float4(input.position, 1.0f), WVP);
	output.worldNormal = mul(input.normal, (float3x3)World);
	output.worldTangent = mul(input.tangent, (float3x3)World);
	output.dirToView = ViewPosition - mul(input.position.xyz, (float3x3)World);
	output.texCoord = input.texCoord * 10.0f;

	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	// Renormalize normals from vertex shader
	float3 normal = normalize(input.worldNormal);
	float3 dirToLight = normalize(-LightDirection.xyz);
	float3 dirToView = normalize(input.dirToView);

	float4 ambient = LightAmbient;
	float4 diffuse = LightDiffuse * saturate(dot(normal, dirToLight));

	float4 textureColor = diffuseMap.Sample(textureSampler, input.texCoord);

	return (ambient + diffuse) * textureColor;
}