//ProgressBar
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

cbuffer ProgressBuffer : register(b10)
{
    float _percent;
    uint _type;
}

Texture2D backImage : register(t10);


float4 PS(PixelInput input) : SV_TARGET
{
    float4 color = diffuseMap.Sample(samp, input.uv);
    float4 zero = float4(0, 0, 0, 0);

    if (_type == 1)
    {
        if (input.uv.x <= _percent)
            return color;
       
    }
    else if (_type == 2)
    {
        if (input.uv.x >= _percent)
            return color;
    
    }
    else if (_type == 3)
    {
        if (input.uv.y <= _percent)
            return color;
    
    }
    else if (_type == 4)
    {
        if (input.uv.y >= _percent)
            return color;
       
    }
	
    return backImage.Sample(samp, input.uv);
}