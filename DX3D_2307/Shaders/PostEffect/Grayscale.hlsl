#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
};

PixelInput VS(VertexUV input)
{
	PixelInput output;
	output.pos = mul(input.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	
	output.uv = input.uv;
	
	return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
	float4 albedo = diffuseMap.Sample(samp, input.uv);
	
	//float scale = (albedo.r + albedo.g + albedo.b) / 3;
	//float3 grayColor;
	//grayColor.r = albedo.r * 0.3f;
	//grayColor.g = albedo.g * 0.59f;
	//grayColor.b = albedo.b * 0.11f;	
	//
	//float scale = grayColor.r + grayColor.g + grayColor.b;
	
	float scale = dot(albedo.rgb, float3(0.3f, 0.59f, 0.11f));
	
	return scale.xxxx * mDiffuse;
}