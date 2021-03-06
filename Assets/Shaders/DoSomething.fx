// Description: Simple Shader that does coloring
// HOMEWORK
struct VS_OUTPUT
{
	float4 position : SV_Position;
	float4 color : COLOR;
};
VS_OUTPUT VS(float3 position : POSITION, float4 color : COLOR)
{
	VS_OUTPUT output;
	output.position = float4(position, 1.0f);
	output.color = color;
	return output;
}
//  |
//  V
// Rasterizer
//  |
//  V
float4 PS(VS_OUTPUT input) : SV_Target
{
	return input.color; // Yellow
}