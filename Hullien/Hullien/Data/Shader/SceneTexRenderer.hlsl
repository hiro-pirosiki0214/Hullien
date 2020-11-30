#include "FXAA.hlsl"

//��۰��ٕϐ�.
//ø����́Aڼ޽� t(n).
Texture2D g_TextureColor	: register(t0);	// �F���.
Texture2D g_TextureNormal	: register(t1);	// �@�����.
Texture2D g_TextureDepth	: register(t2);	// �[�x���.
Texture2D g_TextureTrans	: register(t3);	// �A���t�@���.
Texture2D g_TextureLast		: register(t4);	// �֊s���Ȃǂ�`�悵���e�N�X�`��.
//����ׂ́Aڼ޽� s(n).
SamplerState g_SamLinear : register(s0);

//�ݽ����ޯ̧.
cbuffer cBuffer : register(b0)
{
	matrix g_mW			: packoffset(c0); // ܰ��ލs��.
	float2 g_vViewPort	: packoffset(c4); // �r���[�|�[�g��.
};

//�\����.
struct VS_OUTPUT
{
	float4 Pos			: SV_Position;
	float4 Color		: COLOR;
	float2 Tex			: TEXCOORD0;
	float4 OutLineColor : TEXCOORD1;
};

VS_OUTPUT VS_Main(
	float4 Pos : POSITION,
	float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT) 0;
	output.Pos = mul(Pos, g_mW);

	// �X�N���[�����W�ɍ��킹��v�Z,
	output.Pos.x = (output.Pos.x / g_vViewPort.x) * 2.0f - 1.0f;
	output.Pos.y = 1.0f - (output.Pos.y / g_vViewPort.y) * 2.0f;

	output.Tex = Tex;
//	output.OutLineColor = float4(output.Pos.x + 0.7f, 0.0f, output.Pos.y + 0.7f, 1.0f);
	output.OutLineColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	
	return output;
}

// �s�N�Z���V�F�[�_.
float4 PS_Main(VS_OUTPUT input) : SV_Target
{
	// ���჌���Y.
	/*
	const float f = 1.0f;	// ����.
	float2 pos = input.Tex * 2.0f - 1.0f;
	float p = length(pos);
	pos = (1 + f * p * p) / (1 + 2 * f) * pos;
	float4 color = g_TextureColor.Sample(g_SamLinear, pos*0.5+0.5);
	*/
	
	// �`�悳�ꂽ���f���Ȃǂ̐F�����擾.
	float4 color = g_TextureColor.Sample(g_SamLinear, input.Tex);
	
	//----------------------------------------------------------------.
	// �֊s��.
	//----------------------------------------------------------------.
	float imageSizeW, imageSizeh, levels;
	// �e�N�X�`���̃T�C�Y���擾.
	g_TextureNormal.GetDimensions(0, imageSizeW, imageSizeh, levels);
	
	const float s = 0.25f;	// �T���v�����O���鋭��.
	const float dx = 1.0f / imageSizeW;
	const float dy = 1.0f / imageSizeh;
	const float px = s * dx, py = s * dx;
	
	//----------------------------------------------------------------.
	// �@���̏����擾.
	float3 normColor = float3(0.0f, 0.0f, 0.0f);
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2(	-px,	-py)).xyz; // ����.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2( 0 * px,	-py)).xyz; // ��.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2(	 px,	-py)).xyz; // �E��.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2(	-px, 0 * py)).xyz; // ��.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2( 0 * px, 0 * py)).xyz * -8.0; // ����.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2(	 px, 0 * py)).xyz; // �E.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2(	-px,	 py)).xyz; // ����.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2( 0 * px,	 py)).xyz; // ��.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2(	 px,	 py)).xyz; // �E��.
	
	//----------------------------------------------------------------.
	// �[�x�l�̏����擾.
	float z = g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(0 * dx, 0 * dy)).x; // ����.
	float depthColor11		= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(	 px,	 py)).x; // ����.
	float depthColor10		= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(	 px, 0 * py)).x; // ��.
	float depthColor1_1		= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(	 px,	-py)).x; // �E��.
	float depthColor01		= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2( 0 * px,	 py)).x; // ��.
	float depthColor0_1		= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2( 0 * px,	-py)).x; // �E.
	float depthColor_11		= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(	-px,	 py)).x; // ����.
	float depthColor_10		= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(	-px, 0 * py)).x; // ��.
	float depthColor_1_1	= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(	-px,	-py)).x; // �E��.
	
	// Horizontal.
	float h = (depthColor11	 *  1.0f + depthColor10	  *  2.0f)
			+ (depthColor1_1 *  1.0f + depthColor_11  * -1.0f)
			+ (depthColor_10 * -2.0f + depthColor_1_1 * -1.0f);
	// vertical.
	float v = (depthColor11	 *  1.0f + depthColor01	  *  2.0f)
			+ (depthColor_11 *  1.0f + depthColor1_1  * -1.0f)
			+ (depthColor0_1 * -2.0f + depthColor_1_1 * -1.0f);
	float depth = 1.0f - clamp(abs(h + v), 0.0f, 1.0f);
	
	//----------------------------------------------------------------.
	// �G�t�F�N�g(�p�[�e�B�N��)��`�悵���e�N�X�`���̏����擾.
	float4 transColor = g_TextureTrans.Sample(g_SamLinear, input.Tex);
	// �O���[�X�P�[����.
	float4 grayScale = transColor.r * 0.299 + transColor.g * 0.587 + transColor.b * 0.114;
	grayScale = 1.0f - saturate(grayScale);
	
	// �@�����ƁA�[�x�l�̏�񂪈��ȏ�Ȃ�֊s����\��.
	if (length(normColor) >= 0.72f || abs(depth) < 0.8f)
	{
		grayScale *= 1.0f;
	}
	else
	{
		grayScale *= 0.0f;
	}
	return lerp( color, input.OutLineColor, grayScale);
}

