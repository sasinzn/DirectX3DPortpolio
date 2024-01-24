#define MAX_LIGHT 10
#define MIN_SHININESS 0.1f
#define MAX_SHININESS 50.0f

struct LightPixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 viewPos : POSITION0;
    float3 worldPos : POSITION1;
};

//Pixel Struct
struct LightData
{
    float3 normal;
    float4 diffuseColor;
    float4 specularIntensity;
    float3 viewPos;
    float3 worldPos;
};

struct Light
{
    float4 color;
	
    float3 direction;
    int type;
	
    float3 position;
    float range;
	
    float inner;
    float outer;
    float length;
    int active;
};

struct GBufferOutput
{
    float4 diffuse : SV_TARGET0;
    float4 specular : SV_TARGET1;
    float4 normal : SV_TARGET2;
};
//Pixel Contant Buffer;
cbuffer LightBuffer : register(b0)
{
    Light lights[MAX_LIGHT];
	
    int lightCount;
    float3 ambientLight;
    float3 ambientCeil;
}

cbuffer MaterialBuffer : register(b1)
{
    float4 mDiffuse;
    float4 mSpecular;
    float4 mAmbient;
    float4 mEmissive;
	
    float shininess;
    int hasNormalMap;
}

Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D normalMap : register(t2);

SamplerState samp : register(s0);

//Pixel Function
float3 GetNormal(float3 T, float3 B, float3 N, float2 uv)
{
    T = normalize(T);
    B = normalize(B);
    N = normalize(N);
	
	[flatten]
    if (hasNormalMap)
    {
        float3 normal = normalMap.Sample(samp, uv).rgb;
        normal = normal * 2.0f - 1.0f; //0~1 -> -1~1
        float3x3 TBN = float3x3(T, B, N);
        N = normalize(mul(normal, TBN));
    }
	
    return N;
}

LightData GetLightData(LightPixelInput input)
{
    LightData lightData;
    lightData.normal = GetNormal(input.tangent,
		input.binormal, input.normal, input.uv);
    lightData.diffuseColor = diffuseMap.Sample(samp, input.uv);
    lightData.specularIntensity = specularMap.Sample(samp, input.uv);
    lightData.viewPos = input.viewPos;
    lightData.worldPos = input.worldPos;
	
    return lightData;
}

float4 CalcAmbient(LightData lightData)
{
    float up = lightData.normal.y * 0.5f + 0.5f;
	
    float4 result = float4(ambientLight + up * ambientCeil, 1.0f);
	
    return result * lightData.diffuseColor * mAmbient;
}

float4 CalcDirectional(LightData lightData, Light light)
{
    float3 toLight = normalize(light.direction);

    float diffuseIntensity = saturate(dot(lightData.normal, -toLight));
    float4 finalColor = light.color * diffuseIntensity * mDiffuse;
	
	[flatten]
    if (diffuseIntensity > 0)
    {
        float3 viewDir = normalize(lightData.worldPos - lightData.viewPos);
        float3 halfWay = normalize(viewDir + toLight);
        float specular = saturate(dot(lightData.normal, -halfWay));

        finalColor += light.color * pow(specular, shininess)
			* lightData.specularIntensity * mSpecular;
    }
	
    return finalColor * lightData.diffuseColor;
}

float4 CalcPoint(LightData lightData, Light light)
{
    float3 toLight = lightData.worldPos - light.position;
    float distanceToLight = length(toLight);
    toLight /= distanceToLight;

    float diffuseIntensity = saturate(dot(lightData.normal, -toLight));
    float4 finalColor = light.color * diffuseIntensity * mDiffuse;
	
	[flatten]
    if (diffuseIntensity > 0)
    {
        float3 viewDir = normalize(lightData.worldPos - lightData.viewPos);
        float3 halfWay = normalize(viewDir + toLight);
        float specular = saturate(dot(lightData.normal, -halfWay));

        finalColor += light.color * pow(specular, shininess)
			* lightData.specularIntensity * mSpecular;
    }
	
    float distanceToLightNormal = 1.0f - saturate(distanceToLight / light.range);
    float attention = pow(distanceToLightNormal, 2);
	
    return finalColor * lightData.diffuseColor * attention;
}

