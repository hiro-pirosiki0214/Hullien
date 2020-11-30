//��۰��ٕϐ�.
//ø����� ڼ޽� t(n).
Texture2D		g_Texture	: register(t0);
//����ׂ� ڼ޽� s(n).
SamplerState	g_samLinear	: register(s0);

//�ݽ����ޯ̧.
cbuffer global
{
	matrix	g_WVP;	//ܰ���,�ޭ�,��ۼު���݂܂ł̕ϊ��s��.
	float4	g_Color;	//�F.
	float	g_fAlpha;	//���ߒl.
};

//�\����.
struct VS_OUTPUT
{
	float4	Pos : SV_Position;
	float2	Tex	: TEXCOORD;
};

//���_�����.
VS_OUTPUT VS_Main(float4 Pos : POSITION,
				  float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(Pos, g_WVP);
	output.Tex = Tex;

	return output;
}

//�߸�ټ����.
float4 PS_Main(VS_OUTPUT input) : SV_Target
{
	float4 color = g_Color * g_Texture.Sample(g_samLinear, input.Tex);
	color.a *= g_fAlpha;
	clip(color.a);
	return color;
}