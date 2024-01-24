//Pointer : ������ �ּҰ��� �����ϴ� ����

//Shader : GUP ���α׷���

//Sementic : �Ű����� �Ǵ� ��ȯ�ڷ����� ���� ����
//SV : SystemValue

//������ȯ : 3���� ������ ��ũ�� ������ ����ϱ� ���� ��ȯ ����

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