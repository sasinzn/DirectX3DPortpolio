#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

LightPixelInput VS(VertexUVNormalTangentBlend input)
{
	LightPixelInput output;
	matrix transform;
	
	if (hasAnimation)
		transform = mul(SkinWorld(input.indices, input.weights), world);
	else
		transform = world;
	
	output.pos = mul(input.pos, transform);
	
	output.viewPos = invView._41_42_43;
	output.worldPos = output.pos;
	
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	
	output.uv = input.uv;
	
	output.normal = mul(input.normal, (float3x3) transform);
	output.tangent = mul(input.tangent, (float3x3) transform);
	output.binormal = cross(output.normal, output.tangent);
	
	return output;
}

float4 PS(LightPixelInput input) : SV_TARGET
{
	float4 albedo = diffuseMap.Sample(samp, input.uv);		
	
	LightData lightData = GetLightData(input);
	
	float4 ambient = CalcAmbient(lightData);
	float4 color = CalcLights(lightData);
	float4 emissive = mEmissive;
	
	return color + ambient + emissive;
}