//Specular
#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct VertexInput
{
	float4 pos : POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float4 alpha : ALPHA;
};

struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 viewDir : VIEWDIR;
	float3 worldPos : POSITION;
	float4 alpha : ALPHA;
};

PixelInput VS(VertexInput input)
{
	PixelInput output;
	output.pos = mul(input.pos, world);
	
	output.viewDir = normalize(output.pos.xyz - invView._41_42_43);
	output.worldPos = output.pos.xyz;
	
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	
	output.uv = input.uv;
	output.alpha = input.alpha;
	
	output.normal = mul(input.normal, (float3x3) world);	
	
	return output;
}

cbuffer BrushBuffer : register(b10)
{
	int type;
	float3 pickingPos;
	
	float range;
	float3 color;
}

float4 BrushColor(float3 pos)
{
	float2 direction = pos.xz - pickingPos.xz;
	
	float distance = length(direction);

	if (distance <= range)
		return float4(color, 0);

	return float4(0, 0, 0, 0);
}

Texture2D secondDiffuseMap : register(t11);
Texture2D thirdDiffuseMap : register(t12);

float4 PS(PixelInput input) : SV_TARGET
{
	float4 albedo = diffuseMap.Sample(samp, input.uv);
	float4 second = secondDiffuseMap.Sample(samp, input.uv);
	float4 third = thirdDiffuseMap.Sample(samp, input.uv);
	
	albedo = lerp(albedo, second, input.alpha.r);
	albedo = lerp(albedo, third, input.alpha.g);
	
	float4 brush = BrushColor(input.worldPos);
	
	return brush;
}