// �s�N�Z���V�F�[�_.
float4 PS_LastMain(VS_OUTPUT input) : SV_Target
{
	FxaaTex tex = { g_SamLinear, g_TextureLast };
	float4 color = float4( FxaaPixelShader( input.Tex, tex, float2( 1.0f/g_vViewPort.x, 1.0f/g_vViewPort.y ) ), 1.0f );
	
	//float imageSizeW, imageSizeh, levels;
	//// �e�N�X�`���̃T�C�Y���擾.
	//g_TextureNormal.GetDimensions(0, imageSizeW, imageSizeh, levels);
	//const float s = 5.0f;	// �T���v�����O���鋭��.
	//const float dx = 1.0f / imageSizeW;
	//const float dy = 1.0f / imageSizeh;
	//const float px = s * dx, py = s * dx;
	
	//float4 normColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	//normColor.xyz += g_TextureColor.Sample(g_SamLinear, input.Tex + float2(	  -px,	-py)).xyz * 0.2f; // ����.
	//normColor.xyz += g_TextureColor.Sample(g_SamLinear, input.Tex + float2(0 * px,	-py)).xyz * 0.2f; // ��.
	//normColor.xyz += g_TextureColor.Sample(g_SamLinear, input.Tex + float2(	   px,	-py)).xyz * 0.2f; // �E��.
	//normColor.xyz += g_TextureColor.Sample(g_SamLinear, input.Tex + float2(	  -px,0 * py)).xyz * 0.2f; // ��.
	//normColor.xyz += g_TextureColor.Sample(g_SamLinear, input.Tex + float2(0 * px,0 * py)).xyz * 0.5f; // ����.
	//normColor.xyz += g_TextureColor.Sample(g_SamLinear, input.Tex + float2(	   px,0 * py)).xyz * 0.2f; // �E.
	//normColor.xyz += g_TextureColor.Sample(g_SamLinear, input.Tex + float2(	  -px,	 py)).xyz * 0.2f; // ����.
	//normColor.xyz += g_TextureColor.Sample(g_SamLinear, input.Tex + float2(0 * px,	 py)).xyz * 0.2f; // ��.
	//normColor.xyz += g_TextureColor.Sample(g_SamLinear, input.Tex + float2(	   px,	 py)).xyz * 0.2f; // �E��.
	//color.xyz += normColor.xyz*0.2f;
	
	return color;
}