float4 CalcSpot(LightData lightData, Light light)
{
    float3 toLight = lightData.worldPos - light.position;
    float distanceToLight = length(toLight);
    toLight /= distanceToLight;

    float diffuseIntensity = saturate(dot(lightData.normal, -toLight));
    float4 finalColor = light.color * diffuseIntensity * mDiffuse;
	
	[flatten]
    if (diffuseIntensity > 0)
    {
        float3 viewDir = normalize(lightData.worldPos - lightData.viewPos);
        float3 halfWay = normalize(viewDir + toLight);
        float specular = saturate(dot(lightData.normal, -halfWay));

        finalColor += light.color * pow(specular, shininess)
			* lightData.specularIntensity * mSpecular;
    }
	
    float3 dir = normalize(light.direction);
    float cosAngle = dot(dir, toLight);
	
    float outer = cos(radians(light.outer));
    float inner = 1.0f / cos(radians(light.inner));
	
    float conAttention = saturate((cosAngle - outer) * inner);
    conAttention = pow(conAttention, 2);
	
    float distanceToLightNormal = 1.0f - saturate(distanceToLight / light.range);
    float attention = pow(distanceToLightNormal, 2);
	
    return finalColor * lightData.diffuseColor * attention * conAttention;
}

float4 CalcCapsule(LightData lightData, Light light)
{
    float3 direction = normalize(light.direction);
    float3 start = lightData.worldPos - light.position;
    float distanceOnLine = dot(start, direction) / light.length;
    distanceOnLine = saturate(distanceOnLine) * light.length;
	
    float3 pointOnLine = light.position + direction * distanceOnLine;
	
    float3 toLight = lightData.worldPos - pointOnLine;
    float distanceToLight = length(toLight);
    toLight /= distanceToLight;

    float diffuseIntensity = saturate(dot(lightData.normal, -toLight));
    float4 finalColor = light.color * diffuseIntensity * mDiffuse;
	
	[flatten]
    if (diffuseIntensity > 0)
    {
        float3 viewDir = normalize(lightData.worldPos - lightData.viewPos);
        float3 halfWay = normalize(viewDir + toLight);
        float specular = saturate(dot(lightData.normal, -halfWay));

        finalColor += light.color * pow(specular, shininess)
			* lightData.specularIntensity * mSpecular;
    }
	
    float distanceToLightNormal = 1.0f - saturate(distanceToLight / light.range);
    float attention = pow(distanceToLightNormal, 2);
	
    return finalColor * lightData.diffuseColor * attention;
}

float4 CalcLights(LightData lightData)
{
    float4 color = 0;
	
	[unroll(MAX_LIGHT)]
    for (int i = 0; i < lightCount; i++)
    {
		[flatten]
        if (!lights[i].active)
            continue;
		
		[flatten]
        if (lights[i].type == 0)
            color += CalcDirectional(lightData, lights[i]);
        else if (lights[i].type == 1)
            color += CalcPoint(lightData, lights[i]);
        else if (lights[i].type == 2)
            color += CalcSpot(lightData, lights[i]);
        else if (lights[i].type == 3)
            color += CalcCapsule(lightData, lights[i]);
    }
	
    return color;
}

GBufferOutput PackGBuffer(float3 baseColor, float3 normal, float specularIntensity)
{
    GBufferOutput output;
	
    float specPowNorm = (shininess - MIN_SHININESS) / MAX_SHININESS;

    output.diffuse = float4(baseColor, 1.0f);
    output.specular = float4(specPowNorm, specularIntensity, 0, 1);
    output.normal = float4(normal * 0.5f + 0.5f, 1);

    return output;
}