// Description: Post processing shader.

cbuffer PostProcessSettingsBuffer : register(b0)
{
	float screenWidth : packoffset(c0.x);
	float screenHeight : packoffset(c0.y);
	float time : packoffset(c0.z);
}

Texture2D textureMap : register(t0);
SamplerState textureSampler : register(s0);

struct VSInput
{
	float3 position	: POSITION;
	float2 texCoord	: TEXCOORD;
};

struct VSOutput
{
	float4 position : SV_Position;
	float2 texCoord : TEXCOORD1;
};

VSOutput VS(VSInput input)
{
	VSOutput output;
	output.position = float4(input.position, 1.0f);
	output.texCoord = input.texCoord;
	return output;
}

float4 PS(VSOutput input) : SV_Target
{
	float2 pos = float2(
		(input.texCoord.x * 2.0f) - 1.0f,
		(input.texCoord.y * -2.0f) + 1.0f);
	float2 offset = normalize(pos) * cos(length(pos) * 100.0f + (time * 10.0f));
	float4 color = textureMap.Sample(textureSampler, input.texCoord + (offset * 0.005f));
	return color;
}

float4 PSNoProcessing(VSOutput input) : SV_Target
{
	return textureMap.Sample(textureSampler, input.texCoord);
}