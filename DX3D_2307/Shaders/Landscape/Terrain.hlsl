//Terrain
#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

LightPixelInput VS(VertexUVNormalTangent input)
{
	LightPixelInput output;
	output.pos = mul(input.pos, world);
	
	output.viewPos = invView._41_42_43;
	output.worldPos = output.pos;
	
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	
	output.uv = input.uv;
	
	output.normal = mul(input.normal, (float3x3) world);
	output.tangent = mul(input.tangent, (float3x3) world);
	output.binormal = cross(output.normal, output.tangent);
	
	return output;
}

Texture2D alphaMap : register(t10);
Texture2D secondMap : register(t11);

float4 PS(LightPixelInput input) : SV_TARGET
{
	float4 albedo = diffuseMap.Sample(samp, input.uv);
	float4 alpha = alphaMap.Sample(samp, input.uv);
	float4 second = secondMap.Sample(samp, input.uv);	
	
	albedo = lerp(albedo, second, alpha.r);
	
	LightData lightData = GetLightData(input);
	
	float4 ambient = CalcAmbient(lightData);
	float4 color = CalcLights(lightData);
	float4 emissive = mEmissive;
	
	return color + ambient + emissive;
}