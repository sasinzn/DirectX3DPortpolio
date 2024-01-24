//TextureInstancing
#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct VertexInput
{
	float4 pos : POSITION;
	float2 uv : UV;
	
	matrix transform : INSTANCE_TRANSFORM;
	float4 color : INSTANCE_COLOR;
};

struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
	float4 color : COLOR;
};

PixelInput VS(VertexInput input)
{
	PixelInput output;
	output.pos = mul(input.pos, input.transform);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	
	output.uv = input.uv;
	output.color = input.color;
	
	return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
	return diffuseMap.Sample(samp, input.uv) * input.color;
}