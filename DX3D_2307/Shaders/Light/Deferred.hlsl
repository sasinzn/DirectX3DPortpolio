#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
};

struct SurfaceData
{
	float linearDepth;
	float3 color;
	float3 normal;
	float specInt;
	float specPow;
};

static const float2 arrBasePos[4] =
{
	float2(-1.0f, +1.0f),
	float2(+1.0f, +1.0f),
	float2(-1.0f, -1.0f),
	float2(+1.0f, -1.0f)
};

PixelInput VS(uint vertexID : SV_VertexID)
{
	PixelInput output;
	output.pos = float4(arrBasePos[vertexID].xy, 0.0f, 1.0f);
	output.uv = output.pos.xy;
	
	return output;
}

Texture2D depthTexture : register(t10);
Texture2D diffuseTexture : register(t11);
Texture2D specularTexture : register(t12);
Texture2D normalTexture : register(t13);

float ConvertDepthToLinear(float depth)
{
	float linearDepth = projection._43 / (depth - projection._33);

	return linearDepth;
}

SurfaceData UnpackGBuffer(int2 location)
{
	SurfaceData output;
	
	int3 location3 = int3(location, 0);
	
	float depth = depthTexture.Load(location3).x;
	output.linearDepth = ConvertDepthToLinear(depth);
	output.color = diffuseTexture.Load(location3).rgb;

	float4 specular = specularTexture.Load(location3);
	output.specPow = MIN_SHININESS + specular.x * MAX_SHININESS;
	output.specInt = specular.y;
	
	output.normal = normalTexture.Load(location3).xyz;
	output.normal = normalize(output.normal * 2.0f - 1.0f);
	
	return output;
}

float3 CalcWorldPos(float2 csPos, float linearDepth)
{
	float4 position;
	
	float2 temp;
	temp.x = 1 / projection._11;
	temp.y = 1 / projection._22;
	position.xy = csPos.xy * temp * linearDepth;
	position.z = linearDepth;
	position.w = 1.0f;
	
	return mul(position, invView).xyz;
}

float4 PS(PixelInput input) : SV_TARGET
{
	SurfaceData data = UnpackGBuffer(input.pos.xy);
	
	LightData lightData;
	lightData.normal = data.normal;
	lightData.diffuseColor = float4(data.color, 1.0f);
	lightData.specularIntensity = data.specInt;
	lightData.viewPos = invView._41_42_43;
	lightData.worldPos = CalcWorldPos(input.uv, data.linearDepth);
	
	return CalcLights(lightData);	
}