//Pointer : 변수의 주소값을 저장하는 변수

//Shader : GUP 프로그래밍

//Sementic : 매개변수 또는 반환자료형에 대한 정보
//SV : SystemValue

//동차변환 : 3차원 공간을 스크린 공간에 출력하기 위한 변환 과정

//cbuffer WVP : register(b0)
//{
//	matrix world;
//	float4x4 view;
//	matrix projection;
//}

cbuffer WorldBuffer : register(b0)
{
	matrix world;
}

cbuffer ViewBuffer : register(b1)
{
	matrix view;
}

cbuffer ProjectionBuffer : register(b2)
{
	matrix projection;
}

struct VertexInput
{
	float4 pos : POSITION;
	float4 color : COLOR;
};

struct PixelInput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

PixelInput VS( VertexInput input )
{
	PixelInput output;	
	output.pos = mul(input.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	
	output.color = input.color;
	
	return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
	return input.color;
}