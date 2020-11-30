//��۰��ٕϐ�.
//ø����́Aڼ޽� t(n).
Texture2D g_Texture : register(t0);
//����ׂ́Aڼ޽� s(n).
SamplerState g_SamLinear : register(s0);

//�ݽ����ޯ̧.
cbuffer global : register(b0)
{
	matrix g_mWVP; // ܰ��ލs��.
};

//�\����.
struct VS_OUTPUT
{
	float4 Pos : SV_Position;
	float2 Tex : TEXCOORD;
};

VS_OUTPUT VS_Main(
	float4 Pos : POSITION,
	float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT) 0;
	output.Pos = mul(Pos, g_mWVP);
	output.Tex = Tex;

	return output;
}

// �s�N�Z���V�F�[�_.
float4 PS_Main(VS_OUTPUT input) : SV_Target
{
	float4 color = g_Texture.Sample( g_SamLinear, input.Tex );
	clip(color.a);
	return color;
}