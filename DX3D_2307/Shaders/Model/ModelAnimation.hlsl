#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 viewDir : VIEWDIR;
};

PixelInput VS(VertexUVNormalTangentBlend input)
{
	PixelInput output;
	matrix transform = mul(SkinWorld(input.indices, input.weights), world);
	
	output.pos = mul(input.pos, transform);
	
	output.viewDir = normalize(output.pos.xyz - invView._41_42_43);
	
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	
	output.uv = input.uv;
	
	output.normal = mul(input.normal, (float3x3) transform);
	output.tangent = mul(input.tangent, (float3x3) transform);
	output.binormal = cross(output.normal, output.tangent);
	
	return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
	float4 albedo = diffuseMap.Sample(samp, input.uv);
	
	float3 T = normalize(input.tangent);
	float3 B = normalize(input.binormal);
	float3 N = normalize(input.normal);
	
	float3 normal = N;
	float3 light = normalize(lightDirection);
	
	if (hasNormalMap)
	{
		float3 normalMapColor = normalMap.Sample(samp, input.uv).rgb;
		normal = normalMapColor * 2.0f - 1.0f; //0~1 -> -1~1
		float3x3 TBN = float3x3(T, B, N);
		normal = normalize(mul(normal, TBN));
	}
	
	float diffuseIntensity = saturate(dot(normal, -light));
	
	float4 specular = 0;
	if (diffuseIntensity > 0)
	{
		float3 halfWay = normalize(input.viewDir + light);
		specular = saturate(dot(normal, -halfWay));
		
		float4 specularIntensity = specularMap.Sample(samp, input.uv);
		specular = pow(specular, shininess) * specularIntensity * mSpecular;
	}
	
	float4 diffuse = albedo * diffuseIntensity * mDiffuse;
	float4 ambient = albedo * mAmbient;
	
	return diffuse + specular + ambient + mEmissive;
}